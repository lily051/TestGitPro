@echo off
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\MozillaPlugins" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\MozillaPlugins\@hikvision.com/hik-demo-plugin" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\MozillaPlugins\@hikvision.com/hik-demo-plugin" /v "Path" /t REG_SZ /d "%~dp0npactivex.dll" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\MozillaPlugins\@hikvision.com/hik-demo-plugin" /v "ProductName" /t REG_SZ /d "HIK Web test Plugin" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\MozillaPlugins\@hikvision.com/hik-demo-plugin" /v "Vendor" /t REG_SZ /d "hikvision" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\MozillaPlugins\@hikvision.com/hik-demo-plugin" /v "Version" /t REG_SZ /d "1.0.0.1" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\MozillaPlugins\@hikvision.com/hik-demo-plugin" /v "Description" /t REG_SZ /d "HIK Web test plug-in for NPAPI browsers" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\MozillaPlugins\@hikvision.com/hik-demo-plugin\MimeTypes" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\MozillaPlugins\@hikvision.com/hik-demo-plugin\MimeTypes\application/hik-demo-plugin" /f


reg add "HKEY_LOCAL_MACHINE\SOFTWARE\MozillaPlugins" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\MozillaPlugins\@hikvision.com/hik-demo-plugin" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\MozillaPlugins\@hikvision.com/hik-demo-plugin" /v "Path" /t REG_SZ /d "%~dp0npactivex.dll" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\MozillaPlugins\@hikvision.com/hik-demo-plugin" /v "ProductName" /t REG_SZ /d "HIK Web test Plugin" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\MozillaPlugins\@hikvision.com/hik-demo-plugin" /v "Vendor" /t REG_SZ /d "hikvision" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\MozillaPlugins\@hikvision.com/hik-demo-plugin" /v "Version" /t REG_SZ /d "1.0.0.1" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\MozillaPlugins\@hikvision.com/hik-demo-plugin" /v "Description" /t REG_SZ /d "HIK Web test plug-in for NPAPI browsers" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\MozillaPlugins\@hikvision.com/hik-demo-plugin\MimeTypes" /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\MozillaPlugins\@hikvision.com/hik-demo-plugin\MimeTypes\application/hik-demo-plugin" /f