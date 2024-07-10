#!/usr/bin/perl -w

#########################################################################
## 
## File Name:   xrm_tmp.pl
## Author:      Hongliang.xin
## Description: export function to generate xrm file
## Usage:       
## 
## Modification History
## 2010-11-19   hongliang.xin
##              Add RES_ADD_XXX_RANGE and RES_ADD_XXX_TAG_RANGE
## 2010-10-15   hongliang.xin
##              replace mkpath with mkdir
## 2010-09-08   hongliang.xin
##              Add RES_ADD_XXX_TAG functions
## 2010-09-03   hongliang.xin
##              Create modual resource folder, if it does not exist.
##              e.g. create wifi->MMI_RES_DEFAULT two-level folders.
##              wifi
##                |- MMI_RES_DEFAULT
##
#########################################################################

BEGIN{push @INC,'./make/perl_script/perl_pm'};
use XML::Mini::Document;
use File::Path;

##need not parse excel file
#use Spreadsheet::ParseExcel;
#use Spreadsheet::WriteExcel;

my $key_color = "0x000000";
my $img_cmp_default_str;

my @g_img_cmp_default = undef;
my @g_skin= undef;#("DEFULAT","SKIN1","SKIN2");
my @g_lang= undef;#("English","SimpleChinese");
my $xrm_doc = undef;
my $xrm_root_node= undef;
my $xrm_path= undef;
#my $xrm_first_text_node = undef;
#my $xrm_strings_node = undef;
my $mdu_res_dir = undef;

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
my $str_pl_file_handle;
#######################################################
## param list:
## 1.module define file path
## 2.iamge path
#######################################################
sub START_GEN_XRM{
	my $mdu_def_path = shift;
	$langs = shift;	
	$skins = shift;
	$key_color = shift;
	$img_cmp_default_str = shift;
	$mdu_res_dir = shift;
#test	
#	print "img_cmp----$img_cmp_default_str\n";
#	die;
	
	@g_lang = split(",",$langs);
	@g_skin = split(",",$skins);
	$ncount = scalar(@g_lang);
#	print "langs----$langs------$ncount\n";
#	$ncount = scalar(@g_skin);
#	print "skins----$skins------$ncount\n";
	
	@g_img_cmp_default = split(",",$img_cmp_default_str);
	
		
	$xrm_path = $mdu_def_path;	
	$xrm_path =~ s/^(.*)_mdu_def.h$/$1.xrm/g;
	
	$xrm_doc = XML::Mini::Document->new();
	my $vroot = $xrm_doc->getRoot();
	my $xmlHeader = $vroot->header("xml");	
	$xmlHeader->attribute("encoding", "GB2312");
	$xmlHeader->attribute("version", "1.0");
	$xrm_root_node = $vroot->createChild("XRM");
	$xrm_root_node->attribute("version","x_sci_xrm_v3_0");	

	for(my $i = 0; $i < scalar(@g_skin); $i++)
	{
		my $prd_node = undef;
		if( $i == 0 )
		{
			$prd_node = $xrm_doc->createElement("PRODUCT");	
			$prd_node->attribute("index",$i);		
			$prd_node->attribute("id","0");
			$prd_node->attribute("version","1.0.0");
			$prd_node->attribute("type","PRD");
			$prd_node->attribute("name",$g_skin[$i]);
									
		}
		else
		{
			$prd_node = $xrm_doc->createElement("PRODUCT");	
			$prd_node->attribute("index",$i-1);		
			$prd_node->attribute("id","0");
			$prd_node->attribute("version","1.0.0");
			$prd_node->attribute("type","SKN");
			$prd_node->attribute("name",$g_skin[$i]);
			
		}
		
		$xrm_root_node->appendChild($prd_node);
		
		##create empty image node
		my $imag_node = $xrm_doc->createElement("IMAG");
		$prd_node->appendChild($imag_node);
		
		##create empty anim node
		my $anim_node = $xrm_doc->createElement("ANIM");
		$prd_node->appendChild($anim_node);
		
		##create empty ring node
		my $ring_node = $xrm_doc->createElement("RING");
		$prd_node->appendChild($ring_node);		
		
		##create empty text node
		my $text_node = $xrm_doc->createElement("TEXT");
		$prd_node->appendChild($text_node);	
				
		##init text node	
		#$xrm_first_text_node = $text_node;
		my $text_url_node = $xrm_doc->createElement("URL");
		$text_url_node->text("\\MMI_RES_##\\TEXT\\text.xls");
		$text_node->appendChild($text_url_node);
		my $xrm_strings_node = $xrm_doc->createElement("Strings");
		$text_node->appendChild($xrm_strings_node);
						
		
		##create empty font node
		my $font_node = $xrm_doc->createElement("FONT");
		$prd_node->appendChild($font_node);
				
		##create empty data node
		my $data_node = $xrm_doc->createElement("DATA");
		$prd_node->appendChild($data_node);	
		
		##create empty color node
		my $color_node = $xrm_doc->createElement("COLOR");
		$prd_node->appendChild($color_node);
		
		##create empty rect node
		my $rect_node = $xrm_doc->createElement("RECT");
		$prd_node->appendChild($rect_node);	
	}
	#######################################################
	## add Resource Tag node
	my $tag_node = $xrm_doc->createElement("RESTAG");
	$xrm_root_node->appendChild($tag_node);	
	
	#######################################################
	## create TEXT folder
	#my $text_folder_path = 	$xrm_path;
	#$text_folder_path =~ s/(.*)\/[^\/]*.xrm$/$1\/MMI_RES_DEFAULT\/TEXT/g;	
		
	#if (!(-d $text_folder_path))
	#{
	#	mkpath $text_folder_path or die "can not create folder [$text_folder_path]";
	#}
=head
	$text_file = $text_folder_path;
	$text_file .= "/text.xls";	

	$text_wb   = Spreadsheet::WriteExcel->new($text_file);
        $text_ws = $text_wb->add_worksheet(); 
        
        $text_ws->write_string(0, 0, "ID");
	
	my $str_table_file = $xrm_path;
	$str_table_file =~ s/(.*)\/[^\/]*\/[^\/]*/$1\/str_table.xls/g;	
	my $str_table_wb = Spreadsheet::ParseExcel::Workbook->Parse($str_table_file);
	my @wss = @{$str_table_wb->{Worksheet}};	
	$str_table_ws = $wss[0];
	$min_row = $str_table_ws->{MinRow};
	$min_col = $str_table_ws->{MinCol};
	$max_row = $str_table_ws->{MaxRow};
	$max_col = $str_table_ws->{MaxCol};
	
	my $find_max_len=0;
	
	for($i=0; $i<scalar(@g_lang);$i++)
	{
		my $col= $min_col;
		for(;$col<= $max_col;$col++)
		{
			
			my $cell = $str_table_ws->{Cells}[$min_row][$col]->{Val};			
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
		
		$text_ws->write_string(0, $i+1, $g_lang[$i]);
	}
	if($find_max_len == 0)
	{
		die "not found \"MaxLength\" in str_table.xls\n";
	}
=cut
	
	print "START_GEN_XRM [$xrm_path] complete\n";	
	
}
#######################################################
## param list:
#######################################################
sub END_GEN_XRM{

	my $has_str = shift;
=head	
	if($has_str==1)
	{
		my $text_url_node = $xrm_doc->createElement("URL");
		$text_url_node->text("\\MMI_RES_##\\TEXT\\text.xls");
		$xrm_first_text_node->appendChild($text_url_node);
	}
=cut	
	open(XRM_FILE, ">$xrm_path") or die "Can not open $xrm_path" ;
	print XRM_FILE $xrm_doc->toString();
	close XRM_FILE;	

=head	
	$text_wb->close();
=cut
	print "END_GEN_XRM [$xrm_path] complete\n";

}

sub GetImgCmpIndex{
	my @params;
	my $img_type = shift;
	my $count = scalar(@g_img_cmp_default);
	$img_type = lc($img_type);
	for(my $i = 0; $i < $count;$i= $i + 3)
	{
		my $temp_img_type = $g_img_cmp_default[$i];
		$temp_img_type = lc($temp_img_type);
		if($img_type eq $temp_img_type)
		{
			push @params,$i;
			return wantarray?@params:$params[0];
		}
	}
	push @params, -1;
	return wantarray?@params:$params[0];
}

sub FindAndGetImgCmpIndex{
	$img_path = shift;
	$img_type = shift;	
	$img_cmp_index = -1;
	my @params;
	if(-e $img_path)
	{
		push @params,1;	
	}
	else
	{
		push @params,0;
		push @params,-1;
		return wantarray?@params:$params[0];
	}
	
	if($img_type eq "bmp")
	{
		open(IMG_FILE,$img_path) or die "can not open file ($img_path)";
		binmode(IMG_FILE);
		my $bmp_bits;
		my $buf;
		read(IMG_FILE,$buf,30);
		close(IMG_FILE);
		
		my $tmp;	
		($tmp,$bmp_bits) = unpack("a28 S",$buf);		
		
		if($bmp_bits == 8 )
		{
			$img_cmp_index = GetImgCmpIndex("bmp_8bit");		
		}
		elsif($bmp_bits == 16 )
		{
			$img_cmp_index = GetImgCmpIndex("bmp_16bit");						
		}
		elsif($bmp_bits == 24 )
		{
			$img_cmp_index = GetImgCmpIndex("bmp_24bit");					
		}
		elsif($bmp_bits == 32)
		{
			$img_cmp_index = GetImgCmpIndex("bmp_32bit");						
		}
		else
		{
			die "can not support bmp bits [$img_bits] ($img_path)";
		}		
		
	}	
	else
	{
		$img_cmp_index = GetImgCmpIndex($img_type);		
	}
		
	push @params,$img_cmp_index;
	return wantarray?@params:$params[0];
}

sub GetImgCmpDefault{
	my $img_rel_path = shift;
	my $skin_name    = shift;
	my $img_path = $mdu_res_dir;
	my $def_index = -1;
	my $inlcude_ext_flag = 0;
	my @params;
	my @params2;
	
	$img_rel_path =~ s/\\/\//g;	
	#$img_path =~ s/(.*)\/[^\/]*.xrm/$1/g;		
	$img_path = $img_path . $img_rel_path;
	
	$img_path =~s/\#\#/$skin_name/g;
	$img_path = lc($img_path);
	
	if($img_path=~m/(.)+\.bmp$/)
	{
		$inlcude_ext_flag = 1;		
		@params2 = ();
		@params2 = FindAndGetImgCmpIndex($img_path,"bmp");	
		$def_index = $params2[1];		
	} 
	elsif( $img_path=~m/(.)+\.(jpg|jpeg)$/)
	{
		$inlcude_ext_flag = 1;
		$def_index = GetImgCmpIndex("jpg");	
	}
	elsif( $img_path=~m/(.)+\.png$/)
	{
		$inlcude_ext_flag = 1;
		$def_index = GetImgCmpIndex("png");	
	}
	elsif( $img_path=~m/(.)+\.gif$/)
	{
		$inlcude_ext_flag = 1;
		$def_index = GetImgCmpIndex("gif");	
	}
	
	if($inlcude_ext_flag == 1)
	{
		if($def_index == -1)
		{
			die "[$img_path] can not find default compress type\r\n";
		}
		else
		{	
			@params = ();
			push @params,$g_img_cmp_default[$def_index+1];
			push @params,$g_img_cmp_default[$def_index+2];
			push @params,"";
			return wantarray?@params:$params[0];	
		}
	} 
	
	my @img_typs = ("bmp","jpg","png","gif");
	my $img_type;
	my $img_path_abs;
	foreach $img_type (@img_typs) 
	{
		$img_path_abs = "$img_path.$img_type";
		@params2 = ();
		@params2 = FindAndGetImgCmpIndex($img_path_abs,$img_type);		
	
		if($params2[0]==1 && $params2[1] != -1)
		{
			@params = ();
			push @params,$g_img_cmp_default[$params2[1]+1];
			push @params,$g_img_cmp_default[$params2[1]+2];
			push @params,".$img_type";
			return wantarray?@params:$params[0];
		}		
	}
	@params = ();
	push @params,"";
	push @params,"";
	push @params,"";
	return wantarray?@params:$params[0];
	
	
}

#######################################################
## param list:
## 1.image id
## 2.iamge path
## 3.image compress type
## 4.image compress param
#######################################################
sub RES_ADD_IMG{
#	my $id 		= shift;
#	my $path 	= shift;
#	my $cmp_type 	= shift;
#	my $cmp_param 	= shift;

	my $parm_count = scalar(@_);
	if($parm_count<2)
	{
		die "RES_ADD_IMG param number can not be less than 2\n";
	}
	my $id 		= shift;
	my $path 	= shift;
	my $cmp_type;
	my $cmp_param;
	my $img_type;
	if($parm_count == 4)
	{
		$cmp_type 	= shift;
		$cmp_param 	= shift;
	}
	if($parm_count == 3)
	{
		$cmp_type 	= shift;
		$cmp_param 	= "DEFUALT";
	}
	if($parm_count == 2)
	{
		$cmp_type 	= "DEFUALT";
		$cmp_param 	= "DEFUALT";
	}
	my $img_path = $path;
	my $def_cmp_flag = 0;
	if($cmp_type eq "IMG_CMP_DEFAULT" || $cmp_type eq "DEFUALT")
	{
		$def_cmp_flag = 1;
		($cmp_type, $cmp_param,$img_type) = GetImgCmpDefault($path,$g_skin[0]);
		#print "$cmp_type, $cmp_param,$img_type\n";
		if($cmp_type eq "")
		{
			die "not find default compress type\r\n";
		}
		else
		{
			$img_path = "$path$img_type";
		}
	}	

	
	for(my $i = 0; $i < scalar(@g_skin); $i++)
	{
		my $cmp_type_temp = $cmp_type;
		my $cmp_param_temp = $cmp_param;
		my $img_type_temp = $img_type;
		my $img_path_tmp = $img_path;
		if( $i>0 && $def_cmp_flag)
		{
			($cmp_type_temp, $cmp_param_temp,$img_type_temp) = GetImgCmpDefault($path,$g_skin[0]);
			if($cmp_type_temp eq "")
			{
				$cmp_type_temp = $cmp_type;
				$cmp_param_temp = $cmp_param;
				$img_path_tmp  = "$path$img_type_temp";
			}
		}
		my $prd_node = $xrm_doc->getElement("PRODUCT",$i+1);	
		my $imag_node = $prd_node->getElement("IMAG");
		
					
		$img_item = $xrm_doc->createElement("ImagItem");	
		$img_item->attribute("id",$id);
		
		$url_node = $xrm_doc->createElement("URL");
		$url_node->text($img_path_tmp);	
		$img_item->appendChild($url_node);
		
		$type_node = $xrm_doc->createElement("Type");
		$type_node->text("0");
		$img_item->appendChild($type_node);
		
		$cmp_flag_node = $xrm_doc->createElement("CompressedFlag");
		$cmp_flag_node->text($cmp_type_temp);
		$img_item->appendChild($cmp_flag_node);
		
		$cmp_param_node = $xrm_doc->createElement("Quality");
		$cmp_param_node->text($cmp_param_temp);
		$img_item->appendChild($cmp_param_node);
		
		$imag_node->appendChild($img_item);

	}
	
	print "RES_ADD_IMG [$id] complete\n";		
}

#######################################################
## param list:
## 1.image id
## 2.image id num
## 3.iamge path
## 4.image compress type
## 5.image compress param
#######################################################
sub RES_ADD_IMG_RANGE{
#	my $id 		= shift;
#       my $num         = shift;
#	my $path 	= shift;
#	my $cmp_type 	= shift;
#	my $cmp_param 	= shift;

	my $parm_count = scalar(@_);
	if($parm_count!=5)
	{
		die "RES_ADD_IMG_RANGE param number must be 5.\n";
	}
	my $id 		= shift;
	my $num         = shift;
	my $path 	= shift;
	my $cmp_type 	= shift;
	my $cmp_param 	= shift;	
	
	if( !($path =~ m/([^\.]+)_1\.([^\.]+)/))
	{
		die "RES_ADD_IMG_RANGE: the path [$path] does not match xxx_1.yyy\n";
	}
	
	my $path_base = $1;
	my $ext  = $2;
	
	for(my $i=1;$i<=$num;$i++)
	{
		my $id_temp = $id."_".$i;	
		my $path_temp = $path_base."_".$i.".".$ext;	
				
		RES_ADD_IMG($id_temp,$path_temp,$cmp_type,$cmp_param);	
	}
	
	print "RES_ADD_IMG_RANGE [$id] complete\n";		
}

#######################################################
## param list:
## 1.anim id
## 2.anim path
## 3.anim compress type
## 4.anim compress param
#######################################################
sub RES_ADD_ANIM{

#	my $id 		= shift;
#	my $path 	= shift;
#	my $cmp_type 	= shift;
#	my $cmp_param 	= shift;
#       my $loc_key_color   = shift;
	
	my $parm_count = scalar(@_);
	if($parm_count<2)
	{
		die "RES_ADD_ANIM param number can not be less than 2\n";
	}
	my $id 		= shift;
	my $path 	= shift;
	
	my $cmp_type;
	my $cmp_param;
	my $loc_key_color;
	if($parm_count >= 5)
	{
		$cmp_type 	= shift;
		$cmp_param 	= shift;
		$key_color      = shift;
	}
	elsif($parm_count >= 4)
	{
		$cmp_type 	= shift;
		$cmp_param 	= shift;
		$loc_key_color  = $key_color;
	}
	elsif($parm_count >= 3)
	{
		$cmp_type 	= shift;
		$cmp_param 	= "DEFUALT";
		$loc_key_color  = $key_color;
	}
	else
	{
		$cmp_type 	= "DEFUALT";
		$cmp_param 	= "DEFUALT";
		$loc_key_color  = $key_color;
	}
	
	for(my $i = 0; $i < scalar(@g_skin); $i++)
	{
		my $prd_node = $xrm_doc->getElement("PRODUCT",$i+1);	
		my $anim_node = $prd_node->getElement("ANIM");
		
		$anim_item = $xrm_doc->createElement("AnimItem");
		$anim_item->attribute("id",$id);
		
		$url_node = $xrm_doc->createElement("URL");
		$url_node->text($path);	
		$anim_item->appendChild($url_node);
		
		$type_node = $xrm_doc->createElement("Type");
		$type_node->text("0");
		$anim_item->appendChild($type_node);
		
		$cmp_flag_node = $xrm_doc->createElement("CompressedFlag");
		$cmp_flag_node->text($cmp_type);
		$anim_item->appendChild($cmp_flag_node);
		
		$cmp_param_node = $xrm_doc->createElement("Quality");
		$cmp_param_node->text($cmp_param);
		$anim_item->appendChild($cmp_param_node);
		
		$key_color_node = $xrm_doc->createElement("KeyColorRGB");
		$key_color_node->text($key_color);
		$anim_item->appendChild($key_color_node);
		
		$anim_node->appendChild($anim_item);	
	}
	
	print "RES_ADD_ANIM [$id] complete\n";
}

#######################################################
## param list:
## 1.anim id
## 2.id num
## 3.anim path
## 4.anim compress type
## 5.anim compress param
## 6.key color
#######################################################
sub RES_ADD_ANIM_RANGE{
#	my $id 		= shift;
#	my $num 	= shift;
#	my $path 	= shift;
#	my $cmp_type 	= shift;
#	my $cmp_param 	= shift;
#       my $loc_key_color   = shift;
	
	my $parm_count = scalar(@_);
	if($parm_count!=6)
	{
		die "RES_ADD_ANIM_RANGE param number must be 6\n";
	}
	my $id 		  = shift;
	my $num 	  = shift;
	my $path 	  = shift;
	my $cmp_type      = shift;
	my $cmp_param     = shift;
	my $loc_key_color = shift;
	
	if( !($path =~ m/([^\.]+)_1$/))
	{
		die "RES_ADD_ANIM_RANGE: the path [$path] does not match xxx_1.yyy\n";
	}
	my $path_base = $1;
	
	for(my $i=1;$i<=$num;$i++)
	{
		my $id_temp = $id."_".$i;	
		my $path_temp = $path_base."_".$i;	
						
		RES_ADD_ANIM($id_temp,$path_temp,$cmp_type,$cmp_param,$loc_key_color);	
	}
	
	print "RES_ADD_ANIM_RANGE [$id] complete\n";
	
}

#######################################################
## param list:
## 1.ring id
## 2.ring path
#######################################################
sub RES_ADD_RING{

	my $id 	 = shift;
	my $path = shift;
	
	for(my $i = 0; $i < scalar(@g_skin); $i++)
	{
		my $prd_node = $xrm_doc->getElement("PRODUCT",$i+1);		
		my $ring_node = $prd_node->getElement("RING");
		
		$ring_item = $xrm_doc->createElement("RingItem");
		$ring_item->attribute("id",$id);
		
		$url_node = $xrm_doc->createElement("URL");
		$url_node->text($path);	
		$ring_item->appendChild($url_node);
		
		$type_node = $xrm_doc->createElement("Type");
		$type_node->text($cmp_type);
		$ring_item->appendChild($type_node);
		
		$ring_node->appendChild($ring_item);
	}
	
	print "RES_ADD_RING [$id] complete\n";
}

#######################################################
## param list:
## 1.ring id
## 2.id num
## 3.ring path
#######################################################
sub RES_ADD_RING_RANGE{

	my $id 	 = shift;
	my $num  = shift;
	my $path = shift;
	
	if( !($path =~ m/([^\.]+)_1\.([^\.]+)/))
	{
		die "RES_ADD_RING_RANGE: the path [$path] does not match xxx_1.yyy\n";
	}
	my $path_base = $1;
	my $ext  = $2;
	
	for(my $i=1;$i<=$num;$i++)
	{
		my $id_temp = $id."_".$i;	
		my $path_temp = $path_base."_".$i.".".$ext;	
				
		RES_ADD_RING($id_temp,$path_temp);	
	}
	
	print "RES_ADD_RING_RANGE [$id] complete\n";
	
}

#######################################################
## param list:
## 1.Font id
## 2.Arm Font path
## 3.Win Font path
#######################################################
sub RES_ADD_FONT{

	my $id		= shift;
	my $arm_path 	= shift;
	my $win_path 	= shift;
	
	for(my $i = 0; $i < scalar(@g_skin); $i++)
	{		
		my $prd_node = $xrm_doc->getElement("PRODUCT",$i + 1);
		my $font_node = $prd_node->getElement("FONT");
		
		$font_item = $xrm_doc->createElement("FontItem");
		$font_item->attribute("id",$id);
		
		$type_node = $xrm_doc->createElement("Type");
		$type_node->text("0");
		$font_item->appendChild($type_node);
		
		$database_node = $xrm_doc->createElement("DataBaseItem");
		
		$arm_url_node = $xrm_doc->createElement("URL");
		$arm_url_node->text($arm_path);	
		$database_node->appendChild($arm_url_node);
		
		$win_url_node = $xrm_doc->createElement("URL_WIN");
		$win_url_node->text($win_path);	
		$database_node->appendChild($win_url_node);
		
		$font_item->appendChild($database_node);	
			
		$font_node->appendChild($font_item);
	}
	
	print "RES_ADD_FONT [$id] complete\n";
}

#######################################################
## param list:
## 1.Font id
## 2.id num
## 3.Arm Font path
## 4.Win Font path
#######################################################
sub RES_ADD_FONT_RANGE{

	my $id		= shift;
	my $num         = shift;
	my $arm_path 	= shift;
	my $win_path 	= shift;
	
	if( !($arm_path =~ m/([^\.]+)_1\.([^\.]+)/))
	{
		die "RES_ADD_FONT_RANGE: the arm path [$path] does not match xxx_1.yyy\n";
	}
	my $arm_path_base = $1;
	my $arm_ext  = $2;
	
	my $win_path_base = $win_path;
	my $win_ext = "";
	
	if(!$win_path)
	{
		if( !($win_path =~ m/([^\.]+)_1\.([^\.]+)/))
		{
			die "RES_ADD_FONT_RANGE: the win path [$path] does not match xxx_1.yyy\n";
		}
		$win_path_base = $1;
		$win_ext  = $2;
	}
	
	
	for(my $i=1;$i<=$num;$i++)
	{
		my $id_temp = $id."_".$i;				
		my $arm_path_temp = $arm_path_base."_".$i.".".$arm_ext;
		my $win_path_temp = $win_path;
		if(!$win_path_temp)
		{
			$win_path_temp = $win_path_base."_".$i.".".$win_ext;
		}
				
		RES_ADD_FONT($id_temp,$arm_path_temp,$win_path_temp);	
	}
	
	print "RES_ADD_FONT_RANGE [$id] complete\n";
	
}

#######################################################
## param list:
## 1.data id
## 2.data path
##
## 2009-3-10 modifed by hongliang
##           support data as theme resource.
#######################################################
sub RES_ADD_DATA{

	my $id 	 = shift;
	my $path = shift;
	
	for(my $i = 0; $i < scalar(@g_skin); $i++)
	{	
		my $prd_node = $xrm_doc->getElement("PRODUCT",$i+1);
		my $data_node = $prd_node->getElement("DATA");
		
		$data_item = $xrm_doc->createElement("DataItem");
		$data_item->attribute("id",$id);
		
		$url_node = $xrm_doc->createElement("URL");
		$url_node->text($path);	
		$data_item->appendChild($url_node);
		
		$type_node = $xrm_doc->createElement("Type");
		$type_node->text("0");
		$data_item->appendChild($type_node);
		
		$data_node->appendChild($data_item);
	}
	
	print "RES_ADD_DATA [$id] complete\n";
}

#######################################################
## param list:
## 1.data id
## 2.id num
## 3.data path
#######################################################
sub RES_ADD_DATA_RANGE{

	my $id 	 = shift;
	my $num  = shift;
	my $path = shift;
	
	if( !($path =~ m/([^\.]+)_1\.([^\.]+)/))
	{
		die "RES_ADD_DATA_RANGE: the path [$path] does not match xxx_1.yyy\n";
	}
	my $path_base = $1;
	my $ext  = $2;
	
	for(my $i=1;$i<=$num;$i++)
	{
		my $id_temp = $id."_".$i;	
		my $path_temp = $path_base."_".$i.".".$ext;
			
		RES_ADD_DATA($id_temp,$path_temp);	
	}
	print "RES_ADD_DATA_RANGE [$id] complete\n";	
}

#######################################################
## param list:
## 1.string id
## 2.default content
#######################################################
sub RES_ADD_STRING{
	my $id 	 = shift;
	my $default_content = shift;	
	
	$default_content =~ s/&/&amp;/g;
	$default_content =~ s/</&lt;/g;
	$default_content =~ s/>/&gt;/g;
	$default_content =~ s/'/&apos;/g;
	$default_content =~ s/"/&quot;/g;
	
	for(my $i = 0; $i < scalar(@g_skin); $i++)
	{	
		my $prd_node = $xrm_doc->getElement("PRODUCT",$i+1);
		my $text_node = $prd_node->getElement("TEXT");
		my $xrm_strings_node = $text_node->getElement("Strings");
		
		my $string_node = $xrm_doc->createElement("String");
		$string_node->attribute("id",$id);
		$string_node->attribute("default",$default_content);
		$xrm_strings_node->appendChild($string_node);
	}

=head	
	my $find_flag = 0;
	
	$text_ws->write_string($str_row_count, 0, $id );
	
	foreach my $row ($min_row+1 .. $max_row)
	{
		my $str_id = $str_table_ws->{Cells}[$row][$min_col]->{Val};
		my $maxlen = $str_table_ws->{Cells}[$row][$maxlen_index]->{Val};
		if($str_id eq $id)
		{
			$find_flag = 1;
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
								die "[TEXT] [$str_id] [row:$row] [col:$col] string length ($str_len) is larger than the max length($maxlen)\n";
							}
							
							$text_ws->write_utf16be_string($str_row_count, $col_cout, $str);
						}
						else
						{
							my $str_len = rindex $str."\$","\$";
							if($str_len > $maxlen)
							{
								die "[TEXT] [$str_id] [row:$row] [col:$col] string length ($str_len) is larger than the max length($maxlen)\n";
							}
							$text_ws->write_string($str_row_count, $col_cout, $str);
						}
					}
					else
					{
						my $str_len = rindex $default_content."\$","\$";
						if($str_len > $maxlen)
						{
							die "[TEXT] [$str_id] default string [$default_content] length ($str_len) is larger than the max length($maxlen)\n";
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
						die "[TEXT] [$str_id] default string [$default_content] length ($str_len) is larger than the max length($maxlen)\n";
					}
					
					#print $str_row_count." ".$col_cout." ".$default_content."\n";
					$text_ws->write_string($str_row_count, $col_cout, $default_content );
				}
				
				$col_cout++;
			}
			last;
		}
	}
	
	if($find_flag == 0)
	{
		for(my $i=1;$i <= scalar(@g_lang);$i++)		
		{	
			#print $str_row_count." ".$i." ".$default_content."\n";		
			$text_ws->write_string($str_row_count, $i, $default_content );						
		}
	}
	
	$str_row_count++;
