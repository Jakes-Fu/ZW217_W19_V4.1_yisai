#!/usr/bin/perl

#########################################################################
## 
## File Name:   xrp_temp.pl
## Author:      Hongliang.xin
## Description: provide functions to generate mmi_res.xrp file.
## Usage:       
## 
## Modification History
## 2010-09-08   hongliang.xin  
##              Add RES_ADD_LOGO function.
## 2010-06-23   hongliang.xin  
##              provide new function "RES_SET_MAX_RING_SIZE"
#########################################################################

BEGIN{push @INC,'./make/perl_script/perl_pm'};
use XML::Mini::Document;

my $xrp_path;
my $xrp_doc;
my $xrp_root_node;
my $res_dir;

my @g_lang;
my @g_skin;

#######################################################
## param list:
## 1.image id
## 2.iamge path
## 3.image compress type
## 4.image compress param
#######################################################
sub START_GEN_XRP{
	
	$xrp_path = shift;
	$res_dir  = shift;
	$xrp_path =~ s/^(.*)_prj_def.h$/$1.xrp/g;

	$xrp_doc = XML::Mini::Document->new();
	my $vroot = $xrp_doc->getRoot();
	my $xmlHeader = $vroot->header("xml");
	$xmlHeader->attribute("version", "1.0");
	$xmlHeader->attribute("encoding", "UTF-8");
	$xrp_root_node = $vroot->createChild("XRP");
	$xrp_root_node->attribute("version","x_sci_xrp_v3_0");
	
	my $res_dir_node = $xrp_doc->createElement("RESDIR");
	$res_dir =~ s/\//\\/g;
	$res_dir_node->attribute("url", "$res_dir");
	$xrp_root_node->appendChild($res_dir_node);
	
	my $lang_node = $xrp_doc->createElement("LANG");
	$xrp_root_node->appendChild($lang_node);
	
	my $logo_node = $xrp_doc->createElement("LOGO");
	$xrp_root_node->appendChild($logo_node);
	
	my $skin_node = $xrp_doc->createElement("SKIN");
	my $skin_item = $xrp_doc->createElement("SkinItem");
	$skin_item->attribute("name","DEFAULT");
	$skin_node->appendChild($skin_item);
	$xrp_root_node->appendChild($skin_node);
	
	push @g_skin,"DEFAULT";
	
	print "START_GEN_XRP [$xrp_path] complete\n";
}

#######################################################
## param list:
## 1.image id
## 2.iamge path
## 3.image compress type
## 4.image compress param
#######################################################
sub END_GEN_XRP{
	$xrp_path =~ s/\\/\//g;
	open(XRP_FILE, ">$xrp_path") or die "Can not open $xrp_path" ;
	print XRP_FILE $xrp_doc->toString();
	close XRP_FILE;
	
#	print $xrp_doc->toString();

	print "END_GEN_XRP [$xrp_path] complete\n";
	
}


#######################################################
## param list:
## 1.language name
#######################################################
sub RES_ADD_LANGUAGE{

	my $lang_id = shift;
	my $lang_name = shift;		
	push @g_lang,$lang_name;
	
	my $lang_node = $xrp_doc->getElement("LANG", 1);
	my $lang_item = $xrp_doc->createElement("LangItem");
	$lang_item->attribute("id",$lang_id);
	$lang_item->attribute("name",$lang_name);
	$lang_node->appendChild($lang_item);
	
	print "RES_ADD_LANGUAGE [$lang_name] complete\n";
}
#######################################################
## param list:
## 1.skin name
#######################################################
sub RES_ADD_SKIN{
	my $skin_name = shift;
	my $skin_ver  = shift;
	my $name_id   = shift;
	push @g_skin,$skin_name;
	
	my $skin_node = $xrp_doc->getElement("SKIN",1);
	my $skin_item = $xrp_doc->createElement("SkinItem");
	$skin_item->attribute("name",$skin_name);
	$skin_item->attribute("version",$skin_ver);
	$skin_item->attribute("id",$name_id);
	$skin_node->appendChild($skin_item);
	
	print "RES_ADD_SKIN [$skin_name] complete\n";
}

#######################################################
## param list:
## 1.module id
## 2.module path
#######################################################
sub RES_ADD_MODULE{
	my $module_id = shift;
	my $module_path = shift;
	$module_path =~ s/(.*)_mdu_def.h$/$1.xrm/g;
	my $xrm_node = $xrp_doc->createElement("XRM");
	$xrm_node->attribute("id",$module_id);
	$xrm_node->attribute("url",$module_path);
	
	$xrp_root_node->appendChild($xrm_node);
	
	print "RES_ADD_MODULE [$module_id] complete\n";

}
#######################################################
## param list:
## 1.version
#######################################################
sub RES_SET_VERSION{
	my $res_ver = shift;
	$xrp_root_node->attribute("res-version",$res_ver);	
	
	print "RES_SET_VERSION [$res_ver] complete\n";
}

#######################################################
## param list:
## 1.max module number,32,64...
#######################################################
sub RES_SET_MAX_MODULE_NUM{
	my $max_module = shift;
	$xrp_root_node->attribute("max-module",$max_module);	
	print "RES_SET_MAX_MODULE_NUM [$max_module] complete\n";
}

