//############################################################################
// HEAD : (c) ACTIA 2016 : " Any copy and reuse of this file without
//                           the written consent of ACTIA is forbidden "
//        --------------------------------------------------------------------
//        Classification :  (-)No   (*)Confident. ACTIA   (-)Confident. Client
//        --------------------------------------------------------------------
//        Classification Transcontrol :  (-)No   (*)Restraint   (-)Confident.
//        --------------------------------------------------------------------
//        Module name : USB module
//############################################################################
// FILE : usb.c
//----------------------------------------------------------------------------
// DESC :
//----------------------------------------------------------------------------
// LIST :
//
//----------------------------------------------------------------------------
// HIST : Version |   Date   | Author | Description
//        --------------------------------------------------------------------
//         01.01  | 03/02/16 |  Nad   | Creation of this file
//############################################################################

/*************************************************************************************************/
/*                                       INCLUDE                                                 */
/*************************************************************************************************/
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <syslog.h>
/*************************************************************************************************/
/*                                       INCUSER                                                 */
/*************************************************************************************************/
#include "std_type.h"
#include "usb.h"
/*************************************************************************************************/
/*                                       DEFINE                                                  */
/*************************************************************************************************/
#define USB_DEVICE_MOD  		"g_mass_storage"
#define USB_DEVICE_FILE			"/tmp/usbdevice"

#define KERNEL_MODULES_LOADED 	"/proc/modules"
/*************************************************************************************************/
/*                                       PROTOTYPE                                               */
/*************************************************************************************************/
extern TS32 formatDrive(CHAR * filename);
/*************************************************************************************************/
/*                                       VARIABLE                                                */
/*************************************************************************************************/
TU8 vu8USBTestResult = RSR_TestCondIncorr;
/*************************************************************************************************/
/*                                       FUNCTIONS                                               */
/*************************************************************************************************/

/*************************************************************************************************/
/*                                       EXTERN FUNCTION                                         */
/*************************************************************************************************/

/** ----------------------------------------------------------------------------
 * @brief Start USB Gadget mass-storage self-test
 * @param
 * @return 0 if OK, -1 otherwise
*/
TS32 USB_iStart_gadget_self_test(void) 
{
	CHAR  lts8Cmdline[512];
	TS32  lvs32Fd     = -1;
	TS32  lvs32Length = -1;
	TS32  lvs32Ret    = -1;

	memset((void *)lts8Cmdline, 0x00, sizeof(lts8Cmdline));

	syslog(LOG_NOTICE, " %s", __func__);

	vu8USBTestResult = RSR_TestInProgress;

	/* Format in FAT12 (see fat12.c) */
	formatDrive(USB_DEVICE_FILE);
	/* Check Linux Kernel modules directory */
	lvs32Fd = open(KERNEL_MODULES_LOADED, O_RDONLY);
	if (lvs32Fd < 0) 
	{
		syslog(LOG_INFO, "    %s, line %d : Error open the device %d", __func__, __LINE__, lvs32Fd);
		vu8USBTestResult = RSR_TestFailed;
	} 
	else
	{
		/* Read content */
		lvs32Length = read(lvs32Fd, lts8Cmdline, sizeof(lts8Cmdline));
		if (lvs32Length > 0) 
		{
			if (strstr(lts8Cmdline, USB_DEVICE_MOD) > 0) 
			{
				syslog(LOG_INFO, "    %s, line %d : Missing module g_mass_storage", __func__, __LINE__);			
			} 
			else 
			{
				strcpy(lts8Cmdline, "modprobe ");
				strcat(lts8Cmdline,  USB_DEVICE_MOD);
				strcat(lts8Cmdline, " file=");
				strcat(lts8Cmdline,  USB_DEVICE_FILE);
				strcat(lts8Cmdline," idVendor=2000 ");  		/*USB Vendor ID (16 bit integer)*/
				strcat(lts8Cmdline," idProduct=2000 "); 		/*USB Product ID (16 bit integer)*/
				strcat(lts8Cmdline," bcdDevice=2000 "); 		/*USB Device version (BCD) (16 bit integer)*/
				strcat(lts8Cmdline," iManufacturer=ACTIA "); 	/*USB Manufacturer string (string)*/
				strcat(lts8Cmdline," iProduct=PCM "); 			/*USB Product string (string)*/				
				strcat(lts8Cmdline," iSerialNumber=1 "); 		/*SerialNumber string (sting)*/
				syslog(LOG_INFO, "    %s, line %d : cmd = %s", __func__, __LINE__, lts8Cmdline);								
				lvs32Ret = system(lts8Cmdline);
				if(lvs32Ret != 0)
					vu8USBTestResult = RSR_TestFailed;
			}
			close(lvs32Fd);
		}
		else
		{
			vu8USBTestResult = RSR_TestFailed;
		}
	}
	return lvs32Ret;
}

/** ----------------------------------------------------------------------------
 * @brief Stop the USB gadget test
 * @param
 * @return lvu8Status : 0x01 RSR_TestOK
 *                      0x02  RSR_TestInProgress
 *                      0x03  RSR_TestFailed
 *                      0x04  RSR_TestCondIncorr
 *                      0x04  RSR_TestDone 
*/
TS32 USB_iGadget_test_stop(void) 
{
	CHAR  lts8Cmdline[64];
	TU8   lvu8Status = RSR_TestFailed;

	memset((void *)lts8Cmdline, 0x00, sizeof(lts8Cmdline));

	syslog(LOG_NOTICE, " %s", __func__);

	if(vu8USBTestResult == RSR_TestInProgress)
	{
		/* Remove the module from the Linux Kernel */
		strcpy(lts8Cmdline, "rmmod ");	
		strcat(lts8Cmdline, USB_DEVICE_MOD);
		syslog(LOG_INFO, "    %s, line %d : cmd = %s", __func__, __LINE__, lts8Cmdline);			
		system(lts8Cmdline);
		lvu8Status = RSR_TestOK;
		vu8USBTestResult = RSR_TestCondIncorr;					
	}
	else
	{
		lvu8Status = vu8USBTestResult;
	}

	return lvu8Status;
}