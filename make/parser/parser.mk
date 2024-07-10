# + PARSER module START
#----------------------------------------------------------------------------#
#                            browser_render                                  #
#----------------------------------------------------------------------------#
MCFLAG_OPT    += -D_RTOS
MINCPATH      += PARSER/export/inc \
                PARSER/source/browser_render/inc \
                DAPS/source/wbxml_parser/inc \
                BASE/l4/export/inc \
                BASE/ps/export/inc \
                DAPS/source/common/inc

MSRCPATH      += PARSER/source/browser_render/src

SOURCES       += brw_sgml_parser.c brw_global.c brw_core_api.c brw_layout.c \
	             brw_parser.c brw_url.c brw_wml_layout.c brw_wml_parser.c brw_xhtml_layout.c brw_xhtml_parser.c brw_xhtml_calc_draw.c
	         
ifeq ($(strip $(CSS_SUPPORT)), TRUE)				
SOURCES       += brw_css.c
endif
ifeq ($(strip $(JS_SUPPORT)), TRUE)				
SOURCES       += brw_js.c
endif
#----------------------------------------------------------------------------#
#                            javascript                                        #
#----------------------------------------------------------------------------#
ifeq ($(strip $(JS_SUPPORT)), TRUE)

MCFLAG_OPT    += -D_X86_=1 -D_WINDOWS  -D_IEEE_LIBM   -DDEBUG   -DXP_WIN  -DJSFILE  -DEXPORT_JS_API

ifeq ($(strip $(COMPILER)), ADS)
MCFLAG_OPT += -DHAVE_VA_LIST_AS_ARRAY
endif

MINCPATH    += PARSER/source/js_spidermonkey/src \
                 PARSER/source/js_object/inc \
                 PARSER/source/js_spidermonkey/src/fdlibm/fdlibm \
                 PARSER/export/inc \
                 PARSER/source/browser_render/inc

MSRCPATH    += PARSER/source/js_spidermonkey/src \
                 PARSER/source/js_spidermonkey/src/fdlibm/fdlibm \
                 PARSER/source/js_object/src
                                                                    
SOURCES     +=  js.c jsapi.c  jsarena.c  jsarray.c  jsatom.c  jsbool.c jscntxt.c \
                jsdate.c  jsdbgapi.c  jsdhash.c  jsdtoa.c  jsemit.c jsexn.c  jsfun.c jsgc.c  \
                jshash.c  jsinterp.c  jslock.c jslog2.c jslong.c jsmath.c jsnum.c  jsobj.c  jsopcode.c  jsparse.c   \
                jsprf.c  jsregexp.c  jsscan.c  jsscope.c  jsscript.c jsstr.c  jsutil.c  jsxdrapi.c jsxml.c \
                jsobject_doc.c  jsobject_event.c prmjtime.c  jsobject_navigator.c jsobject_screen.c jsobject_history.c jsobject_location.c \
                jsobject_body.c jsobject_image.c jsobject_link.c jsobject_form.c   jsobject_option.c  jsobject_inputhidden.c   jsobject_tablecell.c\
                jsobject_global.c   jsobject_select.c   jsobject_checkbox.c  jsobject_inputbutton.c  jsobject_inputsubmit.c  jsobject_table.c  jsobject_tablerow.c \
                jsobject_inputpassword.c  jsobject_inputradio.c  jsobject_inputreset.c   jsobject_inputtext.c   jsobject_anchor.c  jsobject_textarea.c  jsobject_div.c

SOURCES     +=  e_atan2.c e_pow.c  e_sqrt.c  k_standard.c s_atan.c  s_copysign.c  s_fabs.c \
                s_finite.c  s_isnan.c   s_matherr.c s_rint.c s_scalbn.c w_atan2.c   w_pow.c  w_sqrt.c  \
                
endif	                          
#----------------------------------------------------------------------------#
#                            libcss                                          #
#----------------------------------------------------------------------------#
ifeq ($(strip $(CSS_SUPPORT)), TRUE)
MCFLAG_OPT    += -D_RTOS -DNDEBUG
MINCPATH      += PARSER/source/libcss/libcss_src/include \
		 PARSER/source/libcss/libcss_src/src \
		 PARSER/source/libcss/libcss_src/src/charset \
		 PARSER/source/libcss/libcss_src/src/utils \
                 PARSER/source/libcss/libcss_src/src/select \
                 PARSER/source/libcss/libcss_src/src/parse \
                 PARSER/source/libcss/libcss_src/src/parse/properties \
                 PARSER/source/libcss/libcss_src/src/lex \
                 PARSER/source/libcss/libcss_src/src/charset \
                 PARSER/source/libcss/libcss_src/src/bytecode \
                 PARSER/source/libcss/libparserutils_src/src \
                 PARSER/source/libcss/libparserutils_src/include \
                 PARSER/source/libcss/libparserutils_src/src/charset \
                 PARSER/source/libcss/libparserutils_src/src/input \
                 PARSER/source/libcss/libparserutils_src/src/utils \
                 PARSER/source/libcss/libwapcaplet_src/include \
                 BASE/ps/export/inc 
                 
                 
MSRCPATH      += PARSER/source/libcss/libcss_src/src \
		 PARSER/source/libcss/libcss_src/src/utils \
                 PARSER/source/libcss/libcss_src/src/select \
                 PARSER/source/libcss/libcss_src/src/parse \
                 PARSER/source/libcss/libcss_src/src/parse/properties \
                 PARSER/source/libcss/libcss_src/src/lex \
                 PARSER/source/libcss/libcss_src/src/charset \
                 PARSER/source/libcss/libcss_src/src/bytecode \
                 PARSER/source/libcss/libparserutils_src/src \
                 PARSER/source/libcss/libparserutils_src/src/charset \
                 PARSER/source/libcss/libparserutils_src/src/charset/codecs \
                 PARSER/source/libcss/libparserutils_src/src/charset/encodings \
                 PARSER/source/libcss/libparserutils_src/src/input \
                 PARSER/source/libcss/libparserutils_src/src/utils \
                 PARSER/source/libcss/libwapcaplet_src/src \
                                                                    
SOURCES       += stylesheet.c libcss.c detect.c lex.c important.c language.c parse.c propstrings.c border_outline.c display_sizing.c effects_stacking.c generated_list.c \
		aural.c background.c font.c margin.c padding.c page.c positioning.c parser_properties.c table.c text.c ui.c properties_utils.c \
		computed.c dispatch.c hash.c select_properties.c select.c cssutils_errors.c utils_utils.c \
		parserutils.c aliases.c charset.c codec.c codec_8859.c codec_ascii.c codec_ext8.c codec_utf16.c codec_utf8.c \
		utf16.c utf8.c filter.c inputstream.c buffer.c parserutil_errors.c stack.c vector.c \
		libwapcaplet.c \
		brw_dump.c
endif						
#----------------------------------------------------------------------------#
#                            mms_core                                        #
#----------------------------------------------------------------------------#
ifeq ($(strip $(MMS_SUPPORT)), DORADO)
MCFLAG_OPT    += -D_RTOS
MINCPATH      += PARSER/export/inc \
				DAPS/export/inc \
                PARSER/source/mms_core/inc

MSRCPATH      += PARSER/source/mms_core/src

SOURCES       += mMmsDecoder.c mMmsNetwork.c mMmsParser.c sysMemFile.c mMmsEncoder.c \
				mMmsDoc.c mMmsSmil.c mMmsHttp.c mMmsMain.c
endif


# - PARSER module END
