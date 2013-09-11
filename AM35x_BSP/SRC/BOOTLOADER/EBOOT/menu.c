// All rights reserved ADENEO EMBEDDED 2010
// Copyright (c) 2007, 2008 BSQUARE Corporation. All rights reserved.

/*
================================================================================
*             Texas Instruments OMAP(TM) Platform Software
* (c) Copyright Texas Instruments, Incorporated. All Rights Reserved.
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied.
*
================================================================================
*/
//
//  File:  menu.c
//
#include <eboot.h>

//------------------------------------------------------------------------------
//
//  Define:  dimof
//
#ifdef dimof
#undef dimof
#endif
#define dimof(x)                (sizeof(x)/sizeof(x[0]))

//------------------------------------------------------------------------------

static VOID SetMacAddress(OAL_BLMENU_ITEM *pMenu);
static VOID ShowSettings(OAL_BLMENU_ITEM *pMenu);
static VOID ShowNetworkSettings(OAL_BLMENU_ITEM *pMenu);
static VOID SetKitlMode(OAL_BLMENU_ITEM *pMenu);
static VOID SetKitlType(OAL_BLMENU_ITEM *pMenu);
static VOID SetDeviceID(OAL_BLMENU_ITEM *pMenu);
static VOID SaveSettings(OAL_BLMENU_ITEM *pMenu);
static VOID SetRetailMsgMode(OAL_BLMENU_ITEM *pMenu);
static VOID SetDisplayResolution(OAL_BLMENU_ITEM *pMenu);
static VOID SetOPPmode(OAL_BLMENU_ITEM *pMenu);


#if BUILDING_EBOOT_SD
static VOID ShowSDCardSettings(OAL_BLMENU_ITEM *pMenu);
static VOID EnterSDCardFilename(OAL_BLMENU_ITEM *pMenu);
#endif

//------------------------------------------------------------------------------
// Variables
extern OAL_BLMENU_ITEM g_menuFlash[];


// Functions
extern BOOL LAN911XFindController(void* pAddr, DWORD * pdwID);
extern BOOL LAN911XSetMacAddress(void* pAddr, UINT16 mac[3]);
extern BOOL LAN911XGetMacAddress(void* pAddr, UINT16 mac[3]);


//------------------------------------------------------------------------------

static OAL_BLMENU_ITEM g_menuNetwork[] = {
    {
        L'1', L"Show Current Settings", ShowNetworkSettings,
        NULL, NULL, NULL
    }, {
        L'2', L"Enable/disable KITL", OALBLMenuEnable,
        L"KITL", &g_bootCfg.kitlFlags, (VOID*)OAL_KITL_FLAGS_ENABLED
    }, {
        L'3', L"KITL interrupt/poll mode", SetKitlMode,
        NULL, NULL, NULL
    }, {
        L'4', L"KITL Active/Passive mode", SetKitlType,
        NULL, NULL, NULL
    }, {
        L'5', L"Enable/disable DHCP", OALBLMenuEnable,
        L"DHCP", &g_bootCfg.kitlFlags, (VOID*)OAL_KITL_FLAGS_DHCP
    }, {
        L'6', L"Set IP address", OALBLMenuSetIpAddress,
        L"Device", &g_bootCfg.ipAddress, NULL
    }, {
        L'7', L"Set IP mask", OALBLMenuSetIpMask,
        L"Device", &g_bootCfg.ipMask, NULL
    }, {
        L'8', L"Set default router", OALBLMenuSetIpAddress,
        L"Default router", &g_bootCfg.ipRoute, NULL
    }, {
        L'9', L"Enable/disable VMINI", OALBLMenuEnable,
        L"VMINI", &g_bootCfg.kitlFlags, (VOID*)OAL_KITL_FLAGS_VMINI
    }, {
        L'a', L"Set MAC address", SetMacAddress,
        NULL, NULL, NULL
    }, {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, NULL
    }, {
        0, NULL, NULL,
        NULL, NULL, NULL
    }
};

