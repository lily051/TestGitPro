'Function IsPEFile(fn)
'	IsPEFile = (fn = "PlaybackOCX3.0.ocx" or fn = "PreviewS20OCX.ocx" or fn = "RPConfig.ocx")
'End Function

Function GetOcxType(param)
	If param = "RPConfig.ocx" Then
		GetOcxType = 1
	ElseIf param = "PreviewS20OCX.ocx" Then
		GetOcxType = 2
	ElseIf param = "PlaybackOCX3.0.ocx" Then
		GetOcxType = 3
	Else
		GetOcxType = 0
	End If
End Function

Function GetOcxStdName(param)
	if param = 1 Then
		GetOcxStdName = "RPConfigOCX"
	ElseIf param = 2 Then
		GetOcxStdName = "PreviewOCX"
	ElseIf param = 3 Then
		GetOcxStdName = "PlaybackOCX"
	Else
		GetOcxStdName = ""
	End If
End Function

Dim g_txtFile

Function walkDir(folderspec)
    Dim fso, f, f1, sf
    Set fso = CreateObject("Scripting.FileSystemObject")
    Set f = fso.GetFolder(folderspec)
    Set sf = f.Files
    For Each f1 in sf
        fn = folderspec + "/" + f1.name
		ocxType = GetOcxType(f1.name)
        if ocxType > 0 Then
            fv = fso.GetFileVersion(fn)
            if Len(fv) > 0 Then
				fv = Replace(fv, ".", ",")
                str1 = GetOcxStdName(ocxType) + "=" + Chr(34) + fv + Chr(34)
            else
                str1 = GetOcxStdName(ocxType) + "=" + """"
            End if
			g_txtFile.WriteLine(str1)
        End if
    Next
	
    Set sf = f.SubFolders
    For Each f1 in sf
        fn = folderspec + "/" + f1.name
        walkDir(fn)
    Next
    
End Function

g_dstFile = "ocxversion.js"
Set fso = CreateObject("Scripting.FileSystemObject")
Set g_txtFile = fso.CreateTextFile(g_dstFile, True)

'str = Date + Time
'g_txtFile.WriteLine(str)

walkDir(".")
g_txtFile.Close()
MsgBox("ÒÑ±£´æµ½ocxversion.js")

