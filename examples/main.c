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
 * File        main.c (example usage file)
 * Created by  Shailesh Mohite (shaileshmohite@gmail.com)
 * Version     1.0
 *
 */
#include <stdio.h>
#include "serial/serial.h"

serial_t SerialSCI1; ///< Serial library object instance for Comm1

/*!
 * API returns Serial Receive byte count
 *
 * @param[in] serial_com : Pointer to serial object instance.
 *
 * @return Rx data count
 */
void main(void)
{
    int16_t c;
    myComm1Init();///< user platform function to initialize serial comm port
    serial_init (&SerialSCI1);
    SerialSCI1.sendchar = sendchar; ///< port function to write byte to uart tx register
    SerialSCI1.enable_tx = enable_tx; ///< port function to enable/disable uart transmit
    SerialSCI1.enable_rx = enable_rx; ///< port function to enable/disable uart receive
    SerialSCI1.enable_rx (true);

    printf("Hello world\n");

    while (0==serial_rxbuflen(&SerialSCI1))
    {
        nop();
    }
    c = getchar();
    printf("you entered ");
    putchar(c);
    while(1);
}
