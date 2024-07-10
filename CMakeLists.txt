cmake_minimum_required(VERSION 2.8.0)
project(MSDev2010)

set(CMAKE_SYSTEM_NAME Windows)
set(Module_list "None" CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /ZI")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /ZI")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT /ZI")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd /ZI")
set(default_lib_path lib/;${PROJECT_BINARY_DIR}/bin/;${PROJECT_BINARY_DIR}/Debug/)
set(default_lib Packet.lib;MSDevBridge_vs2010.lib)

## Extract variables from Makefile files
function (extract_make_variable variable makefile_source)
#		  message("extract_make_variable : input variable is ${variable}\n")
#		  message("extract_make_variable : makefile_source is ${makefile_source}\n")

  string(REGEX MATCH "${variable} = ([^$]+)\\$" temp ${makefile_source})
  string(REGEX MATCHALL "[^ \n\t\\]+" listVar ${CMAKE_MATCH_1})
  set (${variable} ${listVar} PARENT_SCOPE)
#  message("extract_make_variable : return variable is ${variable}\n")
endfunction ()

function (add_prefix_to_list var prefix)
  set (listVar "")
  foreach (f ${${var}})
    list(APPEND listVar "${prefix}${f}")
#    message("add_prefix_to_list : return var is ${prefix}${f}\n")
  endforeach ()
  set (${var} "${listVar}" PARENT_SCOPE)
endfunction ()

function (add_prefix_to_list_1 var prefix out_var)
  set (listVar "")
  foreach (f ${${var}})
    list(APPEND listVar "${prefix}${f}")
#    message("add_prefix_to_list_1 : return var is ${prefix}${f}\n")
  endforeach ()
  set (${out_var} "${listVar}" PARENT_SCOPE)
endfunction ()

function (add_lib_extension_to_list in_var out_var)
  set (lib_listVar "")
  foreach (f ${in_var})
    list(APPEND lib_listVar "${f}.lib")
  endforeach ()
  set (${out_var} "${lib_listVar}" PARENT_SCOPE)
endfunction ()

function (add_group_to_list var prefix)
  set (listVar "")
  set (tmpStr "")
  set (tmpModuleStr "")
  set (tmpListStr "")
  set (group_name "" )
  set (group_src_list "")
  set (out_group_src_list "")
  set (out_lib_list "")
  
  foreach (f ${${var}})  	
  	string(REPLACE "///" "\\\\" tmpStr "${f}")
#    list(APPEND listVar "${tmpStr}")  
#    message("add_group_to_list : input str is ${tmpStr}\n")
# get group name 
    string(REGEX MATCHALL "[^ \n\t\\\\]+" tmpListStr ${tmpStr})       
      foreach (ff ${tmpListStr})  
      	#message("${ff}\n")	
      	set (group_name ${ff})      	
		  endforeach ()
#		  message("add_group_to_list : group_name is ${group_name}\n")
		  set(group_src_list "Filter_Group_${group_name}")
#		  message("add_group_to_list : group_src_list is ${group_src_list}\n")
		  extract_make_variable(${group_src_list} ${prefix})
#		  message("add_group_to_list : BEFORE group_src_list is ${group_src_list}\n")
		  add_prefix_to_list_1(${group_src_list} "${PROJECT_SOURCE_DIR}/" out_group_src_list)
#		  message("add_group_to_list : AFTER group_src_list is ${out_group_src_list}\n") 			  
			source_group("${tmpStr}" FILES ${out_group_src_list})		  			  
  endforeach ()
endfunction ()

function (add_source_group group_name source_list)
source_group(${group_name} FILES ${Src_list})
endfunction ()

function (add_prefix_to_list_2 var out_src_var out_lib_var)
  set (SrclistVar "")
  set (LiblistVar "")
  foreach (f ${${var}})
