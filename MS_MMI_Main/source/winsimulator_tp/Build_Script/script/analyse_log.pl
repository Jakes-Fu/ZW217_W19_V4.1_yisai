#!/usr/bin/perl

################################################################
#  本程序用于分析make_log文件，根据错误信息，从ClearCase中找出
#发生错误的文件的修改者，并把错误信息发mail报告！  
#  支持ARM和VC++ build log文件分析            
#                                                             
# 2004-04-26      Author: wind.deng
################################################################

use Net::SMTP;

###### 定义全局变量 ##################
@report;
$build_log_file = "..\\build.log";
$VOB_ROOT = "..\\..\\..\\..\\..";

######  读取参数   ###################
#  Usage: ccperl analyse_log.pl project-info
#
#  检查参数
$VAR = $ARGV[0];    #第一个参数
$NUM_ARGV = @ARGV;  #参数的个数

if($NUM_ARGV = 1)
{
	$project_info = $ARGV[0];
}else
{
	$project_info = "Automated Buiding have been finished on ";
}

###### 读取Log文件 ###################
open (inf, $build_log_file) || die ("File open error!!!");
# read file content
@line = <inf>;
# close the file handle
close(inf);

# 过滤make_log中的错误信息
# ARM中的错误信息格式：rror: (包括compile和link的错误)
# VC++中的错误信息格式：error C (表示compile错误)， error LNK (表示link错误)
@report = grep (/rror:/, @line);
unshift(@report, grep(/error C/, @line));
unshift(@report, grep(/error LNK/, @line));
#print @report;

# 按照文件名，把错误信息归类
%file_error = classifyFileError(@report);

# 按照用户mail，把文件及其错误信息归类
%email_file_error = classifyEmailFileError(%file_error);

#print %email_file_error;

# 发送mail, 并记录处理的proceed_mail.log
`del proceed_mail.log` if ( -e "proceed_mail.log");   # 如果存在此文件，则删除
open (outf, ">>..\\proceed_mail.log") || die ("File open error!!!");
$cur_time 	  = localtime();
$project_info = "******".$project_info."  ".$cur_time."******\n";
#print outf ($project_info);