#######################################################
## param list:
## 1.compress type
## 2.compress param
#######################################################
sub RES_SET_IMG_GLOBAL_COMPRESS_TYPE{
	my $img_global_compress_type = shift;
	my $img_global_compress_param = shift;
	$xrp_root_node->attribute("img-global-compress-param",$img_global_compress_param);
	$xrp_root_node->attribute("img-global-compress-type",$img_global_compress_type);
	
	print "RES_SET_IMG_GLOBAL_COMPRESS_TYPE [$img_global_compress_type,$img_global_compress_param] complete\n";	
}
#######################################################
## param list:
## 1.compress type
## 2.compress param
#######################################################
sub RES_SET_IMG_DEFAULT_COMPRESS_TYPE{
	my $img_default_compress_type = shift;
	my $img_default_compress_param = shift;
	$xrp_root_node->attribute("img-default-compress-param",$img_default_compress_param);
	$xrp_root_node->attribute("img-default-compress-type",$img_default_compress_type);
	
	print "RES_SET_IMG_DEFAULT_COMPRESS_TYPE [$img_default_compress_type,$img_default_compress_param] complete\n";	
}
#######################################################
## param list:
## 1.compress type
## 2.compress param
#######################################################
sub RES_SET_ANIM_GLOBAL_COMPRESS_TYPE{
	my $anim_global_compress_type = shift;
	my $anim_global_compress_param = shift;
	$xrp_root_node->attribute("anim-global-compress-param",$anim_global_compress_param);
	$xrp_root_node->attribute("anim-global-compress-type",$anim_global_compress_type);	
	
	print "RES_SET_ANIM_GLOBAL_COMPRESS_TYPE [$anim_global_compress_type,$anim_global_compress_param] complete\n";
}
#######################################################
## param list:
## 1.compress type
## 2.compress param
#######################################################
sub RES_SET_ANIM_DEFAULT_COMPRESS_TYPE{
	my $anim_default_compress_type = shift;
	my $anim_default_compress_param = shift;
	$xrp_root_node->attribute("anim-default-compress-param",$anim_default_compress_param);
	$xrp_root_node->attribute("anim-default-compress-type",$anim_default_compress_type);	
	
	print "RES_SET_ANIM_DEFAULT_COMPRESS_TYPE [$anim_default_compress_type,$anim_default_compress_param ] complete\n";
}
#######################################################
## param list:
## 1.anim key color
#######################################################
sub RES_SET_ANIM_GLOBAL_KEY_COLOR{
	my $key_color = shift;
	$xrp_root_node->attribute("key-color",shift);
	print "RES_SET_ANIM_GLOBAL_KEY_COLOR [$key_color] complete\n";
}
#######################################################
## param list:
## 1.jpeg comress rate ...75,85,95....
#######################################################
sub RES_SET_GLOBAL_JPEG_COMPRESS_RATE{
	my $jpeg_cmp_rate = shift;
	$xrp_root_node->attribute("jpeg-compress-rate",$jpeg_cmp_rate);
	print "RES_SET_GLOBAL_JPEG_COMPRESS_RATE [$jpeg_cmp_rate] complete\n";
}

#######################################################
## param list:
## 1.max ring size, if not set,default value is 50*1024
#######################################################
sub RES_SET_MAX_RING_SIZE{
	my $max_ring_size = shift;
	$xrp_root_node->attribute("max-ring-size",$max_ring_size);	
	print "RES_SET_MAX_RING_SIZE [$max_ring_size] complete\n";
}

#######################################################
## param list:
## 1.image id
## 2.iamge path
## 3.image compress type
## 4.image compress param
#######################################################
sub RES_ADD_LOGO{
#	my $id 		= shift;
#	my $path 	= shift;
#	my $cmp_type 	= shift;
#	my $cmp_param 	= shift;

	my $parm_count = scalar(@_);
	if($parm_count<4)
	{
		die "RES_ADD_LOGO param number can not be less than 2\n";
	}
	my $id 		= shift;
	my $path 	= shift;
	my $cmp_type    = shift;
	my $cmp_param   = shift;	
	
	my $logo_node = $xrp_doc->getElement("LOGO", 1);	
				
	$img_item = $xrp_doc->createElement("ImagItem");	
	$img_item->attribute("id",$id);
	
	$url_node = $xrp_doc->createElement("URL");
	$url_node->text($path);	
	$img_item->appendChild($url_node);
	
	$type_node = $xrp_doc->createElement("Type");
	$type_node->text("0");
	$img_item->appendChild($type_node);
	
	$cmp_flag_node = $xrp_doc->createElement("CompressedFlag");
	$cmp_flag_node->text($cmp_type);
	$img_item->appendChild($cmp_flag_node);
	
	$cmp_param_node = $xrp_doc->createElement("Quality");
	$cmp_param_node->text($cmp_param);
	$img_item->appendChild($cmp_param_node);
	
	$logo_node->appendChild($img_item);	
	
	print "RES_ADD_LOGO [$id] complete\n";		
}


#######################################################
## test

=head
START_GEN_XRP("D:\\ResGen\\mmi_res_prj_def.h");

RES_ADD_LANGUAGE("English");
RES_ADD_LANGUAGE("SimpleChiese");
RES_ADD_LANGUAGE("TraditionalChinese");

RES_SET_VERSION("1.0.0");
RES_SET_MAX_MODULE_NUM("64");
RES_SET_IMG_GLOBAL_COMPRESS_TYPE("0", "75");
RES_SET_ANIM_GLOBAL_COMPRESS_TYPE("0", "75");
RES_SET_ANIM_GLOBAL_KEY_COLOR("0x000000");
RES_SET_GLOBAL_JPEG_COMPRESS_RATE("75");

RES_ADD_SKIN("SKIN1", "1.0.0", "0");
RES_ADD_SKIN("SKIN2" , "1.0.0", "0");
RES_ADD_MODULE("MODULE_ID_1","Module1_Path\\Module1_Name" );
RES_ADD_MODULE("MODULE_ID_2", "Module2_Path\\Module2_Name" );

END_GEN_XRP();

=cut

1;