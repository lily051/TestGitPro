demo 解释

要使用hlog，需要包含：
	hlog.h              // 使用时只需include这个头文件
	hlog4cxx.h
	hlogex.h
	hlog.dll
	hlog.lib           // 这个是需要链接的lib
	hlog.pdb
	log4cxx.dll
	log4cxx.pdb
	log4cxx.properties  // 这个是配置文件
	
包含好头文件和设置好lib后，改动 hlog.h 里的几个宏定义即可