Có 2 hệ thống xong chính:
- System Clock (SYSCLK): nó có thể lấy nguồn từ các nguồn sau:
    - HSI oscillator clock: bộ giao động tốc độ cao ở bên trong con chip.
    - HSE oscillator clock: bộ giao động tốc độ cao ở ngoài con chip, phai gắn thêm thạch anh ở bên ngoài.
    - PLL clock: bộ nhân xung có thể lấy nguồn từ HSI hoặc HSE.
- Hệ thống xung clock cho bộ đếm thời gian thực (RTC) nó lấy nguồn từ các nguồn sau:
    - Lấy nguồn từ thạch anh nội 40kHz (RC LSI).
    - Lấy nguồn từ thạch anh ngoại 32.768 kHz (LSE crystal).

#### Viết RCC:
- Xác định nguồn xung nhịp chính:
Lấy các bit của SWS trong thanh ghi RCC_CFGR
![1](/7_RCC/1.png)

Trong trường hợp này HSI và HSE đều là 8 MHz.

```
clksrc = ((RCC->CFGR >> 2) & 0x03);

if(clksrc == 0 || clksrc == 1)
{
    SystemClk = 8000000;
}
```

- Tính toán AHB Prescaler
Lấy giá trị các bit của HPRE của thanh ghi RCC_CFGR
![2](/7_RCC/2.png)

```
uint16_t AHB_Prescaler[8] = {2,4,8,16,64,128,256,512};

temp = ((RCC->CFGR >> 4) & 0xF);

if(temp < 8)
{
    preAHB = 1;
}else
{
    preAHB = AHB_Prescaler[temp - 8];
}
```
- Tính toán giá trị của APB1 Prescaler
Lấy giá trị các bit của PPRE1 trong thanh ghi RCC_CFGR
![3](/7_RCC/3.png)
```
uint8_t APB1_Prescaler[4] = {2,4,8,16};
temp = ((RCC->CFGR >> 10) & 0x7);

if(temp < 4)
{
    preAPB1 = 1;
}else
{
    preAPB1 = APB1_Prescaler[temp - 4];
}
```
- Tính giá trị PCLK1
Xung từ System Clock qua bộ chia AHB qua bộ chia APB1 mới có giá trị PCLK1
![5](/7_RCC/5.png)

```
pclk1 = (SystemClk/preAHB)/preAPB1;
return pclk1;
```
- Tính toán PCLK2 cũng tương tự như PCLK1:
![4](/7_RCC/4.png)

![6](/7_RCC/6.png)
