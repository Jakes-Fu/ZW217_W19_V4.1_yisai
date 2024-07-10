#!/usr/bin/perl -w
BEGIN{push @INC,'./make/perl_script/perl_pm'};
#use Spreadsheet::ParseExcel;
#use Spreadsheet::WriteExcel;


my @g_lang= undef;

my $min_row = undef;
my $min_col = undef;
my $max_row = undef;
my $max_col = undef;

my $str_table_ws = undef;
my $text_ws = undef;
my $text_wb = undef;

my @text_lang_index = ();
my $str_row_count = 1;
my $maxlen_index;
my %str_id_map = ();

my @str_len_error_msg = ();
my @str_id_repeat_msg = ();

my $sub_text_path = undef;
my $cur_row;



sub filter {
    my $work_book = $_[0];
    my $sheetindex= $_[1];
    my $row       = $_[2];
    my $col       = $_[3];
    my $cell      = $_[4];
    
    if($sheetindex>=1)
    {
    	$work_book->ParseAbort(1);
    	return;
    }
}

sub STRART_EXTRACT_SUB_TABLE
{
	my $str_table_path = shift;
	my $langs = shift;
	
	@g_lang = split(",",$langs);	

#	my $parser = Spreadsheet::ParseExcel->new(CellHandler => \&filter,     
#            					  NotSetCell => 0);
#	my $str_table_wb = $parser->Parse($str_table_path);
	my $str_table_wb = Spreadsheet::ParseExcel::Workbook->Parse($str_table_path);
	
	my @wss = @{$str_table_wb->{Worksheet}};	
	$str_table_ws = $wss[0];
	$min_row = $str_table_ws->{MinRow};
	$min_col = $str_table_ws->{MinCol};
	$max_row = $str_table_ws->{MaxRow};
	$max_col = $str_table_ws->{MaxCol};	

	
	my $find_max_len=0;
	
	my $lang_count = scalar(@g_lang);
	
	for($i=0; $i<$lang_count; $i++)
	{
#		print $g_lang[$i]."\r\n";
		my $col= $min_col;
		for(;$col<= $max_col;$col++)
		{
			
			my $cell = $str_table_ws->{Cells}[$min_row][$col]->{Val};
			print "$cell\r\n";		
			if(lc($cell) eq lc($g_lang[$i]))
			{				
				push @text_lang_index,$col;
				last;
			}	
			
			if($find_max_len==0 && 	(lc($cell) eq "maxlength"))
			{
				$maxlen_index=$col;
				$find_max_len = 1;
			}	
		}
		
		if($col > $max_col)
		{
			push @text_lang_index,$col;			
		}
	}
	if($find_max_len == 0)
	{
		die "not found \"MaxLength\" in str_table.xls\n";
	}
	
	
	
	for( $i = 1; $i<=$max_row; $i++)
	{
		my $cell = $str_table_ws->{Cells}[$i][$min_col]->{Val};			
		if($cell ne "")
		{
			if( $str_id_map{$cell} != null)
			{
				my $str_id_repeat= "[str_table.xls] [ID:$cell] [row: $i] is repeated with [row: $str_id_map{$cell}]\n";
				push @str_id_repeat_msg,$str_id_repeat;
			}				
			$str_id_map{$cell} = $i;
		}
	}
	
	print "start extract string from str_table\r\n";
	
}

sub END_EXTRACT_SUB_TABLE
{
	if(scalar(@str_id_repeat_msg)>0)
	{
		foreach $msg (@str_id_repeat_msg)
		{
			print $msg;
		}
		if(scalar(@str_len_error_msg)== 0)
		{
			die "end extract string from str_table failed\n";
		}
	}

	if(scalar(@str_len_error_msg)>0)
	{
		foreach $msg (@str_len_error_msg)
		{
			print $msg;
		}
		
		die "end extract string from str_table failed\n";
	}

        ## release memory	
	%str_id_map = ();
	@str_id_repeat_msg = ();
	@str_len_error_msg = ();
	
	print "end extract string from str_table success\n";
}

