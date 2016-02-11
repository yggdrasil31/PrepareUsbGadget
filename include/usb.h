#ifndef USB_H_
#define USB_H_

/*************************************************************************************************/
/*                                       DEFINE                                                  */
/*************************************************************************************************/
#define RSR_TestOK          0x01
#define RSR_TestInProgress  0x02
#define RSR_TestFailed      0x03
#define RSR_TestCondIncorr  0x04
#define RSR_TestDone        0x05
/************************************************************************************************/
/*                                       EXTERN FUNCTION                                        */
/************************************************************************************************/

/** ----------------------------------------------------------------------------
 * @brief Start USB Gadget mass-storage self-test
 * @param
 * @return 0 if OK, -1 otherwise
*/
extern TS32 USB_iStart_gadget_self_test(void);

/** ----------------------------------------------------------------------------
 * @brief Stop the USB gadget test
 * @param
 * @return lvu8Status : 0x01 RSR_TestOK
 *                      0x02  RSR_TestInProgress
 *                      0x03  RSR_TestFailed
 *                      0x04  RSR_TestCondIncorr
 *                      0x04  RSR_TestDone 
*/
extern TS32 USB_iGadget_test_stop(void);

#endif /* USB_H_ */