#if BUILDING_EBOOT_SD
static OAL_BLMENU_ITEM g_menuSDCard[] = {
    {
        L'1', L"Show Current Settings", ShowSDCardSettings,
        NULL, NULL, NULL
    }, {
        L'2', L"Enter Filename", EnterSDCardFilename,
        NULL, NULL, NULL
    }, {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, NULL
    }, {
        0, NULL, NULL,
        NULL, NULL, NULL
    }
};
#endif

#if BUILDING_EBOOT_SD
static OAL_BLMENU_ITEM g_menuMain[] = {
    {
        L'1', L"Show Current Settings", ShowSettings,
        NULL, NULL, NULL
    }, {
        L'2', L"Select Boot Device", OALBLMenuSelectDevice,
        L"Boot", &g_bootCfg.bootDevLoc, g_bootDevices
    }, {
        L'3', L"Select KITL (Debug) Device", OALBLMenuSelectDevice,
        L"Debug", &g_bootCfg.kitlDevLoc, g_kitlDevices
    }, {
        L'4', L"Network Settings", OALBLMenuShow,
        L"Network Settings", &g_menuNetwork, NULL
    }, {
        L'5', L"SDCard Settings", OALBLMenuShow,
        L"SDCard Settings", &g_menuSDCard, NULL
    }, {
        L'6', L"Set Device ID", SetDeviceID,
        NULL, NULL, NULL
    }, {
        L'7', L"Save Settings", SaveSettings,
        NULL, NULL, NULL
    }, {
        L'8', L"Flash Management", OALBLMenuShow,
        L"Flash Management", &g_menuFlash, NULL
    },  {
        L'9', L"Enable/Disable OAL Retail Messages", SetRetailMsgMode,
        NULL, NULL, NULL
    }, {
        L'a', L"Select Display Resolution", SetDisplayResolution,
        NULL, NULL, NULL
    }, {
        L'b', L"Select OPP Mode", SetOPPmode,
        NULL, NULL, NULL
    }, {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, NULL
    }, {
        0, NULL, NULL,
        NULL, NULL, NULL
    }
};
#else
static OAL_BLMENU_ITEM g_menuMain[] = {
    {
        L'1', L"Show Current Settings", ShowSettings,
        NULL, NULL, NULL
    }, {
        L'2', L"Select Boot Device", OALBLMenuSelectDevice,
        L"Boot", &g_bootCfg.bootDevLoc, g_bootDevices
    }, {
        L'3', L"Select KITL (Debug) Device", OALBLMenuSelectDevice,
        L"Debug", &g_bootCfg.kitlDevLoc, g_kitlDevices
    }, {
        L'4', L"Network Settings", OALBLMenuShow,
        L"Network Settings", &g_menuNetwork, NULL
    }, {
        L'5', L"Flash Management", OALBLMenuShow,
        L"Flash Management", &g_menuFlash, NULL
    }, {
        L'6', L"Set Device ID", SetDeviceID,
        NULL, NULL, NULL
    }, {
        L'7', L"Save Settings", SaveSettings,
        NULL, NULL, NULL
    }, {
        L'8', L"Enable/Disable OAL Retail Messages", SetRetailMsgMode,
        NULL, NULL, NULL
    }, {
        L'9', L"Select Display Resolution", SetDisplayResolution,
        NULL, NULL, NULL
    }, {
        L'a', L"Select OPP Mode", SetOPPmode,
        NULL, NULL, NULL
    }, {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, NULL
    }, {
        0, NULL, NULL,
        NULL, NULL, NULL
    }
};
#endif

static OAL_BLMENU_ITEM g_menuRoot = {
    0, NULL, OALBLMenuShow,
    L"Main Menu", g_menuMain, NULL
};
typedef struct OMAP_LCD_DVI_RES_MENU {
    DWORD       lcdDviRes;
    LPCWSTR     resName;
} OMAP_LCD_DVI_RES_MENU;

