/********************************** (C) COPYRIGHT *******************************
 * File Name          : usb_endp.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/08/08
 * Description        : Endpoint routines
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/ 
#include "USBLIB/USB-Driver/inc/usb_lib.h"
#include "USBLIB/CONFIG/usb_desc.h"
#include "USBLIB/USB-Driver/inc/usb_mem.h"
#include "USBLIB/CONFIG/hw_config_usb.h"
#include "USBLIB/CONFIG/usb_istr.h"
#include "USBLIB/CONFIG/usb_pwr.h"
#include "USBLIB/CONFIG/usb_prop.h"
#include "USBSerial_CH32_LL.h"

uint8_t USBD_Endp3_Busy;
uint16_t USB_Rx_Cnt=0; 

/*********************************************************************
 * @fn      EP2_IN_Callback
 *
 * @brief  Endpoint 1 IN.
 *
 * @return  none
 */
void EP1_IN_Callback (void)
{ 
	
}



/*********************************************************************
 * @fn      EP2_OUT_Callback
 *
 * @brief  Endpoint 2 OUT.
 *
 * @return  none
 */
void EP2_OUT_Callback (void)
{ 
	uint32_t len;
    len = GetEPRxCount( EP2_OUT & 0x7F );
    PMAToUserBufferCopy( &USBSerial_Tx_Buf[ ( UsbSerialCtl.Tx_LoadNum * DEF_USB_FS_PACK_LEN ) ], GetEPRxAddr( EP2_OUT & 0x7F ), len );
    UsbSerialCtl.Tx_PackLen[ UsbSerialCtl.Tx_LoadNum ] = len;
    UsbSerialCtl.Tx_LoadNum++;
    if( UsbSerialCtl.Tx_LoadNum >= DEF_UARTx_TX_BUF_NUM_MAX )
    {
        UsbSerialCtl.Tx_LoadNum = 0x00;
    }
    UsbSerialCtl.Tx_RemainNum++;

	if( UsbSerialCtl.Tx_RemainNum >= ( DEF_UARTx_TX_BUF_NUM_MAX - 2 ) )
    {
        UsbSerialCtl.USB_Down_StopFlag = 0x01;
    }
    else
    {
        SetEPRxValid( ENDP2 );
    }
}
/*********************************************************************
 * @fn      EP3_IN_Callback
 *
 * @brief  Endpoint 3 IN.
 *
 * @return  none
 */
void EP3_IN_Callback (void)
{ 
	USBD_Endp3_Busy = 0;
	UsbSerialCtl.USB_Up_IngFlag = 0x00;
}

/*********************************************************************
 * @fn      USBD_ENDPx_DataUp
 *
 * @brief  USBD ENDPx DataUp Function
 * 
 * @param   endp - endpoint num.
 *          *pbuf - A pointer points to data.
 *          len - data length to transmit.
 * 
 * @return  data up status.
 */
uint8_t USBD_ENDPx_DataUp( uint8_t endp, uint8_t *pbuf, uint16_t len )
{
	if( endp == ENDP3 )
	{
		if (USBD_Endp3_Busy)
		{
			return USB_ERROR;
		}
		USB_SIL_Write( EP3_IN, pbuf, len );
		USBD_Endp3_Busy = 1;
		SetEPTxStatus( ENDP3, EP_TX_VALID );
	}
	else
	{
		return USB_ERROR;
	}
	return USB_SUCCESS;
}
