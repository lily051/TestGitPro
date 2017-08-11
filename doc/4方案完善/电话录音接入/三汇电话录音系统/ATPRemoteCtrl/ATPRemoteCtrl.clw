; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSubDlgCfg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "atpremotectrl.h"
LastPage=0

ClassCount=8
Class1=CATPRemoteCtrlApp
Class2=CATPRemoteCtrlDlg
Class3=CSubDlgCfg
Class4=CSubDlgDvd
Class5=CSubDlgInit
Class6=CSubDlgOp
Class7=CSubDlgQuery
Class8=CSubDlgStatus

ResourceCount=7
Resource1=IDD_DIALOG_DVD (English (U.S.))
Resource2=IDD_DIALOG_STATUS (English (U.S.))
Resource3=IDD_DIALOG_INIT (English (U.S.))
Resource4=IDD_DIALOG_OP (English (U.S.))
Resource5=IDD_DIALOG_CFG (English (U.S.))
Resource6=IDD_ATPREMOTECTRL_DIALOG (English (U.S.))
Resource7=IDD_DIALOG_QUERY (English (U.S.))

[CLS:CATPRemoteCtrlApp]
Type=0
BaseClass=CWinApp
HeaderFile=ATPRemoteCtrl.h
ImplementationFile=ATPRemoteCtrl.cpp
LastObject=IDC_EDIT_REBOOT_HOUR

[CLS:CATPRemoteCtrlDlg]
Type=0
BaseClass=CDialog
HeaderFile=ATPRemoteCtrlDlg.h
ImplementationFile=ATPRemoteCtrlDlg.cpp

[CLS:CSubDlgCfg]
Type=0
BaseClass=CDialog
HeaderFile=SubDlgCfg.h
ImplementationFile=SubDlgCfg.cpp
Filter=D
VirtualFilter=dWC

[CLS:CSubDlgDvd]
Type=0
BaseClass=CDialog
HeaderFile=SubDlgDvd.h
ImplementationFile=SubDlgDvd.cpp

[CLS:CSubDlgInit]
Type=0
BaseClass=CDialog
HeaderFile=SubDlgInit.h
ImplementationFile=SubDlgInit.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT_WEBPORT

[CLS:CSubDlgOp]
Type=0
BaseClass=CDialog
HeaderFile=SubDlgOp.h
ImplementationFile=SubDlgOp.cpp

[CLS:CSubDlgQuery]
Type=0
BaseClass=CDialog
HeaderFile=SubDlgQuery.h
ImplementationFile=SubDlgQuery.cpp
Filter=D
VirtualFilter=dWC
LastObject=CSubDlgQuery

[CLS:CSubDlgStatus]
Type=0
BaseClass=CDialog
HeaderFile=SubDlgStatus.h
ImplementationFile=SubDlgStatus.cpp

[DLG:IDD_ATPREMOTECTRL_DIALOG]
Type=1
Class=CATPRemoteCtrlDlg

[DLG:IDD_DIALOG_CFG]
Type=1
Class=CSubDlgCfg

[DLG:IDD_DIALOG_DVD]
Type=1
Class=CSubDlgDvd

[DLG:IDD_DIALOG_INIT]
Type=1
Class=CSubDlgInit

[DLG:IDD_DIALOG_OP]
Type=1
Class=CSubDlgOp

[DLG:IDD_DIALOG_QUERY]
Type=1
Class=CSubDlgQuery

[DLG:IDD_DIALOG_STATUS]
Type=1
Class=CSubDlgStatus

