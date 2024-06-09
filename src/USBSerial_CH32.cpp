#include "USBSerial_CH32.h"
#include "USBSerial_CH32_LL.h"

#include "USBLIB/CONFIG/usb_desc.h"
#include "USBLIB/CONFIG/usb_prop.h"
#include "USBLIB/USB-Driver/inc/usb_lib.h"

USBSerial::USBSerial(void) {}

void USBSerial::begin() {
    uint8_t i;

    UsbSerialCtl.Tx_LoadNum = 0x00;
    UsbSerialCtl.Tx_DealNum = 0x00;
    UsbSerialCtl.Tx_RemainNum = 0x00;
    for (i = 0; i < DEF_UARTx_TX_BUF_NUM_MAX; i++) {
        UsbSerialCtl.Tx_PackLen[i] = 0x00;
    }
    UsbSerialCtl.Tx_Flag = 0x00;
    UsbSerialCtl.Tx_CurPackLen = 0x00;
    UsbSerialCtl.Tx_CurPackPtr = 0x00;
    UsbSerialCtl.Tx_CurPackIdx = 0x00; 

    UsbSerialCtl.USB_Up_IngFlag = 0x00;
    UsbSerialCtl.USB_Up_TimeOut = 0x00;
    UsbSerialCtl.USB_Up_Pack0_Flag = 0x00;
    UsbSerialCtl.USB_Down_StopFlag = 0x00;

    //    if (mode) {
    //        Uart.Com_Cfg[0] = (uint8_t)(DEF_UARTx_BAUDRATE);
    //        Uart.Com_Cfg[1] = (uint8_t)(DEF_UARTx_BAUDRATE >> 8);
    //        Uart.Com_Cfg[2] = (uint8_t)(DEF_UARTx_BAUDRATE >> 16);
    //        Uart.Com_Cfg[3] = (uint8_t)(DEF_UARTx_BAUDRATE >> 24);
    //        Uart.Com_Cfg[4] = DEF_UARTx_STOPBIT;
    //        Uart.Com_Cfg[5] = DEF_UARTx_PARITY;
    //        Uart.Com_Cfg[6] = DEF_UARTx_DATABIT;
    //        Uart.Com_Cfg[7] = DEF_UARTx_RX_TIMEOUT;
    //    }

    Set_USBConfig();
    USB_Init();
    USB_Interrupts_Config();
}
void USBSerial::end() {}

int USBSerial::available(void) {
    uint8_t readableBytes = 0; 
    readableBytes += (UsbSerialCtl.Tx_CurPackLen - UsbSerialCtl.Tx_CurPackIdx); 
    for (uint8_t i = 1; i < UsbSerialCtl.Tx_RemainNum; i++) {
        readableBytes += UsbSerialCtl.Tx_PackLen[(UsbSerialCtl.Tx_DealNum + i) % DEF_UARTx_TX_BUF_NUM_MAX]; 
    }
    return readableBytes; 
}
int USBSerial::peek(void) {
    if (UsbSerialCtl.Tx_RemainNum) {
        if (UsbSerialCtl.Tx_CurPackLen == 0x00) {
            UsbSerialCtl.Tx_CurPackLen = UsbSerialCtl.Tx_PackLen[UsbSerialCtl.Tx_DealNum];
            UsbSerialCtl.Tx_CurPackPtr = (UsbSerialCtl.Tx_DealNum * DEF_USB_FS_PACK_LEN);
            UsbSerialCtl.Tx_CurPackIdx = 0; 
        }
        return USBSerial_Tx_Buf[UsbSerialCtl.Tx_CurPackPtr + UsbSerialCtl.Tx_CurPackIdx];
    }
    return -1; 
}
int USBSerial::read(void) {
    int8_t c = -1; 
    uint16_t count;

    /* Load data from the serial port send buffer to send  */
    if (UsbSerialCtl.Tx_RemainNum) {
        /* Determine whether to load from the last unsent buffer or from a new buffer */
        if (UsbSerialCtl.Tx_CurPackLen == 0x00) {
            UsbSerialCtl.Tx_CurPackLen = UsbSerialCtl.Tx_PackLen[UsbSerialCtl.Tx_DealNum];
            UsbSerialCtl.Tx_CurPackPtr = (UsbSerialCtl.Tx_DealNum * DEF_USB_FS_PACK_LEN);
            UsbSerialCtl.Tx_CurPackIdx = 0; 
        }
        c = USBSerial_Tx_Buf[UsbSerialCtl.Tx_CurPackPtr + UsbSerialCtl.Tx_CurPackIdx];
        UsbSerialCtl.Tx_CurPackIdx++; 
        if (UsbSerialCtl.Tx_CurPackIdx == UsbSerialCtl.Tx_CurPackLen) {
            /* Calculate the variables of last data */
            count = UsbSerialCtl.Tx_CurPackLen - 0;
            UsbSerialCtl.Tx_CurPackLen -= count;
            UsbSerialCtl.Tx_CurPackPtr += count;
            if (UsbSerialCtl.Tx_CurPackLen == 0x00) {
                UsbSerialCtl.Tx_PackLen[UsbSerialCtl.Tx_DealNum] = 0x0000;
                UsbSerialCtl.Tx_DealNum++;
                if (UsbSerialCtl.Tx_DealNum >= DEF_UARTx_TX_BUF_NUM_MAX) {
                    UsbSerialCtl.Tx_DealNum = 0x00;
                }
                UsbSerialCtl.Tx_RemainNum--;
            }
            UsbSerialCtl.Tx_CurPackIdx = 0; 
        }

        /* If the current serial port has suspended the downlink, restart the driver downlink */
        if ((UsbSerialCtl.USB_Down_StopFlag == 0x01) && (UsbSerialCtl.Tx_RemainNum < 2)) {
            SetEPRxValid(ENDP2);
            UsbSerialCtl.USB_Down_StopFlag = 0x00;
        }

        return c; 
    }
    return -1;
}

size_t USBSerial::write(uint8_t c) {
    write((uint8_t *)&c, 1);
    return 1;
}
size_t USBSerial::write(const uint8_t *buffer, size_t size) {
    NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
    NVIC_DisableIRQ(USB_HP_CAN1_TX_IRQn);
    // Uart.USB_Up_IngFlag = 0x01;
    // Uart.USB_Up_TimeOut = 0x00;
    USBD_ENDPx_DataUp(ENDP3, (uint8_t *)buffer, size);
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
    return 1;
}

USBSerial SerialUSB;
