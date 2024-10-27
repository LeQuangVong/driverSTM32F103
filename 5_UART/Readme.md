`UART` stand for Universal Asynchronous Receiver Transmitter
`USART` stand for Universal Synchronous Receiver Transmitter
#### Difference between UART and USART
The only difference between them is that the UART peripheral supports only asynchronous mode, whereas USART support both Asynchronous and Synchronous mode.
#### UART pin
- TX (Transmit Data Output): We transmit data out of the UART over TX.
- RX (Receive Data Input): We receive data into the UART over the RX pin.
If we are not transmitting anything, then the TX line will be held high.
- CTS (Clear to Send): The UART will send the data out of the TX line, only if the CTS pin is pulled low. Otherwise, the data transmission will be held until this pin is pulled to low.
- RTS (Request to Send): It is an active low pin. The device uses this line to inform the other device that it needs data.

![1](/5_UART/1.png)

#### UART frame format
- Start bit (1 bit): The start bit signals the beginning of a data frame. When a device begins transmitting data, it pulls the signal low (0) for a duration equal to one bit.
- Data bits (5-9 bits): The data bits form the core of the frame, containing the actual information being transmitted
- Parity bit (1 bit, optional): provides error checking.
    - Even parity: The parity bit is set the total number of `1` bits in the frame is even.
    - Odd parity: The pairty bit is set the total number of `1` bits in the frame is odd.
    - No parity: Parity can be disabled if error checking is not equired.

![2](/5_UART/2.png)

- Stop bit (1 bit or 2 bits): The stop bit masks the end of a data frame. The signal is pulled high (1) for a duration of 1 or 2 bits, depending on configuration.
#### UART Baud rate
- The baud rate is the rate at which data is transmitted between devices, defines in bit per second (bps). It represents the number of bits transmitted per second, including start, data, parity, and stop bits. A higher baud rate means faster data transfer, but if set too high, it may lead to data errors due to syschronization issues.
- UART supports several standard baud rates, including 9600, 19200, 38400, 57600, 115200.