while (($email, $file_error) = each(%email_file_error)) 
{
	# send mail          	
	@to = ($email,';wind.den@spreadtrum.com.cn');
	#@to = ('wind.deng@spreadtrum.com.cn',';wind.deng@spreadtrum.com.cn',';edison.rao@spreadtrum.com.cn');
	$subject = ("Automated Build Report ($cur_time): You got error!!!");
	@msg = 
		("
			$project_info
			$file_error
		");
	
	# 显示到屏幕
	print @to;
	print @msg;
	# 记录到文件
	print outf (@to);
	print outf (@msg);
	# 发送mail
	sci_mail(*to, $subject, *msg);
} 

# close the file handle
close(outf);

######################################################
# 说明： 获取整个VOB库文件列表，并与VC++ build log中的
#        文件匹配，以取得真正的文件路径. 
#        此方法解决了VC++ build log中全部把路径转化为小写的问题。
# 参数：$path
# getRealPath($path)
######################################################
sub getRealPath
{
	my ($patt_file) = @_;
	my $file_list = file_list;
	my $real_path = "";
	#$patt_file = 'd:\wind.deng_smmi_view\ms_code\smmi\source\kernel\c\mmk_app.c';
	#$patt = "mmk_app.c";

	# 对于symlink的文件，不能用完整路径获得相应的CC路径，只能使用文件名来匹配
	$position = rindex($patt_file, "\\");
	$patt_file = substr($patt_file, $position+1);
	#$patt_file =~ s/\\/\\\\/g;
	
	if ( not (-e $file_list))     # 第一次执行，生成文件列表
	{
		print "第一次执行，正在生成文件列表，请稍后……\n";
		`cleartool find -avobs -print > $file_list`;
	}
	
	open (inf, $file_list) || die ("File open error!!!");
	# read file content
	@line = <inf>;
	# close the file handle
	close(inf);

	foreach $line (@line)
	{
		if ($line =~ /$patt_file/i)   #忽略大小写匹配
		{
			$line =~ s/@@//;
			$real_path = $line;
		}
	}

	return $real_path;	
}

######################################################
# 说明： 按照文件名，把错误信息归类
# 参数：@report格式：filenane,error info
# classifyFileError(@report)
######################################################
sub classifyFileError
{
	my (@report) = @_;
	my %file_error;     # %file_error: 文件=>错误信息
	
	foreach $report_line (@report)
	{
		# ARM Link错误和VC++ Link错误
		if ( $report_line =~ /Error: L/ || ( $report_line =~ /error:/ && $report_line =~ /\.o|O/ ) || $report_line =~ /error LNK/ )
		{
			# 如果是link错误，则把文件归结为"Link Error" 
			$file_name = "Link Error" ;
			# 按照文件名关键字，把此文件的错误信息归类
			$file_error{$file_name} = $file_error{$file_name}.$report_line;
		}
		# ARM compile 错误
		elsif ( $report_line =~ /[eE]rror:/ && $report_line =~ /\.c|C/ )
		{
			@list = split (/,/, $report_line);
			# 取出文件名
			$file_name = shift(@list);
			# 按照文件名关键字，把此文件的错误信息归类
			$file_error{$file_name} = $file_error{$file_name}.join("", @list);
		}
		# VC++ compile错误
		elsif ( $report_line =~ /error C/ )
		{
			@list = split (/\(/, $report_line);
			# 取出文件名
			$file_name = shift(@list);
			# 按照文件名关键字，把此文件的错误信息归类
			$file_error{$file_name} = $file_error{$file_name}.join("", @list);
		}
		# 未知错误
		else
		{
		
		}
		#print $file_error{$file_name};
	}

	return %file_error;
}


######################################################
# 说明： 按照用户mail，把文件及其错误信息归类
# classifyEmailFileError(%file_error)
######################################################
sub classifyEmailFileError
{
	my (%file_error) = @_;
	
	my %email_file_error = ();      # 定义关联数组：用户email => 文件+错误信息

	while (($file, $error) = each(%file_error)) 
	{
		# link 错误
		if ( $file eq "Link Error" )
		{
			$username = "AllCMGroupEngineers";
		}
		# compile 错误
		else
		{
			$file = getRealPath($file);
			# 从ClearCase中取得修改文件的用户信息
			$username = `cleartool desc -fmt "%u" $file`;
		}
		
		# 如果出现异常，username不存在，则username为：AllCMGroupEngineers
		if ($username eq "")
		{
			$username = "AllCMGroupEngineers";
		}
		$email = $username."\@spreadtrum.com.cn";
		
		# 按照email关键字，把错误报告进行归类
		$email_file_error{$email} = $email_file_error{$email}
									."\n-------------------------------------------------------------------\n"
									.$file
									."\n-------------------------------------------------------------------\n"
									.$error
									."\n===================================================================\n";
	} 
	
	return %email_file_error;
}




#################################################
# 发送mail子程序
# Usage: sci_mail(*TO, $SUBJECT, *MSG);
#
#################################################
sub sci_mail 
{	
	
	local (*TO, $SUBJECT, *MSG)=@_;
	
	#print(@TO);
	#print($SUBJECT);	
	#print(@MSG);
	
	$smtp = Net::SMTP->new('dc-server'); # connect to your SMTP server
	$smtp->mail( '' );                   # use the sender's address here
	$smtp->to(@TO);                      # recipient's address
	$smtp->data();                       # Start the mail

	# Send the header.
	$smtp->datasend("To: @TO\n");
	$smtp->datasend("From: cathy.cao\@spreadtrum.com.cn\n");
	$smtp->datasend("Subject: $SUBJECT\n");
	$smtp->datasend("\n");

	# Send the body.
	$smtp->datasend(@MSG);               # You can preformat messages
	$smtp->dataend();                    # Finish sending the mail
	$smtp->quit;                         # Close the SMTP connection

}
