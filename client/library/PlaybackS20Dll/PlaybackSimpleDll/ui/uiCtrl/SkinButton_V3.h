#pragma once

#include "SkingButton_V2.h"
// CSkinButton_V3

class CSkinButton_V3 : public CSkingButton_V2
{
	DECLARE_DYNAMIC(CSkinButton_V3)

public:
	CSkinButton_V3();
	virtual ~CSkinButton_V3();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


