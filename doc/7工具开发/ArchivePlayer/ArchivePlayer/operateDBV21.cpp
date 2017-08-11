#include "StdAfx.h"
#include "operateDBV21.h"
#include "MysqlPool.h"

operateDBV21::operateDBV21(void)
{
    m_beLoginMysql = FALSE;
    sqlp = NULL;
}

operateDBV21::~operateDBV21(void)
{
    m_beLoginMysql = FALSE;
    if ( NULL != sqlp)
    { 
        delete sqlp;
        sqlp = NULL;
    }
}

static const char *g_dbnameBussiness = "ivms8000fms";

unsigned int __stdcall  loginThread(void* pParam)
{
    operateDBV21 *pDlg = (operateDBV21*)pParam;
    pDlg->LoginMySQL();
    InterlockedExchange(&pDlg->m_beLoginMysql,FALSE);
    return 0;
} 

static void defaultErrMsg(const int code ,const char* msg,void* userdata)
{
    // OutputDebugString(msg);
}

BOOL operateDBV21::LoginMySQL()
{   
    if(NULL == sqlp)
    { 
        sqlp = new MysqlPool(2,g_dbnameBussiness,
            m_Info.Ip.c_str(),
            m_Info.Port.c_str(),
            m_Info.Username.c_str(),
            m_Info.Password.c_str());
    }

   
    if (NULL  == sqlp)
    {
        m_Info.pErrCallback(-1,"内存分配失败",m_Info.pData);
        return FALSE;
    }

    MYSQL *sqlcon = sqlp->GetConnection();
    if (NULL == sqlcon)
    {
        coding eCode;     
        m_Info.pErrCallback(-1,eCode.UnicodeToAnsi(sqlp->str_error.c_str()),m_Info.pData);
        ARCHIVEPLAY_LOG_ERROR("%s",eCode.UnicodeToAnsi(sqlp->str_error.c_str()));      
        sqlp->ReleaseConnection(sqlcon);   

        if ( NULL != sqlp)
        { 
            delete sqlp;
            sqlp = NULL;
        }
        return FALSE;
    }
    InterlockedExchange(&m_beLoginMysql,TRUE);
    sqlp->ReleaseConnection(sqlcon);  
    m_Info.pErrCallback(0,"Login successfully",m_Info.pData);
    return TRUE;
}



int initItemV21(std::vector<Iteminfo>& plist)
{
  
    Itemlist.clear();
    Iteminfo tmpinfo;
    tmpinfo.Dbname = _T("nProId");
    tmpinfo.listname = _T("业务单号");
    tmpinfo.type = TABLE_CLIENTINPUT;
    Itemlist.push_back(tmpinfo);

    /*tmpinfo.Dbname = _T("status");
    tmpinfo.listname = _T("业务单状态");
    tmpinfo.type = TABLE_CLIENTINPUT;
    Itemlist.push_back(tmpinfo);*/

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("inputRegionName");
    tmpinfo.listname = _T("所属网点");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("productName");
    tmpinfo.listname = _T("产品名称");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("productTypeName");
    tmpinfo.listname = _T("产品类型名称");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("proName");
    tmpinfo.listname = _T("业务员姓名");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("inputTime");
    tmpinfo.listname = _T("录入时间");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_PROSAVERECORD;
    tmpinfo.Dbname = _T("fileName");
    tmpinfo.listname = _T("录像名称");
    Itemlist.push_back(tmpinfo);

 /*   tmpinfo.Dbname = _T("");
    tmpinfo.listname = _T("录像类型");
    Itemlist.push_back(tmpinfo);*/

    tmpinfo.type = TABLE_PROSAVERECORD;
    tmpinfo.Dbname = _T("upload");
    tmpinfo.listname = _T("录像状态");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_PROSAVERECORD;
    tmpinfo.Dbname = _T("startTime");
    tmpinfo.listname = _T("开始时间");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("endTime");
    tmpinfo.listname = _T("结束时间");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_DEVICE;
    tmpinfo.Dbname = _T("strIp");
    tmpinfo.listname = _T("存储IP");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_PROSAVERECORD;
    tmpinfo.Dbname = _T("streamNo");
    tmpinfo.listname = _T("编码器信息");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = -1;
    tmpinfo.Dbname = _T("");
    tmpinfo.listname = _T("设备录像状况");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = -1;
    tmpinfo.Dbname = _T("");
    tmpinfo.listname = _T("中心录像状况");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = -1;
    tmpinfo.Dbname = _T("");
    tmpinfo.listname = _T("异常描述");
    Itemlist.push_back(tmpinfo);
    return TRUE;
}
void operateDBV21::setDBInfo(mysqlInfo& pInfo,std::vector<Iteminfo>& plist)
{
    m_Info = pInfo;
    if (m_Info.pErrCallback == NULL)
    {
        m_Info.pErrCallback = defaultErrMsg;
    }
    initItemV21(plist);
    if (FALSE == m_beLoginMysql)
    {
        InterlockedExchange(&m_beLoginMysql,TRUE); 
        m_loginThreadHandle = (HANDLE)_beginthreadex(NULL, 0, loginThread, this, 0,NULL);

    }
}


