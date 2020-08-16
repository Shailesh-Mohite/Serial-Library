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
 * File        serial.h
 * Created by  Shailesh Mohite (shaileshmohite@gmail.com)
 * Version     1.0
 *
 */

/*!@file serial.h
 * Serial library header
 */
#ifndef _SERIAL_H_
#define _SERIAL_H_

/*! C++ guard */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/**
 * Serial library Configuration Options start
 *
*/
/* SPECIFY THE TRANSMIT BUFFER SIZE
 * Serial Transmit buffer size, must be powers of 2 (2,4,8,16..)
 * This has performance impact during heavy usage of logging
 */
#define TXBUF_SIZE   128

/* SPECIFY THE RECEIVE BUFFER SIZE
 * Serial Receive buffer size, must be powers of 2 (2,4,8,16..)
 */
#define RXBUF_SIZE   128

/* SPECIFY WHETHER TO HAVE BLOCKING TRANSMIT FUNCTION
 * In case of non-blocking user will need to handle transmit buffer size
 * check and handling during transmition over serial library. Blocking 
 * is recommended to start with.
 */
#define TX_BLOCKING_EN  (1)
/**
 * Serial library Configuration Options end
 *
*/
/*!
 * Generic Function pointer to user port functions
 *
 * @param[in] 8-bit input value
 */
typedef void (*port_func) (uint8_t);

/**
 * Serial library object definition.
 *
*/
typedef struct
{
    uint8_t txbuf[TXBUF_SIZE]; ///< Serial Transmit data buffer
    uint8_t rxbuf[RXBUF_SIZE]; ///< Serial Receive data buffer
    volatile uint16_t tx_wr; ///< Transmit data buffer write pointer
    volatile uint16_t tx_rd; ///< Transmit data buffer read pointer
    volatile uint16_t rx_wr; ///< Receive data buffer write pointer
    volatile uint16_t rx_rd; ///< Receive data buffer read pointer
    volatile bool tx_enabled; ///< Transmit enable status
    port_func sendchar; ///< pointer to user send char function. Provides byte to transmit as input.
    port_func enable_rx; ///< pointer to user enable receive function. Provide 1 to enable & 0 to disable.
    port_func enable_tx; ///< pointer to user enable transmit function
} serial_t;


/*!
 * Initializes Serial library object.
 * Expects Serial port board specific settings are done before calling this function
 *
 * @param[in] serial_com : Pointer to serial object instance.
 */
void serial_init(serial_t * serial_com);

/*!
 * Call this function from Serial receive interrupt.
 * Function automatically handles buffering of input byte received
 *
 * @param[in] serial_com : Pointer to serial object instance.
 *
 * @param[in] char : serial character received.
 */
void serial_rx_intr(serial_t * serial_com,uint8_t c);

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
void serial_tx_intr(serial_t * serial_com);

/*!
 * User can use/port this function to system stdio io library
 *
 * @param[in] serial_com : Pointer to serial object instance.
 *
 * @return -1 Rx buffer empty
 * @return character received
 */
int16_t serial_getchar(serial_t * serial_com);

/*!
 * User can use/port this function to system stdio io library
 *
 * @param[in] serial_com : Pointer to serial object instance.
 *
 * @return 0 Success
 * @return -1 Tx buffer full
 */
int16_t serial_putchar(serial_t * serial_com,uint8_t c);

/*!
 * API returns Serial Receive byte count
 *
 * @param[in] serial_com : Pointer to serial object instance.
 *
 * @return Rx data count
 */
uint32_t serial_rxbuflen(serial_t * serial_com);

#ifdef __cplusplus
}
#endif /* End of C++ guard */

#endif /* _SERIAL_H_ */
