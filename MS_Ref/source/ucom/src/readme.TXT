.romdisk.bin
. a disk img file, enable autorun under windows
. to install ucom driver and related software automaticaly
. ucom_ubot_xxx.c is used to relealize this function
. finally it's a composite device of ucom and ubot scsi

1. rom disk 光盘中包含可用于Windows系统的Modem驱动以及G3 eWalk(中国移动随意行)的安装文件，光盘中文件/文件夹介绍如下：
   1)DriverSetup.exe：用以检测PC是否安装过Modem驱动以及G3 eWalk(中国移动随意行)，如未安装，则提示用户进行安装
   2)AutoRun.inf：光盘的Autorun配置文件，使光盘具有自动运行功能
   3)ico.ico: 光盘的ico，作为光盘的图标出现在“我的电脑”里
   4)INI文件夹：包含安装驱动以及随意行所需要的一些配置信息及程序
   5)driver_x86文件夹：包含支持32位Windows系统的Modem驱动
   6)driver_x64文件夹：包含支持64位Windows系统的Modem驱动
   7)G3 eWalk文件夹：包含G3 eWalk(中国移动随意行)的安装程序
2. 如果PC的Autorun功能未被禁用，每次当手机连接PC并选择“PC数据通讯”后，DriverSetup.exe都会被自动运行，DriverSetup.exe将检测PC是否安装过Modem驱动以及G3 eWalk(中国移动随意行)，如果检测到未安装，将会提示用户进行安装。
3. 如果PC的Autorun功能被禁用，则需要手动运行DriverSetup.exe以安装Modem驱动以及G3 eWalk(中国移动随意行).
4. Modem驱动的版本为2.4.0.11，支持Windows 2000, Windows XP 32/64 bit, Windows Vista 32/64 bit, Windows 7 32 bit(Windows 7 64 bit会因为没有微软签名而安装不了)
5. G3 eWalk(中国移动随意行)版本为3.2.1.16，版权归中国移动所有
6. 如需卸载Modem驱动或G3 eWalk(中国移动随意行)，请通过“控制面板”里的“添加/删除程序”进行卸载
7. Modem驱动及G3 eWalk(中国移动随意行)的安装/卸载都需要用户账户具有管理员权限

