/*
    Copyright 2016-2019 Arisotura

    This file is part of melonDS.

    melonDS is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    melonDS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with melonDS. If not, see http://www.gnu.org/licenses/.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "libui/ui.h"

#include "../types.h"
#include "PlatformConfig.h"

#include "DlgEmuSettings.h"


void ApplyNewSettings(int type);


namespace DlgEmuSettings
{

bool opened;
uiWindow* win;

uiCheckbox* cbDirectBoot;
uiEntry* eRTCOffset;

int OnCloseWindow(uiWindow* window, void* blarg)
{
    opened = false;
    return 1;
}

void OnCancel(uiButton* btn, void* blarg)
{
    uiControlDestroy(uiControl(win));
    opened = false;
}

void OnOk(uiButton* btn, void* blarg)
{
    Config::DirectBoot = uiCheckboxChecked(cbDirectBoot);
    
    int offset = std::stoi(uiEntryText(eRTCOffset));
    Config::RTCOffset = offset;
    
    Config::Save();

    uiControlDestroy(uiControl(win));
    opened = false;
}

void OnRtcEntryChanged(uiEntry* entry, void* blarg)
{
	int offset = std::stoi(uiEntryText(eRTCOffset));
	Config::RTCOffset = offset;
}

void Open()
{
    if (opened)
    {
        uiControlSetFocus(uiControl(win));
        return;
    }

    opened = true;
    win = uiNewWindow("Emu settings - melonDS", 300, 200, 0, 0, 0);
    uiWindowSetMargined(win, 1);
    uiWindowOnClosing(win, OnCloseWindow, NULL);

    uiBox* top = uiNewVerticalBox();
    uiWindowSetChild(win, uiControl(top));

    {
        uiBox* in_ctrl = uiNewVerticalBox();
        uiBoxAppend(top, uiControl(in_ctrl), 1);

        cbDirectBoot = uiNewCheckbox("Boot game directly");
        uiBoxAppend(in_ctrl, uiControl(cbDirectBoot), 0);
        
        {
	        uiBox* rtc_box = uiNewHorizontalBox();
	        uiBoxSetPadded(rtc_box, 1);
	        uiBoxAppend(in_ctrl, uiControl(rtc_box), 1);
	
	        uiLabel* lRTC = uiNewLabel("RTC Offset: ");
	        uiBoxAppend(rtc_box, uiControl(lRTC), 0);
	        
	        eRTCOffset = uiNewEntry();
	        std::string str = std::to_string(Config::RTCOffset);
	        uiEntrySetText(eRTCOffset, str.c_str());
	        uiEntryOnChanged(eRTCOffset, OnRtcEntryChanged, NULL);
	        uiBoxAppend(rtc_box, uiControl(eRTCOffset), 0);
	        
        }
    }

    {
        uiBox* in_ctrl = uiNewHorizontalBox();
        uiBoxSetPadded(in_ctrl, 1);
        uiBoxAppend(top, uiControl(in_ctrl), 0);

        uiLabel* dummy = uiNewLabel("");
        uiBoxAppend(in_ctrl, uiControl(dummy), 1);

        uiButton* btncancel = uiNewButton("Cancel");
        uiButtonOnClicked(btncancel, OnCancel, NULL);
        uiBoxAppend(in_ctrl, uiControl(btncancel), 0);

        uiButton* btnok = uiNewButton("Ok");
        uiButtonOnClicked(btnok, OnOk, NULL);
        uiBoxAppend(in_ctrl, uiControl(btnok), 0);
    }

    uiCheckboxSetChecked(cbDirectBoot, Config::DirectBoot);

    uiControlShow(uiControl(win));
}

void Close()
{
    if (!opened) return;
    uiControlDestroy(uiControl(win));
    opened = false;
}

}