=cut	
	print "RES_ADD_STRING [$id] complete\n";
}

#######################################################
## param list:
## 1.string id
## 2.id num
## 2.default content
#######################################################
sub RES_ADD_STRING_RANGE{
	my $id 	 = shift;
	my $num  = shift;
	my $default_content = shift;
		
	for(my $i=1;$i<=$num;$i++)
	{	
		my $id_temp = $id."_".$i;			
		RES_ADD_STRING($id_temp,$default_content);	
	}
	
	print "RES_ADD_STRING [$id] complete\n";
}


#######################################################
## param list:
## 1.color id
## 2.color value
##
## 2009-6-25 created by hongliang
#######################################################
sub RES_ADD_COLOR{

	my $id 	 = shift;
	my $color = shift;
	
	for(my $i = 0; $i < scalar(@g_skin); $i++)
	{	
		my $prd_node = $xrm_doc->getElement("PRODUCT",$i+1);
		my $color_node = $prd_node->getElement("COLOR");
		
		my $color_item = $xrm_doc->createElement("ColorItem");
		$color_item->attribute("id",$id);
		$color_item->attribute("color",$color);		
		
		$color_node->appendChild($color_item);
	}
	
	print "RES_ADD_COLOR [$id] complete\n";
}

#######################################################
## param list:
## 1.color id
## 2.id num
## 3.color value
#######################################################
sub RES_ADD_COLOR_RANGE{

	my $id 	 = shift;
	my $num  = shift;
	my $color = shift;
	for(my $i=1;$i<=$num;$i++)
	{	
		my $id_temp = $id."_".$i;			
		RES_ADD_COLOR($id_temp,$color);	
	}
	
	print "RES_ADD_COLOR_RANGE [$id] complete\n";
}

