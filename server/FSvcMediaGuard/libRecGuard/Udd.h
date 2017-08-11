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
#include <ctime>
#include <fclib/fcTchar.h>
//#include <hpr/include/HPR_Types.h>


// static const HPR_INT32 CENTER_STORAGE_TYPE_CVR = 0;//������CVR�洢
// static const HPR_INT32 CENTER_STORAGE_TYPE_CLOUD = 1;//������CLOUD�洢
// static const HPR_INT32 CENTER_STORAGE_TYPE_KMS = 2;//������KMS�洢

typedef struct _cvr_info_t {
    HPR_INT32 nID; // ��ˮ��
    std::_tstring nameID; // ���Ʊ�ʶ
    std::_tstring host;
    std::_tstring port;
    std::_tstring username;
    std::_tstring password;
    std::_tstring groupname;
    HPR_INT32 regionID; // ����������ID
}cvr_info_t;

typedef struct _cloud_info_t {
	HPR_INT32 nID;// ��ˮ��
	std::_tstring nameID; // ���Ʊ�ʶ
	std::_tstring host;
	std::_tstring port;
	std::_tstring username;
	std::_tstring password;
	HPR_INT32 poolid;
	HPR_INT32 regionID; // ����������IDs
}cloud_info_t;

typedef struct _storage_info_t {
    HPR_INT32     nID; // ��ˮ��
    HPR_INT32     nType;  //�洢������
    std::_tstring nameID; // ���Ʊ�ʶ
    std::_tstring host;
    HPR_INT32     nPort;
    std::_tstring username;
    std::_tstring password;
    std::_tstring groupname;
	std::_tstring strAccessKey;
	std::_tstring strSecretKey;
    HPR_INT32 regionID; // ����������ID
}storage_info_t;

// �������ĵ�ҵ���¼,clieninput
typedef struct _bz_record_t {
    HPR_INT32 recordID; // ҵ������ID
    HPR_INT32 tipFlag; // �����˶�������
    std::_tstring bzID; // ҵ��ϵͳID
    std::_tstring bzTime; // ҵ����ʱ��
    std::_tstring productName; // ��Ʒ����
}bz_record_t;

// �������ĵ�¼���¼,prosaverecord
typedef struct _rec_record_t {
    HPR_INT32 ID; // ��ˮ��
    HPR_INT32 recordID; // ¼�������ҵ������ID
    std::_tstring cameraID; // ¼����CVR�ϵ�ID
    HPR_INT32     cvrID; // cvr�ı�ʶ������������
	std::_tstring strBucket; // ����洢��Դ��
    std::_tstring startTime; // ¼��ʼʱ��
    std::_tstring stopTime; // ¼��ν���ʱ��
    bool bUpload;           // �Ƿ��ϴ��ɹ�
	HPR_INT32 devId;        //�ɼ��豸��ʶ
	HPR_INT32 devChannel;   //�ɼ��豸ͨ��
}rec_record_t;

// ��������
typedef struct _work_param_t {
	HPR_INT32 storageType; // �洢��ʽ
	HPR_INT32 tipDays; // �������ʾ��
    std::_tstring startTime; // ��ʼ����ʱ��
    std::_tstring stopTime; // ֹͣ����ʱ��
    _work_param_t(void)
        : storageType(-1)
		, tipDays(-1)
    {
    }
}work_param_t;

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