BOOL operateDBV21::searchFormRecords(const NET_DVR_TIME &StartDayTime, const NET_DVR_TIME &StopDayTime)
{
    //历史录像查询
    BOOL ret = FALSE;

    char cstartTime[TIME_SIZE]={0};
    char cendTime[TIME_SIZE]={0};
    /*2017-05-11 11:45:05*/
    sprintf_s(cstartTime,sizeof(cstartTime),"%04d-%02d-%02d %02d:%02d:%02d",StartDayTime.dwYear,StartDayTime.dwMonth,StartDayTime.dwDay,StartDayTime.dwHour,StartDayTime.dwMinute,StartDayTime.dwSecond);
    sprintf_s(cendTime,sizeof(cendTime),"%04d-%02d-%02d %02d:%02d:%02d",StopDayTime.dwYear,StopDayTime.dwMonth,StopDayTime.dwDay,StopDayTime.dwHour,StopDayTime.dwMinute,StopDayTime.dwSecond);

    char sqlbuf[BUFFERSIZE]={0};

    sprintf_s(sqlbuf,sizeof(sqlbuf),"select clientId,fileName,stateType,upload,startTime,endTime,devId,cvr,devChannel,streamNo from ProSaveRecord where startTime between '%s' and '%s'",cstartTime,cendTime);

    if (!g_filter.empty())
    {
        std::size_t j = 0;
        for (j = 0;j<g_filter.size();++j)
        {
            if (g_filter.at(j).type == TABLE_PROSAVERECORD)
            {
                break;
            }
        }
        if (j != g_filter.size())
        {
            int cnt = 0;
            for (std::size_t i = 0;i<g_filter.size();++i)
            {
                if (g_filter.at(i).type == TABLE_PROSAVERECORD)
                {
                    sprintf_s(sqlbuf,sizeof(sqlbuf),"%s and %s like '%s%%' ",sqlbuf,g_filter.at(i).condition.c_str(),g_filter.at(i).data.c_str());
                    cnt++;
                }
            }

        }
    }

    MYSQL_RES *result =NULL; 
    MYSQL_ROW sql_row; 
    MYSQL *sqlcon = sqlp->GetConnection();
    if(sqlcon == NULL)
    {
        coding eCode;
        ARCHIVEPLAY_LOG_ERROR("无法获取数据库连接:%s",eCode.UnicodeToAnsi(sqlp->str_error.c_str()));
        m_Info.pErrCallback(-1,eCode.UnicodeToAnsi(sqlp->str_error.c_str()),m_Info.pData);
        sqlp->ReleaseConnection(sqlcon);   
        return FALSE;
    }

    int res=mysql_query(sqlcon,sqlbuf);

    int itemcnt = 0;
    if(0 == res) 
    { 
        result=mysql_store_result(sqlcon);
        if(NULL != result) 
        { 
            my_ulonglong nrows = mysql_num_rows(result);
            if (0 == nrows)
            {              
                mysql_free_result(result); 
                sqlp->ReleaseConnection(sqlcon);
                return FALSE;
            }
            my_ulonglong ncols = mysql_num_fields(result); 

            itemcnt = (int)ncols;

            RecordInfo tepinfo = {0};
            while(sql_row=mysql_fetch_row(result)) 
            { 
                //tepinfo = {0};
                if (NULL != sql_row[0])
                {
                    tepinfo.Businessid = atoi(sql_row[0]);
                }
                if (NULL != sql_row[1])
                {
                    sprintf_s(tepinfo.name, sizeof(tepinfo.name),"%s", sql_row[1]);
                }

                if (NULL != sql_row[2])
                {
                    tepinfo.type = atoi(sql_row[2]);
                }

                if (NULL != sql_row[3])
                {
                    tepinfo.status = atoi(sql_row[3]);
                }

                if (NULL != sql_row[4])
                {
                    sprintf_s(tepinfo.startTime, sizeof(tepinfo.startTime),"%s", sql_row[4]);
                }
                if (NULL != sql_row[5])
                {
                    sprintf_s(tepinfo.endTime, sizeof(tepinfo.endTime),"%s", sql_row[5]);
                }

                if(NULL != sql_row[6])
                {
                    tepinfo.devid= atoi(sql_row[6]);
                }
                if(NULL != sql_row[7])
                {
                    tepinfo.cvrid = atoi(sql_row[7]);
                }

                if(NULL != sql_row[8])
                {
                    tepinfo.channel = atoi(sql_row[8]);
                }

                if(NULL != sql_row[9])
                {
                    sprintf_s(tepinfo.streamNo, sizeof(tepinfo.streamNo),"%s", sql_row[9]);
                }
                RecordInfoList.push_back(tepinfo);               
            }

        } 
    }
    else
    {
        TRACE("%s\n",mysql_error(sqlcon));
        ARCHIVEPLAY_LOG_ERROR("%s",mysql_error(sqlcon));
        ARCHIVEPLAY_LOG_ERROR("<%s>查找失败",sqlbuf);
    }

    if(NULL != result)
    {   
        mysql_free_result(result); //释放结果资源 
        result = NULL;
    }

    if (RecordInfoList.empty())
    {
        sqlp->ReleaseConnection(sqlcon);
        return FALSE;
    }

    sqlp->ReleaseConnection(sqlcon);
    return TRUE;
}

