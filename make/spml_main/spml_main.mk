ifeq ($(strip $(CHIP_ENDIAN)),	LITTLE)
MCFLAG_OPT = -Otime
else
MCFLAG_OPT = -Otime $(BIG_ENDIAN_OPT) -g+ $(APCS) /interwork -fy
endif

#存在复杂文本,需要严格的计算字符串的宽高
#需要临时变量做或判断
#arabic, thai, hindi, hebrew, tibet等语言为复杂文本
ifeq ($(strip $(IM_ARABIC_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_PERSIAN_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_URDU_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_THAI_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_HINDI_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_HEBREW_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_BENGALI_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_MYANMAR_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

#MCFLAG_OPT += $(MCFLAG_OPT_COMPLEX_TEMP)
MCFLAG_OPT += -DCOMPLEX_TEXT_SCRIPT
#强制使用复杂文本，xiaodongli
#存在复杂文本,需要严格的计算字符串的宽高

MINCPATH =  CAF/Template/include \
  $(MMI_DIR)/source/mmi_spml/include \
  $(MMI_DIR)/source/mmi_spml/source/bidi/h \
  $(MMI_DIR)/source/mmi_spml/source/font/h \
  $(MMI_DIR)/source/mmi_spml/source/layout/h \
  $(MMI_DIR)/source/mmi_spml/source/otf/h \
  $(MMI_DIR)/source/mmi_spml/source/text/h

MSRCPATH =  $(MMI_DIR)/source/mmi_spml/source/bidi/c \
  $(MMI_DIR)/source/mmi_spml/source/font/c \
  $(MMI_DIR)/source/mmi_spml/source/layout/c \
  $(MMI_DIR)/source/mmi_spml/source/otf/c \
  $(MMI_DIR)/source/mmi_spml/source/text/c

SOURCES  =  spml_api.c spml_bidi.c spml_bidi_types.c spml_font.c spml_font_map.c spml_gc.c  \
  spml_line.c spml_otf.c spml_otf_cmap.c spml_otf_gdef.c spml_otf_gpos.c \
  spml_otf_gsub.c spml_otf_kern.c spml_resolve.c spml_resolve_arabic.c spml_resolve_basic.c spml_resolve_myanmar.c \
  spml_resolve_hebrew.c spml_resolve_indic.c spml_resolve_thai.c spml_run.c spml_script.c \
  spml_util.c spml_image.c spml_outline.c spml_resolve_khmer.c spml_resolve_tibetan.c

ifeq ($(strip $(FONT_TYPE_SUPPORT)), VECTOR)
ifeq ($(strip $(VECTOR_FONT_SUPPORT)), DEFAULT)
ifneq ($(strip $(VECTOR_NO_HINTER_SUPPORT)), TRUE)
	MCFLAG_OPT += -DSPML_AUTOHINTER_ENABLE
	MINCPATH += $(MMI_DIR)/source/mmi_spml/source/autofit/h
	MSRCPATH += $(MMI_DIR)/source/mmi_spml/source/autofit/c
	SOURCES +=  spml_afhints.c spml_aflatin.c spml_afcjk.c spml_afindic.c spml_autofit.c
endif
endif
endif
