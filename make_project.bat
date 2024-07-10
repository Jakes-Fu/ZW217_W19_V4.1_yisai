@set ARMCONF=
@set ARMDLL=
@set ARMHOME=
@set ARMINC=
@set ARMLIB=
make\make_cmd\make -r -R MAKESHELL=CMD p=%1 PID=QA job=32 -j16 new
make\make_cmd\make -r -R MAKESHELL=CMD p=%1 PID=MH job=32 -j16 m="resource_main nv_parameter"
make\make_cmd\make -r -R MAKESHELL=CMD p=%1 PID=MH job=32 -j16 image
make\make_cmd\make -r -R MAKESHELL=CMD p=%1 PID=IM job=32 -j16 m="resource_main nv_parameter"
make\make_cmd\make -r -R MAKESHELL=CMD p=%1 PID=IM job=32 -j16 image
make\make_cmd\make -r -R MAKESHELL=CMD p=%1 PID=AA job=32 -j16 m="resource_main nv_parameter"
make\make_cmd\make -r -R MAKESHELL=CMD p=%1 PID=AA job=32 -j16 image
make\make_cmd\make -r -R MAKESHELL=CMD p=%1 PID=EI job=32 -j16 m="resource_main nv_parameter"
make\make_cmd\make -r -R MAKESHELL=CMD p=%1 PID=EI job=32 -j16 image

