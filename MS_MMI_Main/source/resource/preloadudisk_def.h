
//flash type flash,maybe change in future
SFS_SET_IS_M8_FLASH("FALSE")

#ifdef APP_STORE_SUPPORT
SFS_ADD_FOLDER("preloadudisk_img\\server")
#endif

#ifdef DYNAMIC_WATCHFACE_SUPPORT
SFS_ADD_FOLDER("preloadudisk_img\\wf_temp")
#endif
