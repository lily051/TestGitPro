#ifndef GUARD_UDD_H
#define GUARD_UDD_H

/**	@file    Udd.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   user defined data
 *
 *	@author	 xuezhihu
 *	@date	 2013/1/25
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V0.0.1  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
 */
#include <cassert>
#include <ctime>
#include <fcTchar.h>
#include <HPR_Types.h>

static const HPR_INT32 CENTER_STORAGE_TYPE_CVR = 1101;//������CVR�洢
static const HPR_INT32 CENTER_STORAGE_TYPE_CLOUD = 1100;//������CLOUD�洢

static const DWORD TP_ALARM_INIT = 1;//��ʼ�����
static const DWORD TP_ALARM_DB = 2;//���ݿ����
static const DWORD TP_ALARM_FILE = 3;//�ϴ��ļ��������
static const DWORD TP_ALARM_STORAGE = 4;//���Ĵ洢���
static const DWORD TP_ALARM_CENTER = 5;//����web���

typedef struct _fileToTrans {
    HPR_INT32     id;
    std::_tstring strLinkOrderID; // �������ݿ��е���ˮ��
    std::_tstring channelID;      // cvr ��ʹ�õı�ʾ��
    std::_tstring filePath;
    HPR_INT32     regionID;
    HPR_INT32     mediaType;      // video or audio
    HPR_INT32     mediaFormat;    // hikmp4, wav, mp3
    HPR_INT32     headSize;       // ý��ͷ��С
    HPR_INT32     callType;       // call in/out
    std::_tstring callNo;
	std::_tstring strUserNo;
    HPR_INT32     nUserID;
    HPR_INT32     linkType;       // 0-��ͨ��¼ 1-���׼�¼ 2-Ͷ�߼�¼
    HPR_INT32     fileBegintime;
    HPR_INT32     fileEndtime;
    std::_tstring dtBusinessTime;
    HPR_INT32     state;
    HPR_INT32     uploadPercent;
    HPR_INT32     iTaskType;     //�������� 0һ��¼�� 1��¼
    std::_tstring strVrmIp;      //Vrm���� IP
    HPR_INT32     iVrmPort;      //Vrm���� �˿ں�
    HPR_INT32     iVodPort;      //Vod���� �˿ں�
    std::_tstring strDvrIp;
    HPR_INT32     iDvrChannel;
	std::_tstring strRecordName;
    std::_tstring strDvrUserName; //dvr��¼�û���
    std::_tstring strDvrPasswd;   //dvr��¼����
    HPR_INT32     iDvrPort;       //dvr��¼ͨ����
    HPR_INT32     iManufacturerType; //DVR����0-����1-��
    std::_tstring strMac;       //�ļ����ڻ���������MAC��ַ
}fileToTrans_t;

