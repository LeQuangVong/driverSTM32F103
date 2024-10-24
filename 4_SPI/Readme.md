#### The key components SPI 
- SCK(Serial Clock): A clock signal genarated by the master that synchronizes data transmission between the master and the slave.
- MOSI(Master Out Slave In): The data line used to send data from the master to the slave.
- MISO(Master In Slave Out): The data line used to send data from the slave to the master.
- SS(Slave Select): A signal from the master that selects which slave will communicate. When SS is low, the selected slave is ready to transmit and receive data.
#### The important configuration for SPI
- Clock polarity(CPOL): Determines the idle state of the clock signal (SCK).
    - CPOL = 0: Clock is low when idle.
    - CPOL = 1: Clock is high when idle.
- Clock phase(CPHA): Specifies when data is sampled and shifted relative to the clock edge.
    - CPHA = 0: Data is sampled on the first clock edge (rising or falling depending on CPOL).
    - CPHA = 1: Data is sampled on the second clock edge.

| CPOL | CPHA |                         Description                         |
|:----:|:----:|:-----------------------------------------------------------:|
|   0  |   0  |  Data is sampled on the rising edge. The clock starts low.  |
|   0  |   1  |  Data is sampled on the falling edge. The clock starts low. |
|   1  |   0  |  Data is sampled on the rising edge. The clock starts high. |
|   1  |   1  | Data is sampled on the falling edge. The clock starts high. |

- Clock speed(Frequency): The speed at which the clock signals toggles, affecting the data transfer rate. The master controls the clock frequency, and it must be within the limits supported by both master and slave.
- Data Frame Format: Defines the number of bits in each data frame (typically 8 or 16 bits).
- LSB/MSB first: Determines whether the least significant bit (LSB) or most significant bit (MSB) is transmitted first.

#### How to transfer data works
- Clock synchronization (SCK):
    - The master genarates the clock signal (SCK) to synchronize the communication. Each rising and falling edge (depending on the configuration of CPOL and CPHA) shifts one bit of data in the shift register of both master and slave.
    - This clock signal is shared by both master and slave, ensuring that data is transferred at the same pace in both directions.
- Shift register slave and master:
    - Shift register is a key companent where data is stored and trasferred bit by bit.
    - The master's shift register holds the data to be sent to the slave.
    - The slave's shift register holds the data to the sent to the master.
    - Every clock pulse (SCK) shifts one bit from the master's shift register out via MOSI and simultaneously shifts one bit from the slave's shift register out via MISO.
- Data transfer process:
    - When the master selects the slave by pulling the SS (Slave Select) pin low, the communication starts.

    ![1](/4_SPI/1.png)

    - On the each clock pulse, a bit from the master's shift register is transmitted via MOSI to the slave.
    - At the same time, a bit from the slave's shift register is transmitted via MISO to the master.

    ![2](/4_SPI/2.png)

    - The process continues until all bits (usually 8 or 16 bits) are transferred.
#### SPI bus configuration discussion: Full duplex, Half-duplex, and Simplex
- Full duplex:
    - Simultaneous two-way communication (send and receive at the same time).

![3](/4_SPI/3.png)

- Half-duplex:
    - Alternating two-way communication (either send or receive at one time).

![4](/4_SPI/4.png)

- Simplex:
    - One-way communication (only send or only receive).

![5](/4_SPI/5.png)