#######################################################
## param list:
## 1.rect id
## 2.left
## 3.top
## 4.right
## 5.bottom
##
## 2009-6-25 created by hongliang
#######################################################
sub RES_ADD_RECT{

	my $id 	 = shift;
	my $rect_left = shift;
	my $rect_top = shift;
	my $rect_right = shift;
	my $rect_bottom = shift;
	
	for(my $i = 0; $i < scalar(@g_skin); $i++)
	{	
		my $prd_node = $xrm_doc->getElement("PRODUCT",$i+1);
		my $rect_node = $prd_node->getElement("RECT");
		
		my $rect_item = $xrm_doc->createElement("RectItem");
		$rect_item->attribute("id",$id);
		$rect_item->attribute("left",$rect_left);
		$rect_item->attribute("top",$rect_top);
		$rect_item->attribute("right",$rect_right);
		$rect_item->attribute("bottom",$rect_bottom);		
		
		$rect_node->appendChild($rect_item);
	}
	
	print "RES_ADD_RECT [$id] complete\n";
}

#######################################################
## param list:
## 1.rect id
## 2.id num
## 3.left
## 4.top
## 5.right
## 6.bottom
#######################################################
sub RES_ADD_RECT_RANGE{

	my $id 	 = shift;
	my $num  = shift;
	my $rect_left = shift;
	my $rect_top = shift;
	my $rect_right = shift;
	my $rect_bottom = shift;
	for(my $i=1;$i<=$num;$i++)
	{	
		my $id_temp = $id."_".$i;			
		RES_ADD_RECT($id_temp,$rect_left,$rect_top,$rect_right,$rect_bottom);	
	}
	
	print "RES_ADD_RECT_RANGE [$id] complete\n";
}