OMAP_LCD_DVI_RES_MENU dispResMenu[] = {
    {OMAP_LCD_DEFAULT,          L"LCD  480x272 60Hz"},
    {OMAP_DVI_640W_480H,        L"DVI  640x480 60Hz"},
    {OMAP_DVI_640W_480H_72HZ,   L"DVI  640x480 72Hz"},
    {OMAP_DVI_800W_480H,        L"DVI  800x480 60Hz"},
    {OMAP_DVI_800W_600H,        L"DVI  800x600 60Hz"},
    {OMAP_DVI_800W_600H_56HZ,   L"DVI  800x600 56Hz"},
    {OMAP_DVI_1024W_768H,       L"DVI 1024x768 60Hz"},
    {OMAP_DVI_1280W_720H,       L"DVI 1280x720 60Hz"},
    {OMAP_RES_INVALID,          L"Invalid Entry"},
};

typedef struct OPP_MODE_MENU {
    LPCWSTR     oppModeName;
} OPP_MODE_MENU;

OPP_MODE_MENU oppModeMenu[AM35x_OPP_NUM] = {
    {L"MPU[125Mhz @ 1.2000V]"},
    {L"MPU[250Mhz @ 1.2000V]"},
    {L"MPU[500Mhz @ 1.2000V]"},
    {L"MPU[550Mhz @ 1.2000V]"},
    {L"MPU[600Mhz @ 1.2000V]"},
};


//------------------------------------------------------------------------------

VOID BLMenu(BOOL bForced)
{
    UINT32 time, delay = 5;
    WCHAR key = 0;
    BSP_ARGS *pArgs = OALCAtoUA(IMAGE_SHARE_ARGS_CA);
    
    // First let user break to menu
    while (!bForced && (delay > 0 && key != L' ')) {
       OALLog(L"Hit space to enter configuration menu %d...\r\n", delay);
       time = OALGetTickCount();
       while ((OALGetTickCount() - time) < 1000) {
          if ((key = OALBLMenuReadKey(FALSE)) == L' ') break;
       }
       delay--;
    }
    
    if ((bForced == TRUE) || (key == L' ')) 
    {
        OALBLMenuShow(&g_menuRoot);
        // Invalidate arguments to force them to be reinitialized
        // with new config data generated by the boot menu
        pArgs->header.signature = 0;
    }        
}

//------------------------------------------------------------------------------

VOID SetMacAddress(OAL_BLMENU_ITEM *pMenu)
{
    DWORD dummy;
    UINT16 mac[3] = {0xFFFF, 0xFFFF, 0xFFFF};

#pragma warning(push)
#pragma warning(disable:4204 4221)
    // We're using OALBLMenuSetMacAddress outside of a menu, but it still requires 
    // a menu item input structure
    OAL_BLMENU_ITEM AM35xMacMenuItem = 
    {
        0, NULL, NULL,
        L"AM35x", mac, NULL
    };
    OAL_BLMENU_ITEM ExternalLanMacMenuItem = 
    {
        0, NULL, NULL,
        L"9311", mac, NULL
    };
#pragma warning(pop)

    UNREFERENCED_PARAMETER(pMenu);

	// Set Internal EMAC Mac
    OALLog(L"\r\n Set Internal EMAC (LAN9xxx) MAC Address:\r\n");   
	
	memcpy(mac,g_bootCfg.mac,sizeof(mac));

    OALBLMenuSetMacAddress(&AM35xMacMenuItem);
    
    // Make sure we don't come out of OALBLMenuSetMacAddress with the original 0xFFFF FFFF FFFF or with all zeros
    if (((mac[0]== 0xFFFF) && (mac[1] == 0xFFFF) && (mac[2] == 0xFFFF)) || ((mac[0]== 0x0) && (mac[1] == 0x0) && (mac[2] == 0x0)))
    {
        OALLog(L"\r\n   WARNING: MAC address not programmed!\r\n");
        return;
    }
        
   memcpy(g_bootCfg.mac,mac,sizeof(mac));

    if (!LAN911XFindController(OALPAtoUA((DWORD) BSP_LAN9311_REGS_PA),&dummy))
    {     
        return;
    }

	// Set Internal EMAC Mac
    OALLog(L"\r\n Set External Lan (SAMSC9311) MAC Address:\r\n");   
		
    memset(mac,0xFF,sizeof(mac));
    LAN911XGetMacAddress(OALPAtoUA((DWORD) BSP_LAN9311_REGS_PA),mac);

    OALBLMenuSetMacAddress(&ExternalLanMacMenuItem);
    
    // Make sure we don't come out of OALBLMenuSetMacAddress with the original 0xFFFF FFFF FFFF or with all zeros
    if (((mac[0]== 0xFFFF) && (mac[1] == 0xFFFF) && (mac[2] == 0xFFFF)) || ((mac[0]== 0x0) && (mac[1] == 0x0) && (mac[2] == 0x0)))
    {
        OALLog(L"\r\n   WARNING: MAC address not programmed!\r\n");
        return;
    }
    LAN911XSetMacAddress(OALPAtoUA((DWORD) BSP_LAN9311_REGS_PA),mac);  
}

