#ifndef GUARD_MYSQLDBLAYER_H
#define GUARD_MYSQLDBLAYER_H

#include <fcMySQL.h>
#include "HCNetSDK.h"
//Â¼ÏñÎÄ¼þ
class CMySqlDBLayer : public CMySQLConnection
{
public:
    CMySqlDBLayer(void);
    ~CMySqlDBLayer(void);

    bool ChecKConnect();

    bool GetConnect(db_conn_info_t& stCenterDBInfo);

    bool CloseConnect();
    //
    bool StateRelateOfflineVideo(std::string& FileName);
    bool StateRelateOfflineVideo(CString& DevIp,LONG lChannel,NET_DVR_FINDDATA_V30& FindData);
private:
    static size_t const MAX_SQL_LEN = 4 * 1024;
};

#endif