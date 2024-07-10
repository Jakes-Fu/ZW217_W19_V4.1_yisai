MCFLAG_OPT    += -DUPDATEREGIONS 
MCFLAG_OPT    += -DMSG_PARTITION 
MCFLAG_OPT    += -D_DEBUG 
MCFLAG_OPT    += -DWRE_KERNEL_MODULE 
MCFLAG_OPT    += -DWRE_SUPPORT 


MINCPATH      = wre/wrekernel
MINCPATH      += wre/wrekernel/gdi/cliprgn
MINCPATH      += wre/wrekernel/gdi/decode
MINCPATH      += wre/wrekernel/gdi/pnglib
MINCPATH      += wre/wrekernel/grafixgs
MINCPATH      += wre/wrekernel/grafixgs/common
MINCPATH      += wre/wrekernel/grafixgs/drv
MINCPATH      += wre/wrekernel/include
MINCPATH      += wre/wrekernel/include/grafixgs
MINCPATH      += wre/wrekernel/include/loader
MINCPATH      += wre/wrekernel/include/osadapt
MINCPATH      += wre/wrekernel/include/sdk
MINCPATH      += wre/wrekernel/include/sdk/gui
MINCPATH      += wre/wrekernel/lib
MINCPATH      += wre/wrekernel/loader
MINCPATH      += wre/wrekernel/loader/elf
MINCPATH      += wre/wrekernel/loader/simulator
MINCPATH      += wre/wrekernel/mwin
MINCPATH      += wre/wrekernel/osadapt
MINCPATH      += wre/wrekernel/string
MINCPATH      += wre/wrekernel/winlib
MINCPATH      += MS_MMI/source/mmi_app/app/wre_boot/h
MINCPATH +=  build/$(PROJECT)_builddir/tmp 

MSRCPATH      = wre/wrekernel
MSRCPATH      += wre/wrekernel/gdi/
MSRCPATH      += wre/wrekernel/gdi/cliprgn
MSRCPATH      += wre/wrekernel/gdi/decode
MSRCPATH      += wre/wrekernel/gdi/pnglib
MSRCPATH      += wre/wrekernel/grafixgs
MSRCPATH      += wre/wrekernel/grafixgs/common
MSRCPATH      += wre/wrekernel/grafixgs/drv
MSRCPATH      += wre/wrekernel/lib
MSRCPATH      += wre/wrekernel/fiber
MSRCPATH      += wre/wrekernel/loader
MSRCPATH      += wre/wrekernel/loader/elf
MSRCPATH      += wre/wrekernel/loader/simulator
MSRCPATH      += wre/wrekernel/mwin
MSRCPATH      += wre/wrekernel/osadapt
MSRCPATH      += wre/wrekernel/string
MSRCPATH      += wre/wrekernel/winlib



SOURCES       = glocklist.c \
                com_locklist.c \
	debug.c \
	public_elf.c \
	exe_elf.c \
	wingdi.c \
	drawtext.c \
	gdi_global.c \
	gdipartition.c \
	winfont.c \
	bmp.c \
	gif.c \
	jpg.c \
	pcx.c \
	png.c \
	devrgn.c \
	devclip.c \
	graf1.c \
	d16b_drv.c \
	d16b_fil.c \
	d16b_lin.c \
	m16b_lin.c \
	lcd_dvc.c \
	m16b_drv.c \
	m16b_fil.c \
	arcs8.c \
	arcs0.c \
	arcs4.c \
	blit_5.c \
	bits0.c \
	bits1.c \
	bits2.c \
	bits3.c \
	bits5.c \
	fill_6.c \
	dpen5.c \
	fill_5.c \
	getp0.c \
	graf0.c \
	grafdata.c \
	lin07.c \
	line0.c \
	line1.c \
	line9.c \
	lineclip.c \
	metmacs.c \
	npen0.c \
	npen1.c \
	npen3.c \
	npen4.c \
	npen8.c \
	nu_math.c \
	oval0.c \
	oval1.c \
	oval4.c \
	oval8.c \
	poly0.c \
	poly4.c \
	poly_4.c \
	rect0.c \
	rect3.c \
	rect4.c \
	rect5.c \
	rect6.c \
	rect8.c \
	reg01.c \
	reg02.c \
	reg09.c \
	regclip.c \
	regclipv.c \
	text3.c \
	zect3.c \
	view0.c \
	view1.c \
	view2.c \
	view3.c \
	view4.c \
	view5.c \
	view8.c \
	view9.c \
	wndo0.c \
	wndo2.c \
	wndo9.c \
	xpol4.c \
	xpol5.c \
	xpol6.c \
	xpol7.c \
	xpol_4.c \
	xpol_5.c \
	xypt6.c \
	yore5.c \
	zect0.c \
	winuser.c \
	dialog.c \
	msgpartition.c \
	msgproc.c \
	mw_global.c \
	readres.c \
	windefw.c \
	winevent.c \
	winexpos.c \
	winmain.c \
	winrgn.c \
	winsbar.c \
	winsend.c \
	utf8.c \
	wcwidth.c \
	widestring.c \
	osadapt_charset.c \
	asyncmsg.c \
	devfont.c \
	dial.c \
	drvtimer.c \
	mwin_socket.c \
	os_handle.c \
	os_process.c \
	os_sema.c \
	os_thread.c \
	os_timer.c \
	os_win32fs.c \
	queuemng.c \
	osadapt_device_global.c \
	osadapt_devlist.c \
	osadapt_devscr.c \
	osadapt_font.c \
	osadapt_gdi.c \
	osadapt_grafixgs.c \
	osadapt_image.c \
	osadapt_ime.c \
	osadapt_lcd.c \
	osadapt_mwin.c \
	osadapt_partition.c \
	osadapt_scr_win.c \
	osadapt_vfs.c \
	wre_stub.c \
	mainWREStartup.c \
	dll_elf.c \
	boot_standard.c \
	osadapt_tapi.c \
	container.c \
	winclip1.c \
	fiber_s.s \
	fiber.c \
	arm_fiber.c \
	ghash.c \
	Function.c \
	APIInterface.c \
	osadapt_PhoneSrv.c