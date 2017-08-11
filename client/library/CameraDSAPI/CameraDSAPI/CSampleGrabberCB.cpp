#include "CSampleGrabberCB.h"
#include "hlogConfig.h"

CSampleGrabberCB::CSampleGrabberCB():
m_pUser(NULL)
,m_Timestamp_video(0)
,m_Timestamp_audio(0)
{

}

CSampleGrabberCB::~CSampleGrabberCB()
{

}

long CSampleGrabberCB::Init()
{
    return 0;
}

long CSampleGrabberCB::Fini()
{
    return 0;
}

long CSampleGrabberCB::SetDataCB(pMediaDataBack phTrans, CRITICAL_SECTION* pcs, void* pUser)
{
    m_phTrans = phTrans;
    m_pcs = pcs;
    m_pUser = pUser;
    return 0;
    //return SYSTRANS_RegisterOutputDataCallBack(m_phTrans, datafun, pUser);
}

//long CSampleGrabberCB::Run()
//{
//    return SYSTRANS_Start(m_phTrans, NULL, NULL);
//}

//long CSampleGrabberCB::Stop()
//{
//    return SYSTRANS_Stop(m_phTrans);
//}

STDMETHODIMP CSampleGrabberCB::BufferCB(double Time, BYTE *pBuffer, long BufferLen)
{
    //long lRet = 0;
    //static FILE *jpgFile1 = fopen("d:\\vedio", "wb+");
    //fwrite(pBuffer, BufferLen, 1, jpgFile1);
    //EnterCriticalSection(m_pcs);
    //lRet = SYSTRANS_InputData(m_phTrans, MULTI_DATA, pBuffer, BufferLen);
    //LeaveCriticalSection(m_pcs); 
    //if (SYSTRANS_OK != lRet)
    //{
    //    TPLOG_ERROR("SYSTRANS_InputData failed:%d",lRet);
    //}
    m_phTrans(pBuffer, BufferLen, m_pUser);
    return E_NOTIMPL;
}