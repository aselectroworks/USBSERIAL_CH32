/********************************** (C) COPYRIGHT *******************************
 * File Name          : UART.C
 * Author             : WCH
 * Version            : V1.01
 * Date               : 2022/12/13
 * Description        : uart serial port related initialization and processing
 *******************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "USBSerial_CH32_LL.h"

/*******************************************************************************/
/* Variable Definition */
/* Global */

/* The following are serial port transmit and receive related variables and buffers */
volatile USBSERIAL_CTL UsbSerialCtl;

__attribute__((aligned(4))) uint8_t USBSerial_Tx_Buf[DEF_UARTx_TX_BUF_LEN]; /* Serial port 2 transmit data buffer */
