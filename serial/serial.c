/*
 * MIT License
 *
 * Copyright (c) 2020 Shailesh Mohite
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * File        serial.c
 * Created by  Shailesh Mohite (shaileshmohite@gmail.com)
 * Version     1.0
 * Notes:- 1. Adaptation might be needed in case of Transmit end interrupt not available
 *         on user platform
 *         2. enable_rx function is optional if done by user already
 *         3. Library assumes user has already initialized serial port with
 *         required baud rate
 */
/*! @file serial.c
 * Serial library implementation
 */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"

/*!
 * Private function to get tx buffer bytes to transmit
 */
static uint32_t serial_txbuflen(serial_t * serial_com);

/*!
 * Initializes Serial library object.
 * Expects Serial port board specific settings are done before calling this function
 *
 * @param[in] serial_com : Pointer to serial object instance.
 */
void serial_init(serial_t * serial_com)
{
    if(NULL == serial_com)
        return;

    serial_com->tx_wr=0;
    serial_com->tx_rd = 0;
    serial_com->rx_wr = 0;
    serial_com->rx_rd = 0;
    serial_com->tx_enabled = 0;
    if(NULL != serial_com->enable_rx)
    {
        serial_com->enable_rx (true);
    }

}

/*!
 * Call this function from Serial receive interrupt.
 * Function automatically handles buffering of input byte received
 *
 * @param[in] serial_com : Pointer to serial object instance.
 *
 * @param[in] char : serial character received.
 */
void serial_rx_intr(serial_t * serial_com,uint8_t c)
{
    if (((serial_com->rx_wr - serial_com->rx_rd) & ~(RXBUF_SIZE - 1)) == 0)
    {
        serial_com->rxbuf[serial_com->rx_wr & (RXBUF_SIZE - 1)] = c;
        serial_com->rx_wr++;
    }
}

/*!
 * Call this function from Serial transmit interrupt.
 * Function automatically handles buffering of input byte received
 * User needs to handle transmit end interrupt by disabling
 * transmit function, transmit interrupt & transmit end interrupt
 * In Transmit interrupt if tx_enabled flag is disabled user to disable
 * transmit interrupt and enable transmit end interrupt
 *
 * @param[in] serial_com : Pointer to serial object instance.
 */
void serial_tx_intr(serial_t * serial_com)
{
    if(NULL == serial_com->sendchar)
        return;

    if (serial_com->tx_wr != serial_com->tx_rd)
    {
        serial_com->sendchar(serial_com->txbuf[serial_com->tx_rd & (TXBUF_SIZE - 1)]);
        serial_com->tx_rd++;
        if(0==serial_txbuflen(serial_com))
        {
            serial_com->tx_enabled=0;
        }
    }
    else
    {
        serial_com->enable_tx(false);
    }
}

/*!
 * User can use/port this function to system stdio io library
 *
 * @param[in] serial_com : Pointer to serial object instance.
 *
 * @return -1 Rx buffer empty
 * @return character received
 */
int16_t serial_getchar(serial_t * serial_com)
{
    if (0 == serial_rxbuflen (serial_com))
    {
        return -1;
    }

    return (serial_com->rxbuf[(serial_com->rx_rd++) & (RXBUF_SIZE - 1)]);
}

/*!
 * User can use/port this function to system stdio io library
 *
 * @param[in] serial_com : Pointer to serial object instance.
 *
 * @return 0 Success
 * @return -1 Tx buffer full
 */
int16_t serial_putchar(serial_t *serial_com, uint8_t c)
{
    // Wait in infinite loop here
#if TX_BLOCKING_EN == 1
    while (serial_txbuflen (serial_com) >= TXBUF_SIZE)
        ;
#else
    if (serial_txbuflen (serial_com) >= TXBUF_SIZE)
    {
        return -1;
    }
#endif

    serial_com->txbuf[serial_com->tx_wr & (TXBUF_SIZE - 1)] = c;
    serial_com->tx_wr++;

    if (0 == serial_com->tx_enabled)
    {
        serial_com->tx_enabled = true;
        serial_com->enable_tx (true);
    }
    return 0;
}

/*!
 * API returns Serial Receive byte count
 *
 * @param[in] serial_com : Pointer to serial object instance.
 *
 * @return Rx data count
 */
uint32_t serial_rxbuflen(serial_t * serial_com)
{
    return (serial_com->rx_wr - serial_com->rx_rd);
}

/*!
 * API returns Serial Transmit bytes count
 *
 * @param[in] serial_com : Pointer to serial object instance.
 *
 * @return Tx buffer size
 */
static uint32_t serial_txbuflen(serial_t * serial_com)
{
    return (serial_com->tx_wr - serial_com->tx_rd);
}
