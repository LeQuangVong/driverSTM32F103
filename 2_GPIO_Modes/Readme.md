#### ENABLE CLOCK FOR PORT  
- Use the `RCC_APB2ENR` register to enable the clock for GPIO ports. This register controls the clock for various peripherals, including the GPIO ports.
- Each GPIO port is assigned a specific bit in the `RCC_APB2ENR` register and enable the clock by setting the corresponding bit to 1.
#### GPIO MODES
##### Input Mode
![3](/2_GPIO_Modes/3.png)
Khi tín hiệu từ các thiết bị ngoại vi đi vào qua Điện trở kéo nội sẽ có 3 trạng thái:
- Pull-up: Điện trở kéo lên, giữ chân GPIO ở mức HIGH khi không có tín hiệu bên ngoài.
- Pull-down: Điện trở kéo xuống, giữ chân GPIO ở mức LOW khi không có tín hiệu bên ngoài.
- Floating: Không sử dụng điện trở kéo, chân GPIO sẽ trôi tự do, dễ bị nhiễu.
Sau đó tín hiệu sẽ qua khối TTL Schmitt trigger để loại bỏ tín hiệu nhiễu và tín hiệu sẽ đến Input data register, nơi đây CPU sẽ đến lấy dữ liệu.
##### Output Mode
![4](/2_GPIO_Modes/4.png)
- Push-pull: Cả hai transistor (P-MOS và N-MOS) điều khiển dòng điện ra vào chân GPIO
    - Khi ghi 1 vào Output data register thì P-MOS tắt và N-MOS bật kéo chân GPIO xuống GND.
    - Khi ghi 0 vào Output data register thì P-MOS bật và N-MOS tắt kéo chân GPIO lên VCC.
- Open-drain: Chỉ có N-MOS hoạt động.
    - Khi ghi 1 vào Output data register thì N-MOS tắt và chân GPIO ở trạng thái trôi.
    - Khi ghi 0 vào Output data register thì N-MOS bật và kéo chân GPIO xuống GND.
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
##### Typecasting
```
#define GPIOA ((GPIO_RegDef_t *)GPIOA_BASEADDR)
```
Chuyển GPIOA_BASEADDR thành con trỏ kiểu GPIO_RegDef_t, cho phép truy cập và thao tác trực tiếp với các thanh ghi của GPIO.
##### Kích hoạt và hủy kích hoạt xung nhịp cho GPIO:
```
#define GPIOA_PCLK_EN() (RCC->APB2ENR |= (1<<2))
```
![5](/2_GPIO_Modes/5.png)

```
#define GPIOA_PCLK_DI() (RCC->APB2RSTR &=~ (1<<2))
```
![6](/2_GPIO_Modes/6.png)

