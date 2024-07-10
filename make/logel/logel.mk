include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

logel_label := $(shell $(PERL) $(call Path_process,make/perl_script/getlabel.pl)  ./version/version_software_mocor.h)

.PHONEY:all
all:
ifeq ($(strip $(PRODUCT_DM)), FALSE)
  LOGEL_PATH = Logel
  BAT_FILE = Build.bat
.PHONEY:all
all:
	-@$(DEL_FILE) $(call Path_process,tools/DEBUG_TOOL/LOGEL/Bin/Message_Define.def)
	-@$(DEL_FILE) $(call Path_process,tools/DEBUG_TOOL/LOGEL/Bin/signal_id_header_gen.h)
	-@$(DEL_FILE) $(call Path_process,tools/DEBUG_TOOL/LOGEL/Bin/task_id_header_gen.h)
	-@$(DEL_FILE) $(call Path_process,tools/DEBUG_TOOL/LOGEL/Bin/MapStruct.map)

	cd $(call Path_process,../Tools/Common/Bin/MsgDefBuild/) && $(call Path_process,./$(strip $(BAT_FILE))) C:\Progra~1\ARM\ADSv1_2 $(call Path_process,../../../../) $(logel_label) ..\\..\\..\\..\\MS_Code\\make\\make_cmd\\perl

	-@$(COPY_FILE) $(call Path_process,../Tools/Common/Bin/Message_Define.def) $(call Path_process,tools/DEBUG_TOOL/LOGEL/Bin)
	-@$(COPY_FILE) $(call Path_process,../Tools/Common/Bin/signal_id_header_gen.h) $(call Path_process,tools/DEBUG_TOOL/LOGEL/Bin)
	-@$(COPY_FILE) $(call Path_process,../Tools/Common/Bin/task_id_header_gen.h) $(call Path_process,tools/DEBUG_TOOL/LOGEL/Bin)
	-@$(COPY_FILE) $(call Path_process,../Tools/Common/Bin/MapStruct.map) $(call Path_process,tools/DEBUG_TOOL/LOGEL/Bin)


else
	
	-@$(DEL_FILE) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD/Bin/Message_Define.def)
	-@$(DEL_FILE) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD/Bin/signal_id_header_gen.h)
	-@$(DEL_FILE) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD/Bin/task_id_header_gen.h)
	-@$(DEL_FILE) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD/Bin/MapStruct.map)
	
	-@$(DEL_FILE) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD_cust/Bin/Message_Define.def)
	-@$(DEL_FILE) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD_cust/Bin/signal_id_header_gen.h)
	-@$(DEL_FILE) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD_cust/Bin/task_id_header_gen.h)
	-@$(DEL_FILE) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD_cust/Bin/MapStruct.map)


	cd $(call Path_process,../Tools/Common/Bin/MsgDefBuild/) && $(call Path_process,./Build_dm_2g.bat) C:\Progra~1\ARM\ADSv1_2 $(call Path_process,../../../../) $(logel_label) ..\\..\\..\\..\\MS_Code\\make\\make_cmd\\perl

	-@$(COPY_FILE) $(call Path_process,../Tools/Common/Bin/Message_Define.def) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD/Bin)
	-@$(COPY_FILE) $(call Path_process,../Tools/Common/Bin/signal_id_header_gen.h) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD/Bin)
	-@$(COPY_FILE) $(call Path_process,../Tools/Common/Bin/task_id_header_gen.h) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD/Bin)
	-@$(COPY_FILE) $(call Path_process,../Tools/Common/Bin/MapStruct.map) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD/Bin)
	
	-@$(COPY_FILE) $(call Path_process,../Tools/Common/Bin/Message_Define.def) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD_cust/Bin)
	-@$(COPY_FILE) $(call Path_process,../Tools/Common/Bin/signal_id_header_gen.h) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD_cust/Bin)
	-@$(COPY_FILE) $(call Path_process,../Tools/Common/Bin/task_id_header_gen.h) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD_cust/Bin)
	-@$(COPY_FILE) $(call Path_process,../Tools/Common/Bin/MapStruct.map) $(call Path_process,tools/DEBUG_TOOL/Logel_for_TD_cust/Bin)
	
endif