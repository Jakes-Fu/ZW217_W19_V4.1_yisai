echo "set LM_LICENSE_FILE=8224@shlic01  "
echo "set LM_LICENSE_FILE=7010@172.16.0.235   For ARMCC5	"
echo "set LM_LICENSE_FILE=7010@172.16.0.137   For ARMCC5	"
echo "set LM_LICENSE_FILE=7010@172.16.0.138   For ARMCC5	"
echo "make sure C:\Users\simon.yang\AppData\Local\Temp\22    is not full "
echo "make p=UIX8910_128X256_320X240  -j2 job=2  new    "
echo "       make p=UIS8910C_128x128_320x240_refphone -j2 job=2 new   "

echo %1
ping 1.1.1.1 -n 1 >null
make p=UIX8910_128X256_320X240 m=%1 -j2 job=2 && make p=UIX8910_128X256_320X240  -j2 job=2 image  && mv /cygdrive/d/yangtt/mocor-src/build/UIX8910_128X256_320X240_builddir/img/UIX8910_128X256_320X240.pac  /cygdrive/d/yangtt/mocor-src/build/UIX8910_128X256_320X240_builddir/img/UIX8910_128X256_320X240.pac.bin && cp -fr  /cygdrive/d/yangtt/mocor-src/build/UIX8910_128X256_320X240_builddir/img/UIX8910_128X256_320X240.pac.bin /cygdrive/x/