#######################################################
## param list:
## 1.resource id
## 2.decsiption
##
## 2010-08-30 created by hongliang
#######################################################
sub RES_ADD_IMG_TAG{

	my $id 	 = shift;
	my $desc = shift;
#	my $flag = shift	
		
	my $tag_node = $xrm_doc->getElement("RESTAG",1);		
	my $tag_item = $xrm_doc->createElement("TagItem");
	$tag_item->attribute("id",$id);
	$tag_item->attribute("desc",$desc);
	$tag_item->attribute("res_type","MMI_RT_IMAGE");
	$tag_item->attribute("flag","RES_TAG_REPLACE");		
	$tag_node->appendChild($tag_item);
		
	print "RES_ADD_IMG_TAG [$id] complete\n";
}

#######################################################
## param list:
## 1.resource id
## 2.id num
## 3.decsiption
##
## 2010-10-19 created by hongliang
#######################################################
sub RES_ADD_IMG_TAG_RANGE{

	my $id 	 = shift;
	my $num  = shift;
	my $desc = shift;
#	my $flag = shift	
		
	for(my $i=1;$i<=$num;$i++)
	{	
		my $id_temp = $id."_".$i;		
		my $desc_temp = $desc."_".$i;	
		RES_ADD_IMG_TAG($id_temp,$desc_temp);	
	}
		
	print "RES_ADD_IMG_TAG_RANGE [$id] complete\n";
}