//------------------------------------------------------------------------------

VOID ShowSettings(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);

    OALLog(L"\r\n Main:\r\n");
    OALLog(
        L"  Boot device:   %s\r\n",
        OALKitlDeviceName(&g_bootCfg.bootDevLoc, g_bootDevices)
    );
    OALLog(
        L"  Debug device:  %s\r\n",
        (g_bootCfg.kitlDevLoc.LogicalLoc) ? OALKitlDeviceName(&g_bootCfg.kitlDevLoc, g_kitlDevices) : L"not specified"
    );
    OALLog(
        L"  Retail Msgs:   %s\r\n",
        (g_bootCfg.oalFlags & BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE) ? L"enabled" : L"disabled"
    );
    OALLog(
        L"  Device ID:     %d\r\n", g_bootCfg.deviceID
    );
    OALLog(
        L"  Display Res:   %s\r\n", dispResMenu[g_bootCfg.displayRes].resName
    );
    OALLog(
        L"  Flashing NK.bin:   %s\r\n", 
        (g_bootCfg.flashNKFlags & ENABLE_FLASH_NK) ? L"enabled" : L"disabled"
    );
     OALLog(
        L"  OPP Mode:   %s\r\n", oppModeMenu[g_bootCfg.opp_mode].oppModeName
    );
	
#if BUILDING_EBOOT_SD
    ShowSDCardSettings(pMenu);
#endif
    ShowNetworkSettings(pMenu);
}

//------------------------------------------------------------------------------

VOID ShowNetworkSettings(OAL_BLMENU_ITEM *pMenu)
{
    BOOL fValidExtMacAddr;
    UINT16 mac[3];
    UNREFERENCED_PARAMETER(pMenu);

    memset(mac,0xFF,sizeof(mac));
    LAN911XGetMacAddress(OALPAtoUA((DWORD) BSP_LAN9311_REGS_PA),mac);
    if (((mac[0]== 0xFFFF) && (mac[1] == 0xFFFF) && (mac[2] == 0xFFFF)) || ((mac[0]== 0x0) && (mac[1] == 0x0) && (mac[2] == 0x0)))
    {
        fValidExtMacAddr = FALSE;        
    }
    else
    {
        fValidExtMacAddr = TRUE;        
    }
    

    OALLog(L"\r\n Network:\r\n");
    OALLog(
        L"  KITL state:    %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_ENABLED) ? L"enabled" : L"disabled"
    );

    OALLog(
        L"  KITL type:     %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_PASSIVE) ? L"passive" : L"active"
    );

    OALLog(
        L"  KITL mode:     %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_POLL) ? L"polled" : L"interrupt"
    );
    OALLog(
        L"  DHCP:          %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_DHCP) ? L"enabled" : L"disabled"
    );
    OALLog(L"  IP address:    %s\r\n", OALKitlIPtoString(g_bootCfg.ipAddress));
    OALLog(L"  IP mask:       %s\r\n", OALKitlIPtoString(g_bootCfg.ipMask));
    OALLog(L"  IP router:     %s\r\n", OALKitlIPtoString(g_bootCfg.ipRoute));
    OALLog(L"  Internal Eth MAC Addr:  %s\r\n", OALKitlMACtoString(g_bootCfg.mac));
    OALLog(L"  External Eth MAC Addr:  %s\r\n", fValidExtMacAddr ? OALKitlMACtoString(mac) : L"Not programmed");
    OALLog(
        L"  VMINI:         %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_VMINI) ? L"enabled" : L"disabled"
    );
    OALLog(L"  Note: USBFN RNDIS MAC Addr cannot be changed.\r\n");
    
    OALBLMenuReadKey(TRUE);
}