BOOL operateDBV21::searchBusiness()
{
    BusinessInfolist.clear();
    char sqlbuf[BUFFERSIZE]={0};

    sprintf_s(sqlbuf,sizeof(sqlbuf),"select id,nProId,status,inputRegionName,productTypeName,productName,proName,inputTime from clientinput");
    if (!g_filter.empty())
    {
        std::size_t j = 0;
        for (j = 0;j<g_filter.size();++j)
        {
            if (g_filter.at(j).type == TABLE_CLIENTINPUT)
            {
                break;
            }
        }

        int cnt = 0;
        if (j != g_filter.size())
        {

            //sprintf_s(sqlbuf,sizeof(sqlbuf),"%s where",sqlbuf);
            for (std::size_t i = 0;i<g_filter.size();++i)
            {
                if (g_filter.at(i).type == TABLE_CLIENTINPUT)
                {
                    if (cnt != 0)
                    {
                        sprintf_s(sqlbuf,sizeof(sqlbuf),"%s and %s like '%s%%' ",sqlbuf,g_filter.at(i).condition.c_str(),g_filter.at(i).data.c_str());
                    }
                    else
                    {
                        sprintf_s(sqlbuf,sizeof(sqlbuf),"%s where %s like '%s%%' ",sqlbuf,g_filter.at(i).condition.c_str(),g_filter.at(i).data.c_str());

                    }
                    cnt++;
                }
            }

        }

    }

    MYSQL_RES *result =NULL; 
    MYSQL_ROW sql_row; 
    MYSQL *sqlcon = sqlp->GetConnection();
    if(sqlcon == NULL)
    {
        coding eCode;
        ARCHIVEPLAY_LOG_ERROR("无法获取数据库连接:%s",eCode.UnicodeToAnsi(sqlp->str_error.c_str()));
        sqlp->ReleaseConnection(sqlcon);  
        m_Info.pErrCallback(-1,eCode.UnicodeToAnsi(sqlp->str_error.c_str()),m_Info.pData);
        return FALSE;
    }

    int res=mysql_query(sqlcon,sqlbuf);

    int itemcnt = 0;
    if(0 == res) 
    { 
        result=mysql_store_result(sqlcon);
        if(NULL != result) 
        { 
            my_ulonglong nrows = mysql_num_rows(result);
            if (0 == nrows)
            {              
                mysql_free_result(result); 
                sqlp->ReleaseConnection(sqlcon);
                return FALSE;
            }
            my_ulonglong ncols = mysql_num_fields(result); 

            itemcnt = (int)ncols;

            BusinessInfo tepinfo = {0};

            while(sql_row=mysql_fetch_row(result)) 
            { 


                if (NULL != sql_row[0])
                {
                    tepinfo.id = atoi(sql_row[0]);
                }
                if (NULL != sql_row[1])
                {
                    sprintf_s(tepinfo.number, sizeof(tepinfo.number),"%s", sql_row[1]);
                }
                if (NULL != sql_row[2])
                {
                    tepinfo.status = atoi(sql_row[2]);
                }
                if (NULL != sql_row[3])
                {
                    sprintf_s(tepinfo.regionName, sizeof(tepinfo.regionName),"%s", sql_row[3]);
                }
                if (NULL != sql_row[4])
                {
                    sprintf_s(tepinfo.productTypeName, sizeof(tepinfo.productTypeName),"%s", sql_row[4]);
                }
                if (NULL != sql_row[5])
                {
                    sprintf_s(tepinfo.productName, sizeof(tepinfo.productName),"%s", sql_row[5]);
                }
                if (NULL != sql_row[6])
                {
                    sprintf_s(tepinfo.proName, sizeof(tepinfo.proName),"%s", sql_row[6]);
                }
                if (NULL != sql_row[7])
                {
                    sprintf_s(tepinfo.inputTime, sizeof(tepinfo.inputTime),"%s", sql_row[7]);
                }
                //BusinessInfolist.push_back(tepinfo);
                if (NULL != sql_row[0])
                { 
                    BusinessInfolist.insert(std::make_pair(tepinfo.id, tepinfo));
                }


            }

        } 
    }
    else
    {
        TRACE("%s\n",mysql_error(sqlcon));
        ARCHIVEPLAY_LOG_ERROR("%s",mysql_error(sqlcon));
        ARCHIVEPLAY_LOG_ERROR("<%s>查找失败",sqlbuf);
    }

    if(NULL != result)
    {   
        mysql_free_result(result); //释放结果资源 
        result = NULL;
    }

    if (BusinessInfolist.empty())
    {
        sqlp->ReleaseConnection(sqlcon);
        return FALSE;
    }   
    sqlp->ReleaseConnection(sqlcon);

    return TRUE;
}