#######################################################
## param list:
## 1.resource id
## 2.decsiption
##
## 2010-08-30 created by hongliang
#######################################################
sub RES_ADD_ANIM_TAG{

	my $id 	 = shift;
	my $desc = shift;
#	my $flag = shift	
		
	my $tag_node = $xrm_doc->getElement("RESTAG",1);		
	my $tag_item = $xrm_doc->createElement("TagItem");
	$tag_item->attribute("id",$id);
	$tag_item->attribute("desc",$desc);
	$tag_item->attribute("res_type","MMI_RT_ANIM");
	$tag_item->attribute("flag","RES_TAG_REPLACE");			
	$tag_node->appendChild($tag_item);
		
	print "RES_ADD_ANIM_TAG [$id] complete\n";
}

#######################################################
## param list:
## 1.resource id
## 2.id num
## 3.decsiption
##
## 2010-10-19 created by hongliang
#######################################################
sub RES_ADD_ANIM_TAG_RANGE{

	my $id 	 = shift;
	my $num  = shift;
	my $desc = shift;
#	my $flag = shift	
		
	for(my $i=1;$i<=$num;$i++)
	{	
		my $id_temp = $id."_".$i;		
		my $desc_temp = $desc."_".$i;	
		RES_ADD_ANIM_TAG($id_temp,$desc_temp);	
	}
		
	print "RES_ADD_ANIM_TAG_RANGE [$id] complete\n";
}
#######################################################
## param list:
## 1.resource id
## 2.decsiption
##
## 2010-08-30 created by hongliang
#######################################################
sub RES_ADD_RING_TAG{

	my $id 	 = shift;
	my $desc = shift;
#	my $flag = shift	
		
	my $tag_node = $xrm_doc->getElement("RESTAG",1);		
	my $tag_item = $xrm_doc->createElement("TagItem");
	$tag_item->attribute("id",$id);
	$tag_item->attribute("desc",$desc);
	$tag_item->attribute("res_type","MMI_RT_RING");
	$tag_item->attribute("flag","RES_TAG_REPLACE");	
	$tag_node->appendChild($tag_item);
		
	print "RES_ADD_RING_TAG [$id] complete\n";
}


