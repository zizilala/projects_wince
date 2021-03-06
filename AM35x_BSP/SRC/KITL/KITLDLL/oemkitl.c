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
//  File:  kitl.c
//

//-------------------------------------------------------------------------------

#include "bsp.h"
#include "kitl_cfg.h"
#include "oemkitl.h"

//------------------------------------------------------------------------------
//  Local definition

#ifndef HKEY_LOCAL_MACHINE
#define HKEY_LOCAL_MACHINE          ((HKEY)(ULONG_PTR)0x80000002)
#endif

static BOOL s_bKitlActive = FALSE;


//------------------------------------------------------------------------------
//  External functions

extern void 
OEMEthernet9113DriverEnable(
    BOOL bEnable
    );

extern void 
OEMEthernetInternalDriverEnable(
    BOOL bEnable
    );

extern void 
OEMUsbDriverEnable(
    BOOL bEnable
    );

extern void 
ReadMacAddressFromFuse
    (UCHAR mac[6]);
    
//------------------------------------------------------------------------------
//Currently only support Ethernet Kitl, and we didn't coniser wake-up

BOOL
OEMKitlStartup(
    )
{
    BOOL rc = FALSE;

    OAL_KITL_ARGS *pArgs, args;
    CHAR *szDeviceId;
    
    KITLSetDebug(
                ZONE_ERROR |
                ZONE_WARNING |
        //        ZONE_INIT |        
        //        ZONE_KITL_OAL |
        //        ZONE_KITL_ETHER |
                0);


    KITL_RETAILMSG(ZONE_KITL_OAL, ("+OALKitlStart\r\n"));

    // First get boot args and device id
    pArgs = (OAL_KITL_ARGS*)OALArgsQuery(OAL_ARGS_QUERY_KITL);

    // If we don't get kitl arguments use default
    if (pArgs == NULL)
        {
#ifndef BSP_READ_MAC_FROM_FUSE
		const UINT16 defaultMac[] = DEFAULT_MAC_ADDRESS;
#endif
        KITL_RETAILMSG(ZONE_WARNING, (
            "WARN: Boot arguments not found, use defaults\r\n"
            ));
        memset(&args, 0, sizeof(args));
        args.flags = OAL_KITL_FLAGS_ENABLED|OAL_KITL_FLAGS_DHCP;
        args.devLoc.IfcType = Internal;
        args.devLoc.BusNumber = 0;
        args.devLoc.LogicalLoc = BSP_LAN9311_REGS_PA;
#ifdef BSP_READ_MAC_FROM_FUSE
		ReadMacAddressFromFuse((UCHAR*)args.mac);
#else
        memcpy(args.mac,defaultMac,sizeof(args.mac));        
#endif
		
        pArgs = &args;
        }

    // We always create device name
    szDeviceId = BSP_DEVICE_PREFIX;
    pArgs->flags |= OAL_KITL_FLAGS_EXTNAME;
    
    // Initialize debug device
    switch (pArgs->devLoc.IfcType)
        {
        case Internal:
            switch (pArgs->devLoc.LogicalLoc)
                {
                case BSP_LAN9311_REGS_PA:
/*
                    // enable clocks to GPIO banks that have KITL dependencies so we can do initialization
                    OEMKitlEnableClocks(TRUE);
                    
                    // Reset LAN chip
                    pGPIORegs = OALPAtoUA(BSP_RESET_ETHER_KITL_GPIO_PA);
                    CLRREG32(&pGPIORegs->DATAOUT, 1 << (BSP_RESET_ETHER_KITL_GPIO % 32));
                    OALStall(1000);
                    SETREG32(&pGPIORegs->DATAOUT, 1 << (BSP_RESET_ETHER_KITL_GPIO % 32));
                    OALStall(1000);

                    // Prepare interrupt
                    pGPIORegs = OALPAtoUA(BSP_ETHER_GPIO_PA);
                    SETREG32(&pGPIORegs->OE, 1 << (BSP_IRQ_ETHER_KITL % 32));
                    // Interrupt on falling edge
                    SETREG32(&pGPIORegs->FALLINGDETECT, 1 << (BSP_IRQ_ETHER_KITL % 32));

                    OEMKitlEnableClocks(FALSE);
*/
                    break;
                }
            break;
        }

    // Finally call KITL library
    rc = OALKitlInit(szDeviceId, pArgs, g_kitlDevices);

    // If it failed or KITL is disabled
    if (!rc || (pArgs->flags & OAL_KITL_FLAGS_ENABLED) == 0) goto cleanUp;

    // enable kitl interrupts
    s_bKitlActive = TRUE;
    OEMKitlEnable(TRUE);
    
cleanUp:
    KITL_RETAILMSG(ZONE_KITL_OAL, ("-OALKitlStart(rc = %d)\r\n", rc));

    return rc;
}