int operateDBV21::getStoreInfo(RecordInfo &record)
{
    char sqlbuf[BUFFERSIZE]={0};

    MYSQL_RES *result =NULL; 
    MYSQL_ROW sql_row; 
    MYSQL *sqlcon = sqlp->GetConnection();
    if(sqlcon == NULL)
    {
        coding eCode;
        ARCHIVEPLAY_LOG_ERROR("无法获取数据库连接:%s",eCode.UnicodeToAnsi(sqlp->str_error.c_str()));
        sprintf_s(record.errMsg,sizeof(record.errMsg),"与数据库连接断开");
        m_Info.pErrCallback(-1,eCode.UnicodeToAnsi(sqlp->str_error.c_str()),m_Info.pData);
        sqlp->ReleaseConnection(sqlcon);   
        return FALSE;
    }
    DevicInfo &devinfo = record.store;
   
    sprintf_s(sqlbuf,sizeof(sqlbuf),"select nType,strIp,nPort,strUser,strPassword from Device where id = '%d'",record.cvrid);

    if (!g_filter.empty())
    {
        std::size_t j = 0;
        for (j = 0;j<g_filter.size();++j)
        {
            if (g_filter.at(j).type == TABLE_DEVICE)
            {
                break;
            }
        }
        if (j != g_filter.size())
        {
            int cnt = 0;
            for (std::size_t i = 0;i<g_filter.size();++i)
            {
                if (g_filter.at(i).type == TABLE_DEVICE)
                {
                    sprintf_s(sqlbuf,sizeof(sqlbuf),"%s and %s like '%s%%' ",sqlbuf,g_filter.at(i).condition.c_str(),g_filter.at(i).data.c_str());
                    cnt++;
                }
            }

        }
    }
    
    int  res = mysql_query(sqlcon,sqlbuf);

    if(0 == res) 
    { 
        result=mysql_store_result(sqlcon);
        if(NULL != result) 
        { 
            my_ulonglong nrows = mysql_num_rows(result);
            if (0 == nrows)
            {
                mysql_free_result(result); 
                sqlp->ReleaseConnection(sqlcon);
                ARCHIVEPLAY_LOG_ERROR("无法从设备表中获取设备信息");
                sprintf_s(record.errMsg,sizeof(record.errMsg),"无法从设备表中获取设备信息");
                return FALSE;
            }

            sql_row = mysql_fetch_row(result) ;
            if (NULL != sql_row[0])
            { 
                devinfo.type = atoi(sql_row[0]);
            }
            if (NULL != sql_row[1])
            { 
                sprintf_s(devinfo.ip, sizeof(devinfo.ip),"%s", sql_row[1]);
            }
            if (NULL != sql_row[2])
            { 
                devinfo.port = atoi(sql_row[2]);
            }
            if (NULL != sql_row[3])
            { 
                sprintf_s(devinfo.user, sizeof(devinfo.user),"%s", sql_row[3]);
            }


            int dst_len = sizeof(devinfo.password);
            char* outl = (char*)&dst_len;
            if (NULL != sql_row[4])
            { 
                if (Hak9(sql_row[4],NULL,NULL))
                {
                    if(Hak7(devinfo.password, sql_row[4],outl) != 0)// 解密
                    {
                        mysql_free_result(result);  
                        result = NULL;
                        sqlp->ReleaseConnection(sqlcon);
                        sprintf_s(record.errMsg,sizeof(record.errMsg),"解密失败,无法登陆存储");
                        return FALSE;
                    }
                }
                else
                {
                    strcpy_s(devinfo.password,sizeof(devinfo.password),sql_row[4]);
                } 
            }

        } 
    }
    else
    {
        TRACE("%s\n",mysql_error(sqlcon));
        ARCHIVEPLAY_LOG_ERROR("%s",mysql_error(sqlcon));
        sprintf_s(record.errMsg,sizeof(record.errMsg),"%s\n",mysql_error(sqlcon));
    }

    if(NULL != result)
    {   
        mysql_free_result(result); //释放结果资源 
        result = NULL;
    }

    sqlp->ReleaseConnection(sqlcon);
    return TRUE;

}



