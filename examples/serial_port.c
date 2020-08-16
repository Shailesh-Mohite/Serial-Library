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
 * File        serial_port.c (template for platform porting)
 * Created by  Shailesh Mohite (shaileshmohite@gmail.com)
 * Version     1.0
 *
 */
#include <stdint.h>

static void Comm1_transmit_interrupt(void)
{

    serial_tx_intr(&SerialComm1);
    if (0 == SerialComm1.tx_enabled)
    {
        ///<disable transmit interrupt
        ///<enable transmit end interrupt
    }
}

static void Comm1_transmit_end_interrupt(void)
{
    ///< disable all transmit interrrupts
    ///< disable transmit function
}

static void Comm1_serial_receive_interrupt(void)
{
    serial_rx_intr(&SerialComm1,<input received byte here>);
}

static void Comm1_serial_receiveerror_interrupt(void)
{
    ///< Clear overrun, framing and parity error flags
}

void sendchar (uint8_t c)
{
    ///< transmit byte to serial tx register
}

void enable_rx(uint8_t enable)
{
    if(1==enable)
    {
        ///< enable receive function
    }
    else
    {
        ///< disable receive function
    }
}

void enable_tx (uint8_t enable)
{
    if(1==enable)
    {
        ///< enable transmit function
    }
    else
    {
        ///< disable transmit function
    }
}

void myComm1Init(void)
{
    ///< platform specific serial initialization function
}
