Public Const vbQuote = """"
currentpath = createobject("Scripting.FileSystemObject").GetFolder(".").Path
temp = currentpath  & "\UploadClient\restart.bat"
path = vbQuote & temp & vbQuote
Set wshell=CreateObject("WScript.Shell") 
wshell.Run path,vbhide
Set wshell = Nothing 