int operateDBV21::getCamerainfo(RecordInfo &record)
{
    char sqlbuf[BUFFERSIZE]={0};

    MYSQL_RES *result =NULL; 
    MYSQL_ROW sql_row; 
    MYSQL *sqlcon = sqlp->GetConnection();
    if(sqlcon == NULL)
    {
        coding eCode;
        ARCHIVEPLAY_LOG_ERROR("无法获取数据库连接:%s",eCode.UnicodeToAnsi(sqlp->str_error.c_str()));
        sprintf_s(record.errMsg,sizeof(record.errMsg),"与数据库连接断开");
        m_Info.pErrCallback(-1,eCode.UnicodeToAnsi(sqlp->str_error.c_str()),m_Info.pData);
        sqlp->ReleaseConnection(sqlcon);   
        return FALSE;
    }
    DevicInfo &devinfo = record.camera;
    sprintf_s(sqlbuf,sizeof(sqlbuf),"select nType,strIp,nPort,strUser,strPassword from Device where id = '%d'",record.devid);
    int  res = mysql_query(sqlcon,sqlbuf);

    if(0 == res)  
    { 
        result=mysql_store_result(sqlcon);
        if(NULL != result) 
        { 
            my_ulonglong nrows = mysql_num_rows(result);
            if (0 == nrows)
            {
                mysql_free_result(result); 
                sqlp->ReleaseConnection(sqlcon);
                sprintf_s(record.errMsg,sizeof(record.errMsg),"数据库Device表里无设备信息");
                return FALSE;
            }
            // my_ulonglong ncols = mysql_num_fields(result); 
            sql_row = mysql_fetch_row(result) ;
            if (NULL != sql_row[0])
            { 
                devinfo.type = atoi(sql_row[0]);
            }
            if (NULL != sql_row[1])
            { 
                sprintf_s(devinfo.ip, sizeof(devinfo.ip),"%s", sql_row[1]);
            }
            if (NULL != sql_row[2])
            { 
                devinfo.port = atoi(sql_row[2]);
            }
            if (NULL != sql_row[3])
            { 
                sprintf_s(devinfo.user, sizeof(devinfo.user),"%s", sql_row[3]);
            }
            //memset(record.streamNo,0,sizeof(record.streamNo));
            //sprintf_s(record.streamNo,sizeof(record.streamNo),"%s_%d_%d",devinfo.ip,devinfo.port,record.channel);
            int dst_len = sizeof(devinfo.password);
            char* outl = (char*)&dst_len;
            if (NULL != sql_row[4])
            { 
                if (Hak9(sql_row[4],NULL,NULL))
                {
                    if(Hak7(devinfo.password, sql_row[4],outl) != 0)// 解密
                    {
                        mysql_free_result(result);  
                        result = NULL;
                        sqlp->ReleaseConnection(sqlcon);
                        sprintf_s(record.errMsg,sizeof(record.errMsg),"解密失败,将无法登陆");
                        ARCHIVEPLAY_LOG_ERROR("解密失败,原密码:%s",sql_row[4]);
                        return -1;
                    }
                }
                else
                {
                    strcpy_s(devinfo.password,sizeof(devinfo.password),sql_row[4]);
                } 
            }
            //memcpy(&record.camera,&devinfo,sizeof(devinfo));

        } 
    }
    else
    {
        TRACE("%s\n",mysql_error(sqlcon));
        ARCHIVEPLAY_LOG_ERROR("%s",mysql_error(sqlcon));
        sprintf_s(record.errMsg,sizeof(record.errMsg),"%s\n",mysql_error(sqlcon));
    }

    if(NULL != result)
    {   
        mysql_free_result(result); //释放结果资源 
        result = NULL;
    }

    sqlp->ReleaseConnection(sqlcon);
    return 0;
}


