#MMI需要提高性能的代码, 可以放入此模块

MCFLAG_OPT   	= -Otime

MINCPATH =  CAF/Template/include \
            $(MMI_DIR)/source/mmi_kernel/include \
            $(MMI_DIR)/source/mmi_ctrl/include  \
            $(MMI_DIR)/source/mmi_gui/include\
            $(MMI_DIR)/source/mmi_utility/h

MSRCPATH =  $(MMI_DIR)/source/mmi_utility/c

SOURCES			= mmi_utility.c
