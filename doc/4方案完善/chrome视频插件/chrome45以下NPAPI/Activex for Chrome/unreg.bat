@echo off

REG DELETE "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\MozillaPlugins\@hikvision.com/hik-demo-plugin" /f

REG DELETE "HKEY_LOCAL_MACHINE\SOFTWARE\MozillaPlugins\@hikvision.com/hik-demo-plugin" /f
