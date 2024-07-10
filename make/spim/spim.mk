
MCFLAG_OPT = -O2 -Otime

MINCPATH = CAF/Template/include \
  $(MMI_DIR)/source/spim/include \
  $(MMI_DIR)/source/spim/source/inc

MSRCPATH =  $(MMI_DIR)/source/spim/source/src

SOURCES = spim_ch.c spim_ch_asso.c spim_ch_stroke.c \
  spim_ch_py26k.c spim_ch_py9k.c \
  spim_spells.c spim_spells9k.c \
  spim_english.c