#######################################################
## param list:
## 1.resource id
## 2.id num
## 3.decsiption
##
## 2010-10-19 created by hongliang
#######################################################
sub RES_ADD_RING_TAG_RANGE{

	my $id 	 = shift;
	my $num  = shift;
	my $desc = shift;
#	my $flag = shift	
		
	for(my $i=1;$i<=$num;$i++)
	{	
		my $id_temp = $id."_".$i;		
		my $desc_temp = $desc."_".$i;		
		RES_ADD_RING_TAG($id_temp,$desc_temp);	
	}
		
	print "RES_ADD_RING_TAG_RANGE [$id] complete\n";
}

#######################################################
## param list:
## 1.resource id
## 2.decsiption
##
## 2010-08-30 created by hongliang
#######################################################
sub RES_ADD_COLOR_TAG{

	my $id 	 = shift;
	my $desc = shift;
#	my $flag = shift	
		
	my $tag_node = $xrm_doc->getElement("RESTAG",1);		
	my $tag_item = $xrm_doc->createElement("TagItem");
	$tag_item->attribute("id",$id);
	$tag_item->attribute("desc",$desc);
	$tag_item->attribute("res_type","MMI_RT_COLOR");
	$tag_item->attribute("flag","RES_TAG_REPLACE");	
	$tag_node->appendChild($tag_item);
		
	print "RES_ADD_COLOR_TAG [$id] complete\n";
}

