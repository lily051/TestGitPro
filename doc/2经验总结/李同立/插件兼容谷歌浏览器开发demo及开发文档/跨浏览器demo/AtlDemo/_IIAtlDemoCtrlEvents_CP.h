#pragma once

template<class T>
class CProxy_IIAtlDemoCtrlEvents :
	public IConnectionPointImpl<T, &__uuidof(_IIAtlDemoCtrlEvents)>
{
public:
	HRESULT Fire_MsgNotify( LONG lMsgId,  BSTR szDetail)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

        OutputPrintf("Fire_MsgNotify:%d", cConnections);
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[2];
				avarParams[1] = lMsgId;
				avarParams[1].vt = VT_I4;
				avarParams[0] = szDetail;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

                char* lpszText1 = _com_util::ConvertBSTRToString(szDetail);
                OutputPrintf("Fire_MsgNotify lch1 : lMsgId:%d, szDetail:%s", lMsgId, lpszText1);
				DISPPARAMS params = { avarParams, NULL, 2, 0 };
                hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
                OutputPrintf("Fire_MsgNotify lch2 : lMsgId:%d, szDetail:%s", lMsgId, lpszText1);
                delete[] lpszText1;
			}
		}
		return hr;
	}
};

