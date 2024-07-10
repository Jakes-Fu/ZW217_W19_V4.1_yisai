#Preprocessor
MCFLAG_OPT = -D_REENTRANT
MCFLAG_OPT += -c
#MCFLAG_OPT += -O2

#Directory
#Local Dir inc
MINCPATH += external/juniversalchardet/include
MINCPATH += external/juniversalchardet/c/src
MINCPATH += external/juniversalchardet/c/include
MINCPATH += RTOS/export/inc
MINCPATH += MS_Ref/export/inc
MINCPATH += $(MMI_DIR)/source/mmi_gui/include
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/include

#Local Dir src
MSRCPATH = external/juniversalchardet/c/src
MSRCPATH += external/juniversalchardet/c/dll

#            LangGreekModel.cpp \
#            LangHebrewModel.cpp \
#            LangHungarianModel.cpp \
#            LangThaiModel.cpp \
#            nsBig5Prober.cpp \
#            nsSJISProber.cpp \
#            nsEUCJPProber.cpp \
#            nsEUCKRProber.cpp \
#            nsEUCTWProber.cpp \
#            nsEscCharsetProber.cpp \
#            nsEscSM.cpp \
#            nsGB2312Prober.cpp \
#            nsHebrewProber.cpp \
#            nsLatin1Prober.cpp \
#            LangBulgarianModel.cpp \
#            JpCntx.cpp \
#            CharDistribution.cpp \

SOURCES  += LangCyrillicModel.cpp \
            nsCharSetProber.cpp \
            nsMBCSGroupProber.cpp \
            nsMBCSSM.cpp \
            nsSBCSGroupProber.cpp \
            nsSBCharSetProber.cpp \
            nsUTF8Prober.cpp \
            nsUniversalDetector.cpp \
            dll.cpp