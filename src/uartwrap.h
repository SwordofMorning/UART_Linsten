#pragma once

/**
 * @file uart.h
 * @author @SwordofMorning / xjt.include@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-02-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set the opt object
 * 
 * @param fd file device.
 * @param baudRate baud rate, trans speed.
 * @param nBits one time send N bits.
 * @param nEvent parity bit.
 * @param nStop stop bit.
 * @return success or fail.
 * @retval 0, success.
 * @retval -1, fail.
 */
int UARTWRAP_SetOpt(int fd, int baudRate, int nBits, char nEvent, int nStop);

/**
 * @brief open file device
 * 
 * @param com device path
 * @return fd
 */
int UARTWRAP_OpenPort(char* com);

/**
 * @brief uart init function.
 * 
 * @return success or fail.
 * @retval fd, success.
 * @retval -1, open port fail.
 * @retval -2, set opt fail.
 */
int UARTWRAP_InitDevice(char* dev_name, int baudrate);

/**
 * @brief release uart
 */
void UARTWRAP_ReleaseDevice(int fd);

#ifdef __cplusplus
}
#endif