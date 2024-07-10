#!/usr/bin/perl

################################################################
#  ���������ڷ���make_log�ļ������ݴ�����Ϣ����ClearCase���ҳ�
#����������ļ����޸��ߣ����Ѵ�����Ϣ��mail���棡  
#  ֧��ARM��VC++ build log�ļ�����            
#                                                             
# 2004-04-26      Author: wind.deng
################################################################

use Net::SMTP;

###### ����ȫ�ֱ��� ##################
@report;
$build_log_file = "..\\build.log";
$VOB_ROOT = "..\\..\\..\\..\\..";

######  ��ȡ����   ###################
#  Usage: ccperl analyse_log.pl project-info
#
#  ������
$VAR = $ARGV[0];    #��һ������
$NUM_ARGV = @ARGV;  #�����ĸ���

if($NUM_ARGV = 1)
{
	$project_info = $ARGV[0];
}else
{
	$project_info = "Automated Buiding have been finished on ";
}

###### ��ȡLog�ļ� ###################
open (inf, $build_log_file) || die ("File open error!!!");
# read file content
@line = <inf>;
# close the file handle
close(inf);

# ����make_log�еĴ�����Ϣ
# ARM�еĴ�����Ϣ��ʽ��rror: (����compile��link�Ĵ���)
# VC++�еĴ�����Ϣ��ʽ��error C (��ʾcompile����)�� error LNK (��ʾlink����)
@report = grep (/rror:/, @line);
unshift(@report, grep(/error C/, @line));
unshift(@report, grep(/error LNK/, @line));
#print @report;

# �����ļ������Ѵ�����Ϣ����
%file_error = classifyFileError(@report);

# �����û�mail�����ļ����������Ϣ����
%email_file_error = classifyEmailFileError(%file_error);

#print %email_file_error;

# ����mail, ����¼�����proceed_mail.log
`del proceed_mail.log` if ( -e "proceed_mail.log");   # ������ڴ��ļ�����ɾ��
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
	
	# ��ʾ����Ļ
	print @to;
	print @msg;
	# ��¼���ļ�
	print outf (@to);
	print outf (@msg);
	# ����mail
	sci_mail(*to, $subject, *msg);
} 

# close the file handle
close(outf);

######################################################
# ˵���� ��ȡ����VOB���ļ��б�����VC++ build log�е�
#        �ļ�ƥ�䣬��ȡ���������ļ�·��. 
#        �˷��������VC++ build log��ȫ����·��ת��ΪСд�����⡣
# ������$path
# getRealPath($path)
######################################################
sub getRealPath
{
	my ($patt_file) = @_;
	my $file_list = file_list;
	my $real_path = "";
	#$patt_file = 'd:\wind.deng_smmi_view\ms_code\smmi\source\kernel\c\mmk_app.c';
	#$patt = "mmk_app.c";

	# ����symlink���ļ�������������·�������Ӧ��CC·����ֻ��ʹ���ļ�����ƥ��
	$position = rindex($patt_file, "\\");
	$patt_file = substr($patt_file, $position+1);
	#$patt_file =~ s/\\/\\\\/g;
	
	if ( not (-e $file_list))     # ��һ��ִ�У������ļ��б�
	{
		print "��һ��ִ�У����������ļ��б����Ժ󡭡�\n";
		`cleartool find -avobs -print > $file_list`;
	}
	
	open (inf, $file_list) || die ("File open error!!!");
	# read file content
	@line = <inf>;
	# close the file handle
	close(inf);

	foreach $line (@line)
	{
		if ($line =~ /$patt_file/i)   #���Դ�Сдƥ��
		{
			$line =~ s/@@//;
			$real_path = $line;
		}
	}

	return $real_path;	
}

######################################################
# ˵���� �����ļ������Ѵ�����Ϣ����
# ������@report��ʽ��filenane,error info
# classifyFileError(@report)
######################################################
sub classifyFileError
{
	my (@report) = @_;
	my %file_error;     # %file_error: �ļ�=>������Ϣ
	
	foreach $report_line (@report)
	{
		# ARM Link�����VC++ Link����
		if ( $report_line =~ /Error: L/ || ( $report_line =~ /error:/ && $report_line =~ /\.o|O/ ) || $report_line =~ /error LNK/ )
		{
			# �����link��������ļ����Ϊ"Link Error" 
			$file_name = "Link Error" ;
			# �����ļ����ؼ��֣��Ѵ��ļ��Ĵ�����Ϣ����
			$file_error{$file_name} = $file_error{$file_name}.$report_line;
		}
		# ARM compile ����
		elsif ( $report_line =~ /[eE]rror:/ && $report_line =~ /\.c|C/ )
		{
			@list = split (/,/, $report_line);
			# ȡ���ļ���
			$file_name = shift(@list);
			# �����ļ����ؼ��֣��Ѵ��ļ��Ĵ�����Ϣ����
			$file_error{$file_name} = $file_error{$file_name}.join("", @list);
		}
		# VC++ compile����
		elsif ( $report_line =~ /error C/ )
		{
			@list = split (/\(/, $report_line);
			# ȡ���ļ���
			$file_name = shift(@list);
			# �����ļ����ؼ��֣��Ѵ��ļ��Ĵ�����Ϣ����
			$file_error{$file_name} = $file_error{$file_name}.join("", @list);
		}
		# δ֪����
		else
		{
		
		}
		#print $file_error{$file_name};
	}

	return %file_error;
}


######################################################
# ˵���� �����û�mail�����ļ����������Ϣ����
# classifyEmailFileError(%file_error)
######################################################
sub classifyEmailFileError
{
	my (%file_error) = @_;
	
	my %email_file_error = ();      # ����������飺�û�email => �ļ�+������Ϣ

	while (($file, $error) = each(%file_error)) 
	{
		# link ����
		if ( $file eq "Link Error" )
		{
			$username = "AllCMGroupEngineers";
		}
		# compile ����
		else
		{
			$file = getRealPath($file);
			# ��ClearCase��ȡ���޸��ļ����û���Ϣ
			$username = `cleartool desc -fmt "%u" $file`;
		}
		
		# ��������쳣��username�����ڣ���usernameΪ��AllCMGroupEngineers
		if ($username eq "")
		{
			$username = "AllCMGroupEngineers";
		}
		$email = $username."\@spreadtrum.com.cn";
		
		# ����email�ؼ��֣��Ѵ��󱨸���й���
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
# ����mail�ӳ���
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
