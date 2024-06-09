/********************************** (C) COPYRIGHT *******************************
 * File Name          : UART.H
 * Author             : WCH
 * Version            : V1.01
 * Date               : 2022/12/13
 * Description        : UART communication-related headers
 *******************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#ifndef __USBSERIAL_LL_H
#define __USBSERIAL_LL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "debug.h"
#include "stdio.h"
#include "string.h"

/******************************************************************************/
/* Related macro definitions */
/* Serial buffer related definitions */
#define DEF_UARTx_TX_BUF_LEN (2 * 512)                                        /* Serial x transmit buffer size */
#define DEF_USB_FS_PACK_LEN 64                                                /* USB full speed mode packet size for serial x data */
#define DEF_UARTx_TX_BUF_NUM_MAX (DEF_UARTx_TX_BUF_LEN / DEF_USB_FS_PACK_LEN) /* Serial x transmit buffer size */

/************************************************************/
/* Serial port X related structure definition */
typedef struct __attribute__((packed)) _USBSERIAL_CTL {
    volatile uint16_t Tx_LoadNum;                           /* Serial x data send buffer load number */
    volatile uint16_t Tx_DealNum;                           /* Serial x data send buffer processing number */
    volatile uint16_t Tx_RemainNum;                         /* Serial x data send buffer remaining unprocessed number */
    volatile uint16_t Tx_PackLen[DEF_UARTx_TX_BUF_NUM_MAX]; /* The current packet length of the serial x data send buffer */
    uint8_t Tx_Flag;                                        /* Serial x data send status */

    uint16_t Tx_CurPackLen; /* The current packet length sent by serial port x */
    uint16_t Tx_CurPackPtr; /* Pointer to the packet currently being sent by serial port x */
    uint16_t Tx_CurPackIdx; /* The current packet index sent by serial port x */

    uint8_t USB_Up_IngFlag; /* Serial xUSB packet being uploaded flag */

    uint16_t USB_Up_TimeOut;   /* Serial xUSB packet upload timeout timer */
    uint8_t USB_Up_Pack0_Flag; /* Serial xUSB data needs to upload 0-length packet flag */
    uint8_t USB_Down_StopFlag; /* Serial xUSB packet stop down flag */

    uint8_t Com_Cfg[8]; /* Serial x parameter configuration (default baud rate is 115200, 1 stop bit, no parity, 8 data bits) */

} USBSERIAL_CTL, *PUARTUSBSERIAL_CTL_CTL;

/***********************************************************************************************************************/
/* Constant, variable extents */
/* The following are serial port transmit and receive related variables and buffers */
extern volatile USBSERIAL_CTL UsbSerialCtl;                                             /* Serial x control related structure */
extern __attribute__((aligned(4))) uint8_t USBSerial_Tx_Buf[DEF_UARTx_TX_BUF_LEN]; /* Serial x transmit buffer */

#ifdef __cplusplus
}
#endif

#endif

