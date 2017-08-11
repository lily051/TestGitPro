#ifndef __KEY_BORD_DATA_H__
#define __KEY_BORD_DATA_H__

#pragma once

#include "stdafx.h"

#pragma warning(push)
#pragma warning(disable:4244)
CString ResolutionKeyFlag(int nKeyFlag)
{   
    CString strKeyDesc;
    if (nKeyFlag & (1<<16))
    {
        strKeyDesc = "Shift + ";
        nKeyFlag -= 1<<16;
    }
    else if (nKeyFlag & (1<<17))
    {
        strKeyDesc = "Ctrl + ";
        nKeyFlag -= 1<<17;
    }
    else if (nKeyFlag & (1<<18))
    {
        strKeyDesc = "Alt + ";
        nKeyFlag -= 1<<18;
    }
    else if (nKeyFlag & (1<<19))
    {
        strKeyDesc = "Win + ";
        nKeyFlag -= 1<<19;
    }
    else
    {
        strKeyDesc = "";
    }
    char ch = 0 ;
    int nKeyVal = nKeyFlag;

    if (VK_SHIFT == nKeyVal)
    {
        strKeyDesc = "Shift";
        return strKeyDesc;
    }
    if (VK_CONTROL == nKeyVal)
    {
        strKeyDesc = "Ctrl";
        return strKeyDesc;
    }
    if (VK_CAPITAL == nKeyVal)
    {
        strKeyDesc = "Caps";
        return strKeyDesc;
    }
    if (VK_MENU == nKeyVal)
    {
        strKeyDesc = "Alt";
        return strKeyDesc;
    }

    if (VK_F1 <= nKeyVal && nKeyVal <= VK_F12)
    {
        strKeyDesc.Format(_T("%sF%d"), strKeyDesc.GetString(), nKeyVal-VK_F1+1);
        return strKeyDesc;
    }

    if (VK_BACK == nKeyVal)
    {
        strKeyDesc = "Backspace";
        return strKeyDesc;
    }
    if (VK_ESCAPE == nKeyVal)
    {
        strKeyDesc = "Esc";
        return strKeyDesc;
    }
    if (VK_SPACE == nKeyVal)
    {
        strKeyDesc = "Space";
        return strKeyDesc;
    }

    if(nKeyVal==9) //TAB
    {
        strKeyDesc += "Tab";
        return strKeyDesc;
    }

    if(nKeyVal==13) //回车键
    {
        strKeyDesc += "Enter";
        return strKeyDesc;
    }

    if(nKeyVal>=48 && nKeyVal<=57) //数字键0-9
    {
        ch = nKeyVal;
        strKeyDesc += ch;
        return strKeyDesc;
    }

    if(nKeyVal>=65 && nKeyVal<=90) //A-Z a-z
    {
        ch=nKeyVal;
        strKeyDesc += ch;
        return strKeyDesc;
    }

    if(nKeyVal>=96 && nKeyVal<=105) //小键盘0-9
    {
        ch = nKeyVal-96+48;
        strKeyDesc += ch;
        return strKeyDesc;
    }

    if(nKeyVal==106)
    {
        ch = '*';
        strKeyDesc += ch;
        return strKeyDesc;
    }
    if(nKeyVal==107)
    {
        ch = '+';
        strKeyDesc += ch;
        return strKeyDesc;
    }
    if(nKeyVal==111)
    {
        ch = '/';
        strKeyDesc += ch;
        return strKeyDesc;
    }
    if(nKeyVal==109)
    {
        ch = '-';
        strKeyDesc += ch;
        return strKeyDesc;
    }
    if(nKeyVal==110)
    {
        ch = '.';
        strKeyDesc += ch;
        return strKeyDesc;
    }

    if(nKeyVal>=186 && nKeyVal<=222) //其它键
    {
        switch(nKeyVal)
        {
        case 186:
            ch=';' ;
            break;
        case 187:
            ch='=' ;
            break;
        case 188:
            ch=',' ;
            break;
        case 189:
            ch='-' ;
            break;
        case 190:
            ch='.' ;
            break;
        case 191:
            ch='/' ;
            break;
        case 192:
            ch='`' ;
            break;
        case 219:
            ch='[';
            break;
        case 220:
            ch='\\' ;
            break;
        case 221:
            ch=']';
            break;
        case 222:
            ch='\'';
            break;
        }
        strKeyDesc += ch;
        return strKeyDesc;
    } 
    return strKeyDesc;
}