//------------------------------------------------------------------------------

VOID SetKitlMode(OAL_BLMENU_ITEM *pMenu)
{
    WCHAR key;
    UNREFERENCED_PARAMETER(pMenu);

    if ((g_bootCfg.kitlFlags & OAL_KITL_FLAGS_POLL) != 0) {
        OALLog(L" Set KITL to interrupt mode [y/-]: ");
    } else {
        OALLog(L" Set KITL to polled mode [y/-]: ");
    }    

    // Get key
    key = OALBLMenuReadKey(TRUE);
    OALLog(L"%c\r\n", key);

    if (key == L'y' || key == L'Y') {
        if ((g_bootCfg.kitlFlags & OAL_KITL_FLAGS_POLL) != 0) {
            g_bootCfg.kitlFlags &= ~OAL_KITL_FLAGS_POLL;
            OALLog(L" KITL set to interrupt mode\r\n");
        } else {
            g_bootCfg.kitlFlags |= OAL_KITL_FLAGS_POLL;
            OALLog(L" KITL set to polled mode\r\n");
        }            
    }
}

//------------------------------------------------------------------------------

VOID
SetKitlType(
    OAL_BLMENU_ITEM *pMenu
    )
{
    WCHAR key;
    UNREFERENCED_PARAMETER(pMenu);

    if ((g_bootCfg.kitlFlags & OAL_KITL_FLAGS_PASSIVE) != 0) 
        {
        OALLog(L" Set KITL to active mode [y/-]: ");
        } 
   else 
        {
        OALLog(L" Set KITL to passive mode [y/-]: ");
        }    

    // Get key
    key = OALBLMenuReadKey(TRUE);
    OALLog(L"%c\r\n", key);

    if (key == L'y' || key == L'Y') 
        {
        if ((g_bootCfg.kitlFlags & OAL_KITL_FLAGS_PASSIVE) != 0) 
            {
            g_bootCfg.kitlFlags &= ~OAL_KITL_FLAGS_PASSIVE;
            OALLog(L" KITL set to active mode\r\n");
            }
        else 
            {
            g_bootCfg.kitlFlags |= OAL_KITL_FLAGS_PASSIVE;
            OALLog(L" KITL set to passive mode\r\n");
            }            
        }
}

//------------------------------------------------------------------------------

VOID 
SetDeviceID(
    OAL_BLMENU_ITEM *pMenu
    )
{
    WCHAR  szInputLine[16];
    UNREFERENCED_PARAMETER(pMenu);

    OALLog(
        L" Current Device ID:  %d\r\n", g_bootCfg.deviceID
    );

    OALLog(L"\r\n New Device ID: ");

    if (OALBLMenuReadLine(szInputLine, dimof(szInputLine)) == 0) 
        {
        goto cleanUp;
        }

    // Get device ID
    g_bootCfg.deviceID = OALStringToUINT32(szInputLine);

cleanUp:
    return;
}

//------------------------------------------------------------------------------

VOID SetRetailMsgMode(OAL_BLMENU_ITEM *pMenu)
{
    WCHAR key;

    UNREFERENCED_PARAMETER(pMenu);

    if (g_bootCfg.oalFlags & BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE) {
        OALLog(L" Disable OAL Retail Messages [y/-]: ");
    } else {
        OALLog(L" Enable OAL Retail Messages [y/-]: ");
    }    

    // Get key
    key = OALBLMenuReadKey(TRUE);
    OALLog(L"%c\r\n", key);

    if (key == L'y' || key == L'Y') {
        if (g_bootCfg.oalFlags & BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE) 
		{
            g_bootCfg.oalFlags &= ~BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE;
            OALLog(L" OAL Retail Messages disabled\r\n");
        }
		else 
		{
            g_bootCfg.oalFlags |= BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE;
            OALLog(L" OAL Retail Messages enabled\r\n");
        }    
    }
}