#  	message("add_prefix_to_list_2 f : ${f}\n")
  	if(${f} MATCHES "\\.lib$")
  		list(APPEND LiblistVar ${f})
  	else()
	    list(APPEND SrclistVar ${f})
  	endif()
  endforeach ()
  set (${out_src_var} "${SrclistVar}" PARENT_SCOPE)
  set (${out_lib_var} "${LiblistVar}" PARENT_SCOPE)
endfunction ()



message("Wenny Start")

#file(READ ${PROJECT_SOURCE_DIR}/Makefile.sources SRCSOURCES)
#file(READ ${PROJECT_SOURCE_DIR}/app.list SRCSOURCES)

if(NOT Module_list STREQUAL "None")

#step0 separate module item


string(REPLACE "**" " " tmpModuleStr "${Module_list}")

message("tmpModuleStr ${tmpModuleStr}")

string(REGEX MATCHALL "[^ ]+" AllModule ${tmpModuleStr})

message("AllModule ${AllModule}")

foreach (module_item ${AllModule})
  message("Add Module : ${module_item}\n")
  message("Path : ${PROJECT_BINARY_DIR}/${module_item}/${module_item}.list")
  set (module_src_list "")
  set (module_lib_list "")
  
#step1 get filter list
	file(READ ${PROJECT_BINARY_DIR}/${module_item}/${module_item}.list SRCSOURCES)
#	file(READ ${PROJECT_BINARY_DIR}/${module_item}/${module_item}.dsp_inc INCSOURCES)
	
#	string(REGEX MATCHALL "[^ ]+" incListVar ${INCSOURCES})
	
  extract_make_variable(ALL_sources ${SRCSOURCES})
  extract_make_variable(ALL_Inc ${SRCSOURCES})
  add_prefix_to_list(ALL_sources "${PROJECT_SOURCE_DIR}/")
  add_prefix_to_list(ALL_Inc "${PROJECT_SOURCE_DIR}/")
	add_prefix_to_list_2(ALL_sources module_src_list module_lib_list)
  
  if(${module_item} STREQUAL "msdevkernel")
  	add_library(msdevkernel SHARED ${module_src_list})
  	add_dependencies(msdevkernel ${AllModule})
#  	target_link_directories(msdevkernel PRIVATE "${PROJECT_BINARY_DIR}/Debug/")
#  	add_lib_extension_to_list(${AllModule} out_lib_list)
		target_link_directories(msdevkernel PRIVATE ${default_lib_path})
  	foreach (lib_item ${AllModule})
  	  if(NOT lib_item STREQUAL "msdevkernel")
  		  message("lib_item ${lib_item}")
  		  target_link_libraries(msdevkernel "${lib_item}.lib")
  		endif()
		endforeach ()
		set_target_properties(msdevkernel 
		PROPERTIES 
		ARCHIVE_OUTPUT_DIRECTORY "bin/$<0:>"
		LIBRARY_OUTPUT_DIRECTORY "bin/$<0:>"
		RUNTIME_OUTPUT_DIRECTORY "bin/$<0:>"
		VS_DEBUGGER_COMMAND "${PROJECT_BINARY_DIR}/bin/MSDev.exe"
		LINK_FLAGS_DEBUG "/NODEFAULTLIB:LIBC.lib"
		)
		
		target_link_libraries(msdevkernel ${default_lib})
		
#  	message("out_lib_list ${out_lib_list}")
  else()
#  	add_library(${module_item} ${ALL_sources}) 
  	add_library(${module_item} ${module_src_list}) 
  endif() 

  extract_make_variable(Filter_list ${SRCSOURCES})
  add_group_to_list(Filter_list ${SRCSOURCES})
  
  extract_make_variable(ALL_Macro ${SRCSOURCES})
  target_compile_definitions(${module_item} PRIVATE ${ALL_Macro})
  target_include_directories(${module_item} PRIVATE ${ALL_Inc})
  target_link_libraries(${module_item} "${module_lib_list}")
endforeach ()

message("Module_list ${Module_list}")
message("PROJECT_BINARY_DIR ${PROJECT_BINARY_DIR}")
message("Wenny End")

else()
message("Module_list is Empty !!!")
endif()