void operateDBV21::searchBusinessForRecord(RecordInfo& pRecordinfo)
{
    std::map<int,BusinessInfo>::iterator itb = BusinessInfolist.begin();

    while(itb != BusinessInfolist.end())
    { 

        if (strcmp(itb->second.number,pRecordinfo.reserve) == 0)
        {
            pRecordinfo.Businessid = itb->first;

            return;
        }
        itb++;
    }
}


void operateDBV21::getRecordInfo(const NET_DVR_TIME &StartDayTime, const NET_DVR_TIME &StopDayTime)
{
    RecordInfoList.clear();
    BusinessInfolist.clear();
    searchFormRecords(StartDayTime,StopDayTime);  
    searchBusiness();

    for(std::size_t i = 0;i<RecordInfoList.size();++i)
    {

        getStoreInfo(RecordInfoList.at(i));
        getCamerainfo(RecordInfoList.at(i));      

    }

}

void operateDBV21::getDeviceInfoFromDB()
{
    g_Devlist.clear();

    char sqlbuf[BUFFERSIZE]={0};
    MYSQL_RES *result =NULL; 
    MYSQL_ROW sql_row; 
    MYSQL *sqlcon = sqlp->GetConnection();
    if (NULL == sqlcon)
    {
        coding eCode;    
        ARCHIVEPLAY_LOG_ERROR("%s",eCode.UnicodeToAnsi(sqlp->str_error.c_str()));      
        sqlp->ReleaseConnection(sqlcon);   
        return;
    }
    DevicInfo devinfo;
    sprintf_s(sqlbuf,sizeof(sqlbuf),"select nType,strName,strIp,nPort,strUser,strPassword from Device where nType = 50000 or nType = 50012");
    int res = mysql_query(sqlcon,sqlbuf);
    ARCHIVEPLAY_LOG_INFO("查找设备信息");
    if(0 == res) 
    { 
        result=mysql_store_result(sqlcon);
        if(NULL != result) 
        { 
            my_ulonglong nrows = mysql_num_rows(result);
            if (0 != nrows)
            {
                while(sql_row=mysql_fetch_row(result)) 
                { 
                    if (NULL != sql_row[0])
                    {               
                        devinfo.type = atoi(sql_row[0]);
                    }
                    if (NULL != sql_row[1])
                    { 
                        sprintf_s(devinfo.name, sizeof(devinfo.name),"%s", sql_row[1]);
                    }
                    if (NULL != sql_row[2])
                    { 
                        sprintf_s(devinfo.ip, sizeof(devinfo.ip),"%s", sql_row[2]);
                    }
                    if (NULL != sql_row[3])
                    { 
                        devinfo.port = atoi(sql_row[3]);
                    }
                    if (NULL != sql_row[4])
                    { 
                        sprintf_s(devinfo.user, sizeof(devinfo.user),"%s", sql_row[4]);
                    }
                    int dst_len = sizeof(devinfo.password);
                    char* outl = (char*)&dst_len;
                    if (NULL != sql_row[5])
                    { 
                        if (Hak9(sql_row[5],NULL,NULL))
                        {
                            if(Hak7(devinfo.password, sql_row[5],outl) != 0)// 解密
                            {
                                devinfo.bpwdEnable = FALSE;
                            }
                            else
                            {
                                devinfo.bpwdEnable = TRUE;
                            }

                        }
                        else
                        {
                            devinfo.bpwdEnable = TRUE;
                            strcpy_s(devinfo.password,sizeof(devinfo.password),sql_row[5]);
                        } 
                    }

                    g_Devlist.push_back(devinfo);
                }
            }
            //my_ulonglong ncols = mysql_num_fields(result); 



        } 
    }
    else
    {
        TRACE("%s\n",mysql_error(sqlcon));
        ARCHIVEPLAY_LOG_ERROR("%s",mysql_error(sqlcon));
        ARCHIVEPLAY_LOG_ERROR("<%s>查找失败",sqlbuf);

    }

    if(NULL != result)
    {   
        mysql_free_result(result); //释放结果资源 
        result = NULL;
    }

    sqlp->ReleaseConnection(sqlcon);
}
void operateDBV21::getDeviceInfo()
{
    getDeviceInfoFromDB();
}