xcopy /Y /I /D /EXCLUDE:.\xcopy-filter-bin.txt "..\..\..\public\base\hlog\v2.1.0.42965\win_32_debug\*" ".\Debug"
xcopy /Y /I /D /EXCLUDE:.\xcopy-filter-bin.txt "..\..\..\public\base\hpr\v1.0.0.0\lib\x86_vc60\*" ".\Debug"
xcopy /Y /I /D /EXCLUDE:.\xcopy-filter-bin.txt "..\..\..\public\finance\GeneralPlayerAPI\*" ".\Debug"
xcopy /Y /I /D /EXCLUDE:.\xcopy-filter-bin.txt "..\..\..\public\finance\MsgBox\*" ".\Debug"
xcopy /Y /I /D /EXCLUDE:.\xcopy-filter-bin.txt "..\..\..\public\finance\StreamLayer\*" ".\Debug"
pause;