[DLG:IDD_ATPREMOTECTRL_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=5
Control1=IDC_TAB_MAIN,SysTabControl32,1342177280
Control2=IDC_RADIO_MODE,button,1342308361
Control3=IDC_RADIO_MODE2,button,1342177289
Control4=IDC_EDIT_RESULT,edit,1352728580
Control5=IDC_COMBO_SERVER_LIST,combobox,1344339971

[DLG:IDD_DIALOG_INIT (English (U.S.))]
Type=1
Class=CSubDlgInit
ControlCount=18
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_SERVER_IP,edit,1350631552
Control3=IDC_BUTTON_SOAP_DEST,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_LOCAL_IP,edit,1350631552
Control6=IDC_EDIT_LOCAL_PORT,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_USER,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_PWD,edit,1350631552
Control11=IDC_BUTTON_CONNECT,button,1342242816
Control12=IDC_BUTTON_DISCONNECT,button,1342242816
Control13=IDC_STATIC,static,1342308352
Control14=IDC_EDIT_SET_PARAM1,edit,1350631552
Control15=IDC_BUTTON_SetNetTimeout,button,1342242816
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT_WEBPORT,edit,1350631552
Control18=IDC_BUTTON_SETWEBPORT,button,1342242816

[DLG:IDD_DIALOG_CFG (English (U.S.))]
Type=1
Class=CSubDlgCfg
ControlCount=61
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_COMPRESS,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_FILE_FORMAT,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_START_CH,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_TOTAL_CH,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_REC_PATH,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT_REBOOT_TIME,edit,1350631552
Control13=IDC_CHECK_AUTO_REBOOT,button,1342242819
Control14=IDC_STATIC,static,1342308865
Control15=IDC_EDIT_REBOOT_HOUR,edit,1350639744
Control16=IDC_STATIC,static,1342308864
Control17=IDC_EDIT_REBOOT_MIN,edit,1350639744
Control18=IDC_BUTTON_GetGlobalCfg,button,1342242816
Control19=IDC_BUTTON_SetGlobalCfg,button,1342242816
Control20=IDC_STATIC,static,1342308352
Control21=IDC_EDIT_CH,edit,1350631552
Control22=IDC_STATIC,static,1342308352
Control23=IDC_EDIT_REC_TYPE,edit,1350631552
Control24=IDC_STATIC,static,1342308352
Control25=IDC_EDIT_REC_DIR,edit,1350631552
Control26=IDC_STATIC,static,1342308352
Control27=IDC_EDIT_MAXRECLEN,edit,1350631552
Control28=IDC_STATIC,static,1342308352
Control29=IDC_EDIT_REC_INT,edit,1350631552
Control30=IDC_STATIC,static,1342308352
Control31=IDC_EDIT_RCV_DTMF_TIME,edit,1350631552
Control32=IDC_STATIC,static,1342308352
Control33=IDC_EDIT_BIND_PHONE,edit,1350631552
Control34=IDC_STATIC,static,1342308352
Control35=IDC_EDIT_START_DTMF,edit,1350631552
Control36=IDC_STATIC,static,1342308352
Control37=IDC_EDIT_STOP_DTMF,edit,1350631552
Control38=IDC_BUTTON_GetChannelCfg,button,1342242816
Control39=IDC_BUTTON_SetChannelCfg,button,1342242816
Control40=IDC_STATIC,static,1342308352
Control41=IDC_EDIT_IP_ADDR,edit,1350631552
Control42=IDC_STATIC,static,1342308352
Control43=IDC_EDIT_DNS,edit,1350631552
Control44=IDC_STATIC,static,1342308352
Control45=IDC_EDIT_GATEWAY,edit,1350631552
Control46=IDC_STATIC,static,1342308352
Control47=IDC_EDIT_MASK,edit,1350631552
Control48=IDC_BUTTON_GetNetCfg,button,1476460544
Control49=IDC_BUTTON_SetNetCfg,button,1342242816
Control50=IDC_EDIT_DATETIME,edit,1350631552
Control51=IDC_BUTTON_SetDateTime,button,1342242816
Control52=IDC_STATIC,static,1342308352
Control53=IDC_EDIT_OLD_PWD,edit,1350631552
Control54=IDC_STATIC,static,1342308352
Control55=IDC_EDIT_NEW_PWD,edit,1350631552
Control56=IDC_BUTTON_SetAdminPassword,button,1342242816
Control57=IDC_EDIT_LOG_LEVEL,edit,1350631552
Control58=IDC_BUTTON_EnableDebugMsg,button,1342242816
Control59=IDC_EDIT_WEBPORT,edit,1350631552
Control60=IDC_BUTTON_SET_WEBPORT,button,1342242816
Control61=IDC_BUTTON_GET_WEBPORT,button,1342242816

[DLG:IDD_DIALOG_STATUS (English (U.S.))]
Type=1
Class=?
ControlCount=9
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_CH,edit,1350631552
Control3=IDC_BUTTON_GetHardwareInfo,button,1342242816
Control4=IDC_BUTTON_GetChannelStatus,button,1342242816
Control5=IDC_BUTTON_SYGetDiskFreeSpace,button,1342242816
Control6=IDC_BUTTON_GetLineVol,button,1342242816
Control7=IDC_BUTTON_GetChannelRunningStatus,button,1342242816
Control8=IDC_BUTTON_GetDeviceTime,button,1342242816
Control9=IDC_BUTTON_GetDevID,button,1342242816

[DLG:IDD_DIALOG_OP (English (U.S.))]
Type=1
Class=?
ControlCount=32
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_CH,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_MONITORIP,edit,1350631552
Control5=IDC_BUTTON_StartMonitor,button,1342242816
Control6=IDC_BUTTON_StopMonitor,button,1342242816
Control7=IDC_BUTTON_RebootDevice,button,1342242816
Control8=IDC_COMBO_RebootDevice,combobox,1344339971
Control9=IDC_BUTTON_DelFile,button,1342242816
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_DELFILE,edit,1350631552
Control12=IDC_BUTTON_DelFiles,button,1342242816
Control13=IDC_STATIC,static,1342308352
Control14=IDC_EDIT_CH_START,edit,1350631552
Control15=IDC_STATIC,static,1342308352
Control16=IDC_EDIT_CH_END,edit,1350631552
Control17=IDC_STATIC,static,1342308352
Control18=IDC_EDIT_DATE_BEGIN,edit,1350631552
Control19=IDC_EDIT_TIME_BEGIN,edit,1350631552
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_EDIT_DATE_END,edit,1350631552
Control23=IDC_EDIT_TIME_END,edit,1350631552
Control24=IDC_STATIC,static,1342308352
Control25=IDC_BUTTON_StopDelFile,button,1342242816
Control26=IDC_BUTTON_UploadFile,button,1342242816
Control27=IDC_STATIC,static,1342308352
Control28=IDC_EDIT_UploadFile,edit,1350631552
Control29=IDC_BUTTON_DownloadFile,button,1342242816
Control30=IDC_STATIC,static,1342308352
Control31=IDC_EDIT_SaveFile,edit,1350631552
Control32=IDC_BUTTON_StartUploadFile,button,1342242816

[DLG:IDD_DIALOG_DVD (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=IDC_BUTTON_CheckMediaConnected,button,1342242816
Control2=IDC_BUTTON_CheckMediaAvailable,button,1342242816
Control3=IDC_BUTTON_WriteDVD,button,1342242816

[DLG:IDD_DIALOG_QUERY (English (U.S.))]
Type=1
Class=CSubDlgQuery
ControlCount=24
Control1=IDC_EDIT_MSG,edit,1350635588
Control2=IDC_BUTTON_ExecuteSQL,button,1342242816
Control3=IDC_BUTTON_QueryRecord,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_START_DATE,SysDateTimePick32,1342242848
Control6=IDC_START_TIME,SysDateTimePick32,1342242857
Control7=IDC_STATIC,static,1342308352
Control8=IDC_END_DATE,SysDateTimePick32,1342242848
Control9=IDC_END_TIME,SysDateTimePick32,1342242857
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_START_CH,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDIT_END_CH,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT_START_INDEX,edit,1350631552
Control16=IDC_BUTTON_CalcData,button,1342242816
Control17=IDC_RADIO_CALCDATA_MODE,button,1342308361
Control18=IDC_RADIO_CALCDATA_MODE2,button,1342177289
Control19=IDC_DATETIMEPICKER1,SysDateTimePick32,1342373920
Control20=IDC_DATETIMEPICKER2,SysDateTimePick32,1342242848
Control21=IDC_COMBO_CALC_YEAR,combobox,1344339970
Control22=IDC_COMBO_CALC_MON,combobox,1344339971
Control23=IDC_COMBO_MEDIA_TYPE,combobox,1344339971
Control24=IDC_STATIC,static,1342308352