//------------------------------------------------------------------------------

VOID SetDisplayResolution(OAL_BLMENU_ITEM *pMenu)
{
    WCHAR key;
    UINT32 i;

    UNREFERENCED_PARAMETER(pMenu);
    
    OALBLMenuHeader(L"Select Display Resolution");

    for (i=0; i<OMAP_RES_INVALID; i++)
    {
        OALLog(L" [%d] %s\r\n", i+1 , dispResMenu[i].resName);
    }
    OALLog(L" [0] Exit and Continue\r\n");

    OALLog(L"\r\n Selection (actual %s): ", dispResMenu[g_bootCfg.displayRes].resName);
    // Get key
    do {
        key = OALBLMenuReadKey(TRUE);
    } while (key < L'0' || key > L'0' + i);    
    OALLog(L"%c\r\n", key);
    
    // If user select exit don't change device
    if (key == L'0') return;

    g_bootCfg.displayRes = (key - L'0' - 1);

}



//------------------------------------------------------------------------------

VOID SetOPPmode(OAL_BLMENU_ITEM *pMenu)
{
    WCHAR key;
    UINT32 i;
    UINT32 len = AM35x_OPP_NUM;
    OPP_MODE_MENU * pOppMenu = oppModeMenu;

    UNREFERENCED_PARAMETER(pMenu);
    
    OALBLMenuHeader(L"Select OPP Mode");
    
    for (i=0; i<len; i++)
    {
        OALLog(L" [%d] %s\r\n", i+1 , pOppMenu[i].oppModeName);
    }
    OALLog(L" [0] Exit and Continue\r\n");

    OALLog(L"\r\n Selection (actual [%s]): ", pOppMenu[g_bootCfg.opp_mode].oppModeName);
    
    // Get key
    do {
        key = OALBLMenuReadKey(TRUE);
    } while (key < L'0' || key > L'0' + i);    
    OALLog(L"%c\r\n", key);
    
    // If user select exit don't change device
    if (key == L'0') return;

    g_bootCfg.opp_mode = (key - L'0' -1);

}


//------------------------------------------------------------------------------

VOID SaveSettings(OAL_BLMENU_ITEM *pMenu)
{
    WCHAR key;
    UNREFERENCED_PARAMETER(pMenu);

    OALLog(L" Do you want save current settings [-/y]? ");

    // Get key
    key = OALBLMenuReadKey(TRUE);
    OALLog(L"%c\r\n", key);

    // Depending on result
    if (key != L'y' && key != L'Y') goto cleanUp;

    if (BLWriteBootCfg(&g_bootCfg)) {
        OALLog(L" Current settings has been saved\r\n");
    } else {        
        OALLog(L"ERROR: Settings save failed!\r\n");
    }

cleanUp:
    return;
}

//------------------------------------------------------------------------------

UINT32 OALStringToUINT32(LPCWSTR psz)
{
    UINT32 i = 0;

    // Replace the dots with NULL terminators
    while (psz != NULL && *psz != L'\0') 
        {
        if (*psz < L'0' || *psz > L'9') 
            {
            break;
            }
        i = i * 10 + (*psz - L'0');
        psz++;
        }

    return i;
}

#if BUILDING_EBOOT_SD

//------------------------------------------------------------------------------

VOID ShowSDCardSettings(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);

    OALLog(L"\r\n SDCard:\r\n");
    OALLog(
        L"  Filename:      \"%s\"\r\n", g_bootCfg.filename
    );
//  OALBLMenuReadKey(TRUE);
}

//------------------------------------------------------------------------------
#define dimof(x)                (sizeof(x)/sizeof(x[0]))

VOID EnterSDCardFilename(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);

    OALLog(L"\r\n Type new filename (8.3 format) :");
    OALBLMenuReadLine(g_bootCfg.filename, dimof(g_bootCfg.filename));
}

//------------------------------------------------------------------------------

#endif