sub START_MODULE
{
	my $text_path  = shift;
	my $has_str = shift;
	$sub_text_path = $text_path;
	$str_row_count = 1;
	
	if($has_str == 1)
	{
	
		$text_wb   = Spreadsheet::WriteExcel->new($text_path);
	        $text_ws = $text_wb->add_worksheet(); 
	        
	        $text_ws->write_string(0, 0, "ID");
		
		for($i=0; $i<scalar(@g_lang);$i++)
		{		
			$text_ws->write_string(0, $i+1, $g_lang[$i]);
		}
	
	}	
	
	print "start extract string for \"$text_path\" : $has_str\n";
}

sub END_MODULE
{
	my $has_str = shift;
	
	if($has_str == 1)
	{
		$text_ws = undef;
		$text_wb->close();
		$text_wb = undef;
	}
	print "end extract string: $has_str\n";
}

sub RES_ADD_STRING
{
	my $id 	 = shift;
	my $default_content = shift;	
	
	$text_ws->write_string($str_row_count, 0, $id );
	
	my $row = $str_id_map{$id};
#	print "ID: ". $row . "\r\n";
	if($row)
	{
		my $maxlen = $str_table_ws->{Cells}[$row][$maxlen_index]->{Val};
		my $col_cout = 1;
		foreach my $col (@text_lang_index) 
		{
			if($col <= $max_col)
			{
				my $str = $str_table_ws->{Cells}[$row][$col]->{Val};					
				if($str) 	
				{
					my $code = $str_table_ws->{Cells}[$row][$col]->{Code};	
					#print $str_row_count." ".$col_cout." ".$str."\n";
					if($code && $code eq "ucs2")
					{	
						## if string length is large the max lenght,die						
						my $str_len = rindex $str."\$","\$";
						$str_len = $str_len / 2; ##unicode chars so divied by 2
						if($str_len > $maxlen)
						{
							#die "[TEXT] [$id] [row:$row] [col:$col] string length ($str_len) is larger than the max length($maxlen)\n";
							push @str_len_error_msg,"[TEXT:error] [$id] [row:$row] [col:$col] string length ($str_len) is larger than the max length($maxlen)\n";
						}
						
						$text_ws->write_utf16be_string($str_row_count, $col_cout, $str);
					}
					else
					{
						my $str_len = rindex $str."\$","\$";
						if($str_len > $maxlen)
						{
							#die "[TEXT] [$id] [row:$row] [col:$col] string length ($str_len) is larger than the max length($maxlen)\n";
							push @str_len_error_msg,"[TEXT:error] [$id] [row:$row] [col:$col] string length ($str_len) is larger than the max length($maxlen)\n";
						}
						$text_ws->write_string($str_row_count, $col_cout, $str);
					}
				}
				else
				{
					my $str_len = rindex $default_content."\$","\$";
					if($str_len > $maxlen)
					{
						#die "[TEXT] [$id] default string [$default_content] length ($str_len) is larger than the max length($maxlen)\n";
						push @str_len_error_msg,"[TEXT:error] [$id] default string [$default_content] length ($str_len) is larger than the max length($maxlen)\n";
					}
						
					#print $str_row_count." ".$col_cout." ".$default_content."\n";
					$text_ws->write_string($str_row_count, $col_cout, $default_content );
				}
			}
			else
			{
				my $str_len = rindex $default_content."\$","\$";
				if($str_len > $maxlen)
				{
					#die "[TEXT] [$id] default string [$default_content] length ($str_len) is larger than the max length($maxlen)\n";					
					push @str_len_error_msg,"[TEXT:error] [$id] default string [$default_content] length ($str_len) is larger than the max length($maxlen)\n";
				}
				
				#print $str_row_count." ".$col_cout." ".$default_content."\n";
				$text_ws->write_string($str_row_count, $col_cout, $default_content );
			}
			
			$col_cout++;
		}
	}	
	else
	{
		for(my $i=1;$i <= scalar(@g_lang);$i++)		
		{	
			#print $str_row_count." ".$i." ".$default_content."\n";		
			$text_ws->write_string($str_row_count, $i, $default_content );						
		}
	}
	
	$str_row_count++;
	
	print "RES_ADD_STRING [$id] complete\n";	
}

1;