#pragma once

//////////////////////////////////////////////////////////////////////////
// WorkItem
class WorkItem
{
public:
    virtual ~WorkItem(void){};

    virtual BOOL UnderTake(void) = 0;
};

//////////////////////////////////////////////////////////////////////////
// TestWorkItem
class TestWorkItem : public WorkItem
{
public:
    TestWorkItem(void)
    {
        m_nSeq = nSeq++;
    }

    virtual ~TestWorkItem(void){};

    static int nSeq;
    int m_nSeq;

    virtual BOOL UnderTake(void)
    {
        CLIENT_INFO("TestWorkItem %d Undertaken", m_nSeq);
        //Sleep(50);
        return TRUE;
    };
};