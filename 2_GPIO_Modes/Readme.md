#### ENABLE CLOCK FOR PORT  
- Use the `RCC_APB2ENR` register to enable the clock for GPIO ports. This register controls the clock for various peripherals, including the GPIO ports.
- Each GPIO port is assigned a specific bit in the `RCC_APB2ENR` register and enable the clock by setting the corresponding bit to 1.
#### GPIO MODES
##### Input Mode
- `GPIO_MODE_ANALOG`: The pin is configured as an analog input (e.g., for ADC).
- `GPIO_MODE_INPUT_FLOATING`: Floating input, no internal pull-up or pull-down resistors.
- `GPIO_MODE_INPUT_PULL_UP`: Input with a pull-up resistor enabled.
- `GPIO_MODE_INPUT_PULL_DOWN`: Input with a pull-down resistor enabled.
##### Output Mode
- `GPIO_MODE_OUTPUT_PP (Push-Pull Output)`: The pin drives high or low levels (0 or 1).
- `GPIO_MODE_OUTPUT_OD (Open-Drain Output)`: The pin can only pull down to 0, and it requires an external pull-up resistor for high state.
##### Alternate Function Mode
- `GPIO_MODE_AF_PP (Alternate Function Push-Pull)`: The pin operates in alternate function mode, used for peripherals like UART, SPI, or I2C, with push-pull output.
- `GPIO_MODE_AF_OD (Alternate Function Open-Drain)`: The pin operates in alternate function mode with open-drain output (used for protocols like I2C).

#### GPIO CONFIGURATIONS
##### Output configuration
- `GPIO_SPEED_LOW`: Low speed for slower signals or less power consumption.
- `GPIO_SPEED_MEDIUM`: Medium speed for moderate performance.
- `GPIO_SPEED_HIGH`: High speed for high-frequency signal toggling.
##### Input configuration
- `GPIO_NOPULL`: No internal pull-up or pull-down resistor.
- `GPIO_PULLUP`: Enables a pull-up resistor to ensure the pin is at a logic high when no signal is applied.
- `GPIO_PULLDOWN`: Enables a pull-down resistor to ensure the pin is at a logic low when no signal is applied.
#### SPECIFIC MODE AND CONFIGURATION VALUES
![1](/2_GPIO_Modes/1.png)

![2](/2_GPIO_Modes/2.png)

##### Input Mode
- `Input Analog (CNF=00, MODE=00)`: Used for analog input (e.g., ADC).
- `Input Floating (CNF=01, MODE=00)`: No pull-up or pull-down, used for signals like buttons.
- `Input Pull-up/Pull-down (CNF=10, MODE=00)`: Configured with pull-up or pull-down via the ODR register.
##### Output Mode
- `Output Push-Pull (CNF=00, MODE=01/10/11)`: Push-pull output for driving high and low logic levels with different speeds (2 MHz, 10 MHz, 50 MHz).
- `Output Open-Drain (CNF=01, MODE=01/10/11)`: Open-drain output for sinking current (e.g., I2C).
##### Alternate Function Mode
- `Alternate Function Push-Pull (CNF=10, MODE=01/10/11)`: For peripherals like UART or SPI that require push-pull output.
- `Alternate Function Open-Drain (CNF=11, MODE=01/10/11)`: For peripherals like I2C that require open-drain output.
