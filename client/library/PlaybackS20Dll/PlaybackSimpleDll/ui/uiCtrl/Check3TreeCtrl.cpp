// Check3TreeCtrl.cpp : ʵ���ļ�
//
//lint -library
#include "stdafx.h"
#include "Check3TreeCtrl.h"


// CCheck3TreeCtrl

IMPLEMENT_DYNAMIC(CCheck3TreeCtrl, CTreeCtrl)

CCheck3TreeCtrl::CCheck3TreeCtrl()
{
	m_nHitFlag = 0;
	m_hHitItem = 0;
}

CCheck3TreeCtrl::~CCheck3TreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CCheck3TreeCtrl, CTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, &CCheck3TreeCtrl::OnNMClick)
END_MESSAGE_MAP()



// CCheck3TreeCtrl ��Ϣ�������


BOOL CCheck3TreeCtrl::SetCheck(HTREEITEM hItem, int nCheck /*= 1*/)
{
	ASSERT(::IsWindow(m_hWnd));

	if (nCheck < 0 || nCheck > 2)
		nCheck = 2;

	TVITEM item;
	item.mask = TVIF_HANDLE | TVIF_STATE;
	item.hItem = hItem;
	item.stateMask = TVIS_STATEIMAGEMASK;

	// state images are one-based
	item.state = INDEXTOSTATEIMAGEMASK(nCheck + 1);

	return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)&item);
}

int CCheck3TreeCtrl::GetCheck(HTREEITEM hItem) const
{
	ASSERT(::IsWindow(m_hWnd));

	TVITEM item;
	item.mask = TVIF_HANDLE | TVIF_STATE;
	item.hItem = hItem;
	item.stateMask = TVIS_STATEIMAGEMASK;
	VERIFY(::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item));

	// return 0, 1, 2.
	int nCheck = (item.state >> 12) -1;
	if (nCheck < 0 || nCheck > 2)
		nCheck = 2;

	return nCheck;
}

void CCheck3TreeCtrl::UpdateParentState(HTREEITEM hItem, int nCheck)
{
	ASSERT(hItem != NULL);

	HTREEITEM hParent = GetParentItem(hItem);
	if (hParent == NULL)
		return;

	//����ǵ���̬�����ڵ�϶�Ҳ�ǵ���̬�������ж�.
	if (nCheck != 2)
	{
		HTREEITEM hSibling = GetPrevSiblingItem(hItem);
		while (hSibling)
		{
			if (GetCheck(hSibling) != nCheck)
			{
				nCheck = 2;
				break;
			}

			hSibling = GetPrevSiblingItem(hSibling);
		}
	}

	if (nCheck != 2)
	{
		HTREEITEM hSibling = GetNextSiblingItem(hItem);
		while (hSibling)
		{
			if (GetCheck(hSibling) != nCheck)
			{
				nCheck = 2;
				break;
			}

			hSibling = GetNextSiblingItem(hSibling);
		}
	}

	//�޸�״̬
	SetCheck(hParent, nCheck);
	UpdateParentState(hParent, nCheck);
}

void CCheck3TreeCtrl::UpdateChildrenState(HTREEITEM hItem, int nCheck)
{
	ASSERT(hItem != NULL);

	//����ǵ���̬���������ӽڵ�
	if (nCheck == 0 || nCheck == 1)
	{
		HTREEITEM hChild = GetChildItem(hItem);
		while (hChild)
		{
			SetCheck(hChild, nCheck);
			UpdateChildrenState(hChild, nCheck);

			hChild = GetNextSiblingItem(hChild);
		}
	}
}

void CCheck3TreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_hHitItem = HitTest(point, &m_nHitFlag);

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CCheck3TreeCtrl::OnNMClick(NMHDR * /*pNMHDR*/, LRESULT * pResult)
{
	*pResult = 0;

	if ((m_hHitItem != NULL) && (m_nHitFlag & TVHT_ONITEMSTATEICON))
	{
		int nNewCheck = !GetCheck(m_hHitItem);

		//���±��ڵ�
		SetCheck(m_hHitItem, nNewCheck);

		//�����ӽڵ�͸��ڵ�
		UpdateChildrenState(m_hHitItem, nNewCheck);
		UpdateParentState(m_hHitItem, nNewCheck);

		//��Ϣ�Ѿ�����
		*pResult = 1;
	}

	m_nHitFlag = 0;
	m_hHitItem = 0;
}