CString TransformKeyToChar(int nKeyVal, int& nFlagKey)
{
    BOOL bShift = (GetKeyState(0x10)&0x8000);
    BOOL bCtrl = (GetKeyState(0x11)&0x8000);
    BOOL bAlt = (GetKeyState(0x12)&0x8000);
    BOOL bCapital = (GetKeyState(0x14)&1);
    BOOL bNumLock = (GetKeyState(0x90)&1);
    //bool bWin = false;
    //if (GetKeyState(0x5B)&0x8000 || GetKeyState(0x5C)&0x8000)
    //{
    //    bWin = true;
    //}

    CString strKeyDesc;
    char ch = 0 ;

    if (bShift)
    {
        strKeyDesc = "Shift + ";
        nFlagKey = 1<<16;
    }
    else if (bCtrl)
    {
        strKeyDesc = "Ctrl + ";
        nFlagKey = 1<<17;
    }
    else if (bAlt)
    {
        strKeyDesc = "Alt + ";
        nFlagKey = 1<<18;
    }
    //     else if (bWin)
    //     {
    //         strKeyDesc = "Win + ";
    //         nFlagKey = 1<<19;
    //     }
    else
    {
        strKeyDesc = "";
    }

    if (VK_F1 <= nKeyVal && nKeyVal <= VK_F12)
    {
        nFlagKey += nKeyVal;
        strKeyDesc.Format(_T("%sF%d"), strKeyDesc.GetString(), nKeyVal-VK_F1+1);
        return strKeyDesc;
    }

    if (VK_BACK == nKeyVal)
    {
        nFlagKey = nKeyVal;
        strKeyDesc = "Backspace";
        return strKeyDesc;
    }
    if (VK_ESCAPE == nKeyVal)
    {
        nFlagKey = nKeyVal;
        strKeyDesc = "Esc";
        return strKeyDesc;
    }
    if (VK_SPACE == nKeyVal)
    {
        nFlagKey = nKeyVal;
        strKeyDesc = "Space";
        return strKeyDesc;
    }

    if(nKeyVal==9) //TAB
    {
        nFlagKey += 9;
        strKeyDesc += "Tab";
        return strKeyDesc;
    }

    if(nKeyVal==13) //回车键
    {
        nFlagKey += 13;
        strKeyDesc += "Enter";
        return strKeyDesc;
    }

    if(nKeyVal>=48 && nKeyVal<=57) //数字键0-9
    {
        ch = nKeyVal;
        nFlagKey += nKeyVal;
        strKeyDesc += ch;
        return strKeyDesc;
    }

    if(nKeyVal>=65 && nKeyVal<=90) //A-Z a-z
    {
        nFlagKey += nKeyVal;
        ch=nKeyVal;
        strKeyDesc += ch;
        return strKeyDesc;
    }

    if(nKeyVal>=96 && nKeyVal<=105) //小键盘0-9
    {
        if(bNumLock)
        {
            ch = nKeyVal-96+48;
            nFlagKey += nKeyVal;
            strKeyDesc += ch;
            return strKeyDesc;
        }
    }

    if(nKeyVal==106)
    {
        ch = '*';
        nFlagKey += nKeyVal;
        strKeyDesc += ch;
        return strKeyDesc;
    }
    if(nKeyVal==107)
    {
        ch = '+';
        nFlagKey += nKeyVal;
        strKeyDesc += ch;
        return strKeyDesc;
    }
    if(nKeyVal==111)
    {
        ch = '/';
        nFlagKey += nKeyVal;
        strKeyDesc += ch;
        return strKeyDesc;
    }
    if(nKeyVal==109)
    {
        ch = '-';
        nFlagKey += nKeyVal;
        strKeyDesc += ch;
        return strKeyDesc;
    }
    if(nKeyVal==110)
    {
        if(bNumLock)
        {
            ch = '.';
        }
        nFlagKey += nKeyVal;
        strKeyDesc += ch;
        return strKeyDesc;
    }

    if(nKeyVal>=186 && nKeyVal<=222) //其它键
    {
        switch(nKeyVal)
        {
        case 186:
            ch=';' ;
            break;
        case 187:
            ch='=' ;
            break;
        case 188:
            ch=',' ;
            break;
        case 189:
            ch='-' ;
            break;
        case 190:
            ch='.' ;
            break;
        case 191:
            ch='/' ;
            break;
        case 192:
            ch='`' ;
            break;
        case 219:
            ch='[';
            break;
        case 220:
            ch='\\' ;
            break;
        case 221:
            ch=']';
            break;
        case 222:
            ch='\'';
            break;
        }
        nFlagKey += nKeyVal;
        strKeyDesc += ch;
        return strKeyDesc;
    } 

    if (bShift)
    {
        strKeyDesc = "Shift";
        nFlagKey = VK_SHIFT;
    }else if (bCtrl)
    {
        strKeyDesc = "Ctrl";
        nFlagKey = VK_CONTROL;
    }
    else if (bAlt)
    {
        strKeyDesc = "Alt";
        nFlagKey = VK_MENU;
    }
    else if (bCapital)
    {
        strKeyDesc = "Caps";
        nFlagKey = VK_CAPITAL;
    }
    //     else if (bWin)
    //     {
    //         strKeyDesc = "Win";
    //     }
    else
    {
        strKeyDesc = "Enter";
        nFlagKey = VK_RETURN;
    }
    return strKeyDesc;
}

#pragma warning(pop)



#endif
