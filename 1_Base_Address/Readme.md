#### Base address, Peripheral, RAM-FLASH registers

##### Why do we need to define base addresses ?
- Easy access to peripherals and memory.
- Increases clarity and maintainability in source code.
- Optimized address management when working with peripherals in embedded systems.

##### Base Addresses of FLASH, ROM, SRAM

##### Where to find base addresses of FLASH, ROM, SRAM ?
- ```3.Memory and bus architecture``` -> ```3.4.Configuration```

  ![baseaddresses](/1_Base_Address/baseAddressFLASH_ROM_SRAM.png)

##### What is Flash memory, System memory, SRAM ?

- Flash memory: 
    - Purpose: Non-volatile memory used to store the program code and constant data. It retains its contents even when the power is off.
    - Usage: Typically used to store the firmware or application code in microcontrollers. It is also used for data that must persist across power cycles, such as configuration settings.
- System memory:
    - Purpose: A special region of memory that contains a bootloader or other system-level code used for initialization and firmware upgrades.
    - Usage: contains the factory bootloader, enabling the microcontroller to enter a boot for firmware updates.
- SRAM:
    - Purpose: Volatile memory used for temporary data storage during program execution. It's where the variables, stack, and buffer are stored.
    - Usage: Use to store dynamic data like variables, fuction call stacks and temporary buffers. Faster access compared to Flash memory but loses its contents when power off.

##### Where to find base addresses of peripheral ?

- ```3.Memory and bus architecture``` -> ```3.3.Memory map```