#######################################################
## param list:
## 1.resource id
## 2.id num
## 3.decsiption
##
## 2010-10-19 created by hongliang
#######################################################
sub RES_ADD_COLOR_TAG_RANGE{

	my $id 	 = shift;
	my $num  = shift;
	my $desc = shift;
#	my $flag = shift	
		
	for(my $i=1;$i<=$num;$i++)
	{	
		my $id_temp = $id."_".$i;		
		my $desc_temp = $desc."_".$i;		
		RES_ADD_COLOR_TAG($id_temp,$desc_temp);	
	}
		
	print "RES_ADD_COLOR_TAG_RANGE [$id] complete\n";
}

#######################################################
## param list:
## 1.resource id
## 2.decsiption
##
## 2010-08-30 created by hongliang
#######################################################
sub RES_ADD_RECT_TAG{

	my $id 	 = shift;
	my $desc = shift;
#	my $flag = shift	
		
	my $tag_node = $xrm_doc->getElement("RESTAG",1);		
	my $tag_item = $xrm_doc->createElement("TagItem");
	$tag_item->attribute("id",$id);
	$tag_item->attribute("desc",$desc);
	$tag_item->attribute("res_type","MMI_RT_RECT");
	$tag_item->attribute("flag","RES_TAG_REPLACE");		
	$tag_node->appendChild($tag_item);
		
	print "RES_ADD_RECT_TAG [$id] complete\n";
}

