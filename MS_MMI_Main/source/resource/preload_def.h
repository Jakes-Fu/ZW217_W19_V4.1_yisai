
//flash type flash,maybe change in future
SFS_SET_IS_M8_FLASH("FALSE")
#if defined(PLATFORM_ANTISW3)
	#ifdef APP_STORE_SUPPORT
	SFS_ADD_FOLDER("preload_img\\server")
	#endif

	#ifdef WATCHFACE_MANAGE_SUPPORT
	SFS_ADD_FOLDER("preloadudisk_img\\wf_temp")
	#endif
#endif //PLATFORM_ANTISW3

#if defined(PLATFORM_UWS6121E)
	#ifdef DYNAMIC_WATCHFACE_SUPPORT
	SFS_ADD_FOLDER("preload_img\\ur_root\\watchface")
	#endif

	#if defined(APP_STORE_SUPPORT)
	SFS_ADD_FOLDER("preload_img\\ur_root\\c_app")
	SFS_ADD_FOLDER("preload_img\\server")
	SFS_ADD_FOLDER("preload_img\\installed")
	#endif

	#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
	SFS_ADD_FOLDER("preload_img\\jee")
	SFS_ADD_FOLDER("preload_img\\jee\\system")
	SFS_ADD_FOLDER("preload_img\\jee\\system\\framework")
	//Add preload package(urapk)
	SFS_ADD_FOLDER("preload_img\\jee\\system\\preload")
	//Add preload app(source code)
	SFS_ADD_FOLDER("preload_img\\jee\\data\\app")
	SFS_ADD_FOLDER("preload_img\\ur_root\\apps")
	#endif
#endif //PLATFORM_UWS6121E