//------------------------------------------------------------------------------
//
//  Function:  OALKitlInitRegistry
//
VOID
OALKitlInitRegistry(
    )
{
    DEVICE_LOCATION devLoc;

    // Get KITL device location
    if (!OALKitlGetDevLoc(&devLoc)) goto cleanUp;

    // Depending on device bus
    switch (devLoc.IfcType)
        {
        case Internal:
            switch (devLoc.LogicalLoc)
                {
                case BSP_LAN9311_REGS_PA:
                    // Disable 9113
                    OEMEthernet9113DriverEnable(FALSE);
                    OEMEthernetInternalDriverEnable(TRUE);
                    OEMUsbDriverEnable(TRUE);
                    break;   
                case OMAP_CPGMAC_REGS_PA:
                    // Disable internal
                    OEMEthernet9113DriverEnable(TRUE);
                    OEMEthernetInternalDriverEnable(FALSE);
                    OEMUsbDriverEnable(TRUE);
                    break;
                case OMAP_USBHS_REGS_PA:
                    // Disable USB
                    OEMEthernet9113DriverEnable(TRUE);
                    OEMEthernetInternalDriverEnable(TRUE);
                    OEMUsbDriverEnable(FALSE);
                    break;
                default:
                    // Enable both USB and ethernet
                    OEMEthernetInternalDriverEnable(TRUE);
                    OEMEthernet9113DriverEnable(TRUE);
                    OEMUsbDriverEnable(TRUE);
                    break;   
                }
            break;
        }

cleanUp:
    return;
}

//------------------------------------------------------------------------------

DWORD
OEMEthGetSecs(
    )
{
    return OALGetTickCount()/1000;
}

//------------------------------------------------------------------------------
//
//  Function:     OEMKitlEnable
//
//  Enables/disables kitl.  Necessary to enable/disable gpio pin for kitl
//  interrupts. 
//
BOOL
OEMKitlEnable(
    BOOL bEnable
    )
{
    if (s_bKitlActive == FALSE) return FALSE;

    UNREFERENCED_PARAMETER(bEnable);

    return TRUE;
}

//------------------------------------------------------------------------------
//
//  Function:  OALGetTickCount
//
UINT32
OALGetTickCount(
    )
{
    OMAP_GPTIMER_REGS *pGPTimerRegs = OALPAtoUA(OMAP_GPTIMER1_REGS_PA);
    return INREG32(&pGPTimerRegs->TCRR) >> 5;
}

//------------------------------------------------------------------------------
//
//  Function:  OEMKitlIoctl
//
//  This function handles KITL IOCTL codes.
//
//
BOOL OEMKitlIoctl (DWORD code, VOID * pInBuffer, DWORD inSize, VOID * pOutBuffer, DWORD outSize, DWORD * pOutSize)
{
    BOOL fRet = FALSE;
    switch (code) {
    case IOCTL_HAL_INITREGISTRY:
        OALKitlInitRegistry();
        // Leave return code false and set last error to ERROR_NOT_SUPPORTED
        // This allows code to fall through to OEMIoctl so IOCTL_HAL_INITREGISTRY can be 
        // handled there as well.
        NKSetLastError(ERROR_NOT_SUPPORTED);
        break;
    default:
        fRet = OALIoCtlVBridge (code, pInBuffer, inSize, pOutBuffer, outSize, (UINT32*)pOutSize);
    }

    return fRet;
}