#######################################################
## param list:
## 1.resource id
## 2.id num
## 3.decsiption
##
## 2010-10-19 created by hongliang
#######################################################
sub RES_ADD_RECT_TAG_RANGE{

	my $id 	 = shift;
	my $num  = shift;
	my $desc = shift;
#	my $flag = shift	
		
	for(my $i=1;$i<=$num;$i++)
	{	
		my $id_temp = $id."_".$i;		
		my $desc_temp = $desc."_".$i;		
		RES_ADD_RECT_TAG($id_temp,$desc_temp);	
	}
		
	print "RES_ADD_RECT_TAG_RANGE [$id] complete\n";
}

#######################################################
## param list:
## 1.resource id
## 2.decsiption
##
## 2010-08-30 created by hongliang
#######################################################
sub RES_ADD_FONT_TAG{

	my $id 	 = shift;
	my $desc = shift;
#	my $flag = shift	
		
	my $tag_node = $xrm_doc->getElement("RESTAG",1);		
	my $tag_item = $xrm_doc->createElement("TagItem");
	$tag_item->attribute("id",$id);
	$tag_item->attribute("desc",$desc);
	$tag_item->attribute("res_type","MMI_RT_FONT");
	$tag_item->attribute("flag","RES_TAG_REPLACE");	
	$tag_node->appendChild($tag_item);
		
	print "RES_ADD_FONT_TAG [$id] complete\n";
}

#######################################################
## param list:
## 1.resource id
## 2.id num
## 3.decsiption
##
## 2010-10-19 created by hongliang
#######################################################
sub RES_ADD_FONT_TAG_RANGE{

	my $id 	 = shift;
	my $num  = shift;
	my $desc = shift;
#	my $flag = shift	
		
	for(my $i=1;$i<=$num;$i++)
	{	
		my $id_temp = $id."_".$i;		
		my $desc_temp = $desc."_".$i;		
		RES_ADD_FONT_TAG($id_temp,$desc_temp);	
	}
		
	print "RES_ADD_FONT_TAG_RANGE [$id] complete\n";
}

#######################################################
## param list:
## 1.resource id
## 2.decsiption
##
## 2010-08-30 created by hongliang
#######################################################
sub RES_ADD_DATA_TAG{

	my $id 	 = shift;
	my $desc = shift;
#	my $flag = shift	
		
	my $tag_node = $xrm_doc->getElement("RESTAG",1);		
	my $tag_item = $xrm_doc->createElement("TagItem");
	$tag_item->attribute("id",$id);
	$tag_item->attribute("desc",$desc);
	$tag_item->attribute("res_type","MMI_RT_DATA");
	$tag_item->attribute("flag","RES_TAG_REPLACE");		
	$tag_node->appendChild($tag_item);
		
	print "RES_ADD_DATA_TAG [$id] complete\n";
}

#######################################################
## param list:
## 1.resource id
## 2.id num
## 3.decsiption
##
## 2010-10-19 created by hongliang
#######################################################
sub RES_ADD_DATA_TAG_RANGE{

	my $id 	 = shift;
	my $num  = shift;
	my $desc = shift;
#	my $flag = shift	
		
	for(my $i=1;$i<=$num;$i++)
	{	
		my $id_temp = $id."_".$i;		
		my $desc_temp = $desc."_".$i;		
		RES_ADD_DATA_TAG($id_temp,$desc_temp);	
	}
		
	print "RES_ADD_DATA_TAG_RANGE [$id] complete\n";
}

#######################################################
## test
=head
START_GEN_XRM("D:\\ResGen\\common_mdu_def.h","0x000000");

RES_ADD_IMG("IMG_ID_1","\\MMI_RES_##\\IMAG\\1.bmp","0","75");
RES_ADD_IMG("IMG_ID_2","\\MMI_RES_##\\IMAG\\2.bmp","0","75");
RES_ADD_IMG("IMG_ID_3","\\MMI_RES_##\\IMAG\\3.bmp","0","75");

RES_ADD_ANIM("ANIM_ID_1","\\MMI_RES_##\\ANIM\\1","0","75");
RES_ADD_ANIM("ANIM_ID_2","\\MMI_RES_##\\ANIM\\2","0","75");
RES_ADD_ANIM("ANIM_ID_3","\\MMI_RES_##\\ANIM\\3","0","75");

RES_ADD_RING("RING_ID_1","\\MMI_RES_##\\RING\\1.mid");
RES_ADD_RING("RING_ID_2","\\MMI_RES_##\\RING\\2.mid");
RES_ADD_RING("RING_ID_3","\\MMI_RES_##\\RING\\3.mid");

RES_ADD_FONT("FONT_ID_1","\\MMI_RES_##\\FONT\\arm1.lib","\\MMI_RES_##\\FONT\\win1.lib");
RES_ADD_FONT("FONT_ID_2","\\MMI_RES_##\\FONT\\arm2.lib","\\MMI_RES_##\\FONT\\win2.lib");
RES_ADD_FONT("FONT_ID_3","\\MMI_RES_##\\FONT\\arm3.lib","\\MMI_RES_##\\FONT\\win2.lib");

RES_ADD_DATA("DATA_ID_1","\\MMI_RES_##\\DATA\\1.dat");
RES_ADD_DATA("DATA_ID_2","\\MMI_RES_##\\DATA\\2.dat");
RES_ADD_DATA("DATA_ID_3","\\MMI_RES_##\\DATA\\3.dat");

END_GEN_XRM();
=cut

1;


