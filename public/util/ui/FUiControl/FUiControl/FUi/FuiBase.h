/**   @file		FuiBase.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 
 *
 *    @author	jddd
 *    @date		2015/09/22
 *
 *    @note 	
 *    @note 	
 *    @note 	
 *
 *    @warning 	
 */
#pragma once
#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#include "./ToolTip/PPTooltip.h"

#define SAFE_DELETE_IMAGE(p) if(p) {delete (p); (p)=0;}
#define SAFE_DELETE_FONT(p) if(p.m_hObject) {(p).DeleteObject();}
#define SAFE_DELETE_BRUSH(p) if(p.m_hObject) {(p).DeleteObject();}

// ��������
#define SONG_12_N       _T("Song12N")       // ����, 12, NORMAL
#define SONG_12_SB      _T("Song12SB")      // ����, 12, SEMIBOLD
#define SONG_16_B       _T("Song16B")       // ����, 16, BOLD
#define ARIAL_15_N      _T("Arial15N")      // Arial, 15, NORMAL
#define YAHEI_18_N      _T("Yahei18N")      // �ź�, 18, NORMAL
#define YAHEI_18_B      _T("Yahei18B")      // �ź�, 18, Bold
#define YAHEI_18_SB     _T("Yahei18SB")     // �ź�, 18, SEMIBOLD
#define YAHEI_20_N      _T("Yahei20N")      // �ź�, 20, NORMAL
#define YAHEI_20_SB     _T("Yahei20SB")     // �ź�, 20, SEMIBOLD
#define YAHEI_20_B		_T("Yahei20B")		// �ź�, 20, BOLD
#define YAHEI_26_SB     _T("Yahei26SB")     // �ź�, 26, SEMIBOLD
#define YAHEI_24_B     _T("Yahei24B")     // �ź�, 24, SEMIBOLD
#define YAHEI_24_SB     _T("Yahei24SB")     // �ź�, 24, SEMIBOLD
#define YAHEI_26_N      _T("Yahei26N")      // �ź�, 26, NORMAL
#define YAHEI_26_SB     _T("Yahei26SB")     // �ź�, 6, SEMIBOLD

#define MAX_LEN_TOOLTIP	128

namespace FUi
{
    // ������ɫ
    enum fui_color
    {
        White = RGB(255, 255, 255),
        Black = RGB(0, 0, 0),
        Gray = RGB(80, 80, 80),
		Blue = RGB(63, 133, 228),
		LightGray = RGB(158, 158, 158),
        Red = RGB(255, 0, 0),
		Pink = RGB(226, 75, 81),
        BurlyWood = RGB(144, 238, 144),
        RGB_102 = RGB(102, 102, 102),
        RGB_197 = RGB(197, 197, 197),
        RGB_204 = RGB(204, 204, 204),
        RGB_213 = RGB(213, 213, 213),
        RGB_222 = RGB(222, 222, 222),
        RGB_234 = RGB(234, 234, 234),
        RGB_241 = RGB(241, 241, 241),
        RGB_242 = RGB(242, 242, 242),
        RGB_251 = RGB(251, 251, 251),
        EDIT_FOCUS = RGB(102, 102, 102),
        LISTHEAD_BG = RGB(102, 122, 147),
    };

    // �ؼ�״̬
    enum ctrl_state
    {
        Normal      = 0x1,  // ��ͨ״̬
        Hovered     = 0x2,  // ��ͣ״̬
        Pressed     = 0x4,  // ����״̬
        Disabled    = 0x8,  // �û�״̬

        Focus       = 0x10, // ����״̬
        Error       = 0x20, // ��ʾ����
		Default     = 0x40, // Ĭ��״̬
    };
}

//pclint +e528