typedef struct _fileTransing {
    HPR_INT32     recordID; // records�е���ˮ��
    HPR_INT32     regionID;
    HPR_INT32     mediaType;
    HPR_INT32     mediaFormat;
    HPR_INT32     callType;
    HPR_INT32     linkType;
    std::_tstring callNo;
    std::_tstring strLinkOrderID;
    std::_tstring strUserNo;
    std::_tstring strRecordName;

    std::_tstring sceneID; // not use
    std::_tstring channelID;
    std::_tstring filePath;
    HPR_INT32     headSize;
    HPR_INT64     startOffset;
    HPR_INT32     segBegintime;
    HPR_INT32     segEndtime;
    HPR_INT32     segNo;
    HPR_INT32     uploadPercent;
	HPR_INT32     tryTimes;
    HPR_INT32     iTaskType;     //�������� 0һ��¼�� 1��¼
    std::_tstring strVrmIp;      //Vrm���� IP
    HPR_INT32     iVrmPort;      //Vrm���� �˿ں�
    HPR_INT32     iVodPort;      //Vod���� �˿ں�
    std::_tstring strDvrIp;
    HPR_INT32     iDvrChannel;
    std::_tstring strDvrUserName; //dvr��¼�û���
    std::_tstring strDvrPasswd;   //dvr��¼����
    HPR_INT32     iDvrPort;       //dvr��¼ͨ����
    HPR_INT32     iManufacturerType; //DVR����0-����1-��
    _fileTransing()
        : recordID(0)
        , sceneID(_T(""))
        , channelID(_T(""))
        , filePath(_T(""))
        , headSize(0)
        , startOffset(0)
        , segBegintime(0)
        , segEndtime(0)
        , segNo(0)
        , uploadPercent(0)
        , tryTimes(1)
        , iTaskType(0)
        , strVrmIp(_T(""))
        , iVrmPort(0)
        , iVodPort(0)
        , strDvrIp(_T(""))
        , iDvrChannel(0)
        , strDvrUserName(_T(""))
        , strDvrPasswd(_T(""))
        , iDvrPort(0)
        , iManufacturerType(0)
    {}
    _fileTransing(const fileToTrans_t &fileInfo, HPR_INT64 offset,
        HPR_INT32 segNumber)
        : recordID(fileInfo.id)
        , regionID(fileInfo.regionID)
        , callType(fileInfo.callType)
        , linkType(fileInfo.linkType)
        , mediaType(fileInfo.mediaType)
        , mediaFormat(fileInfo.mediaFormat)
        , callNo(fileInfo.callNo)
        , strUserNo(fileInfo.strUserNo)
        , strRecordName(fileInfo.strRecordName)
        , strLinkOrderID(fileInfo.strLinkOrderID)

        , channelID(fileInfo.channelID)
        , filePath(fileInfo.filePath)
        , headSize(fileInfo.headSize)
        , startOffset(offset)
        , segBegintime(fileInfo.fileBegintime)
        , segEndtime(fileInfo.fileEndtime)
        , segNo(segNumber)
        , uploadPercent(fileInfo.uploadPercent)
        , tryTimes(1)
        , iTaskType(fileInfo.iTaskType)
        , strVrmIp(fileInfo.strVrmIp)
        , iVrmPort(fileInfo.iVrmPort)
        , iVodPort(fileInfo.iVodPort)
        , strDvrIp(fileInfo.strDvrIp)
        , iDvrChannel(fileInfo.iDvrChannel)
        , strDvrUserName(fileInfo.strDvrUserName)
        , strDvrPasswd(fileInfo.strDvrPasswd)
        , iDvrPort(fileInfo.iDvrPort)
        , iManufacturerType(fileInfo.iManufacturerType)
    {}
}fileTransing_t;

typedef struct _FileDel
{
    HPR_INT32     nID;     // �ڱ������ˮ��
    std::_tstring strFilePath;
    _FileDel()
        :nID(-1)
        ,strFilePath(_T(""))
    {}
}FileDel_t, *pFileDel_t;

typedef struct _storage_info_t {
	HPR_INT32 type;
	HPR_INT32 partitionid;
	std::_tstring id;
	std::_tstring host;
	std::_tstring port;
	std::_tstring username;
	std::_tstring password;
	std::_tstring devicecode;
}storage_info_t;

typedef struct _cvr_info_t {
    std::_tstring id;
    std::_tstring host;
    std::_tstring port;
    std::_tstring username;
    std::_tstring password;
    std::_tstring groupname;
}cvr_info_t;

typedef struct _cloud_info_t {
	std::_tstring id;
	std::_tstring host;
	std::_tstring port;
	std::_tstring username;
	std::_tstring password;
	std::_tstring devicecode;
	HPR_INT32 poolid;
}cloud_info_t;

typedef struct _collector_info_t {
    std::_tstring ip;
    HPR_INT32 port;
}collector_info_t;

typedef struct _center_info_t {
	std::_tstring ip;
	HPR_INT32 port;
}center_info_t;

// ��������
typedef struct _work_param_t {
    std::_tstring startTime; // ��ʼ����ʱ�䣬���� 01:00:00
    std::_tstring stopTime; // ֹͣ����ʱ�䣬���� 23:00:00
}work_param_t;

//KMS��Ϣ
typedef struct _Struct_KMSInfo
{
    std::_tstring strKMSIP;
    std::_tstring strKMSPort;
    std::_tstring strKMSUserId;
    std::_tstring strKMSPasswd;
}Struct_KMSInfo;
//NAS��Ϣ
typedef struct _Struct_NASInfo
{
    std::_tstring strNASIP;
    std::_tstring strNASPort;
    std::_tstring strNASUserId;
    std::_tstring strNASPasswd;
}Struct_NASInfo;
// �ж��Ƿ�ո�ˢ�¹��Ĺ���
class CRefresher 
{
public:
    CRefresher()
        :m_updateTime(0)
    {}
    // �Ƿ�ո�ˢ�¹�
    bool IsJustRefresh(int const nInterval)
    {
        assert(nInterval > 0);
        return (::time(NULL) - m_updateTime) < nInterval;
    }
    // ˢ��һ��ʱ��
    void Refresh(void)
    {
        m_updateTime = ::time(NULL);
    }
private:
    time_t m_updateTime; 
};

#endif // end of GUARD_UDD_H

