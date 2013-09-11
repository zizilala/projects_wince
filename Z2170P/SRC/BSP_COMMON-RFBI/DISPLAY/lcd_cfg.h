// All rights reserved ADENEO EMBEDDED 2010
// Copyright (c) 2007, 2008 BSQUARE Corporation. All rights reserved.


BOOL LcdInitGpio(void);
void LcdPowerControl(BOOL bEnable);
void LcdResBControl(BOOL bEnable);
void LcdIniControl(BOOL bEnable);
void LcdDviEnableControl(BOOL bEnable);
void LcdStall(DWORD dwMicroseconds);
void LcdSleep(DWORD dwMilliseconds);

// RFBI_SYSSTATUS
#define RFBI_RESETDONE						(1 << 0)

// RFBI_SYSCONFIG
#define RFBI_AUTOIDLE						(1 << 0)
#define RFBI_SOFTRESET						(1 << 1)
#define RFBI_SIDLEMODE_SMART_IDLE			(2 << 3)

// RFBI_CONTROL
#define RFBI_CONTROL_ENABLE					(1 << 0)
#define RFBI_CONTROL_BYPASSMODE				(1 << 1)
#define RFBI_CONTROL_CS0					(1 << 2)
#define RFBI_CONTROL_CS1					(1 << 3)
#define RFBI_CONTROL_BOTH					(3 << 2)

// RFBI_CONFIG
#define RFBI_CONFIG_PARALLELMODE_16			(3 << 0)
#define RFBI_CONFIG_DATATYPE_16				(1 << 5)
#define RFBI_CONFIG_A0POLARITY_HI			(1 << 16)
#define RFBI_CONFIG_REPOLARITY_HI			(1 << 17)
#define RFBI_CONFIG_WEPOLARITY_HI			(1 << 18)
#define RFBI_CONFIG_CSPOLARITY_HI			(1 << 19)
#define RFBI_CONFIG_TEPOLARITY_HI			(1 << 20)
#define RFBI_CONFIG_HSPOLARITY_HI			(1 << 21)
