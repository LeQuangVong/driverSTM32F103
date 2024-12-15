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

- RTS của một thiết bị được kết nối với CTS của thiết bị khác. Khi thiết bị thứ nhất muốn dữ liệu từ thiết bị thứ hai thì RTS sẽ được kéo xuống mức thấp ở thiết bị thứ nhất, điều này kiến thiết bị thứ hai truyền dữ liệu trên TX bằn cách kéo CTS của nó xuống mức thấp.
#### UART frame format
- Start bit (1 bit): The start bit signals the beginning of a data frame. When a device begins transmitting data, it pulls the signal low (0) for a duration equal to one bit.
- Data bits (5-9 bits): The data bits form the core of the frame, containing the actual information being transmitted
- Parity bit (1 bit, optional): provides error checking.
    - Even parity: The parity bit is set the total number of `1` bits in the frame is even.
    - Odd parity: The pairty bit is set the total number of `1` bits in the frame is odd.
    - No parity: Parity can be disabled if error checking is not equired.
- Tính toán dựa trên các bit dữ liệu trong byte để đảm bảo một lượng bit 1 nhất định.
![2](/5_UART/2.png)

- Stop bit (1 bit or 2 bits): The stop bit masks the end of a data frame. The signal is pulled high (1) for a duration of 1 or 2 bits, depending on configuration.
#### UART Baud rate
- The baud rate is the rate at which data is transmitted between devices, defines in bit per second (bps). It represents the number of bits transmitted per second, including start, data, parity, and stop bits. A higher baud rate means faster data transfer, but if set too high, it may lead to data errors due to syschronization issues.
- UART supports several standard baud rates, including 9600, 19200, 38400, 57600, 115200.
- Tốc độ baud càng cao thì dữ liệu được gửi hoặc nhận càng nhanh. Nhưng có những giới hạn về tốc độ truyền dữ liệu. Tốc độ truyền thường phụ thuộc vào tần số xung nhịp ngoại vi của thiết bị ngoại vi UART.
- Tốc độ Baud có thể là bất kỳ giá trị nào. Yêu cầu duy nhất là cả thiết bị truyền và thiết bị thu phải hoạt động ở cùng tốc độ.

#### UART block diagram
![3](/5_UART/3.png)

- Chân TX, RX dùng để truyền hoặc nhận dữ liệu, còn chấn RTS, CTS dùng để điều kiển luồng phần cứng.
- Có 2 thanh ghi dữ liệu, một thanh ghi truyền dữ liệu và một thanh ghi nhận dữ liệu, mỗi thanh ghi đều có Shift register.
- Có vài thanh ghi điều khiển có các bit điều khiển khác nhau được sử dụng điều khiển các khối truyền, khối nhận cũng như khối ngắt.
- Khối tạo tốc độ truyền, được sử dụng để tạo tốc độ truyền thích hợp 
#### UART peripheral Clock
![4](/5_UART/4.png)

- USART1 kết nối với bus APB2 nên tốc độ tối đa là 72MHz.

![5](/5_UART/5.png)

- USART2, USART3 kết nối với bus APB1 nên tốc độ tối đa là 36MHz
#### UART Transmit
- Enable USART bằng set bit UE trong thanh ghi USART_CR1 lên 1.
- Lập trình bit M trong thanh ghi USART_CR1 để xác định độ dài dữ liệu.
- Chọn stop bits trong thanh ghi USART_CR2.
- Chọn tốc độ truyền mong muốn bằng thanh ghi USART_BRR
- Set cờ TE trong thanh ghi USART_CR1 ghi dữ liệu vào thanh ghi USART_DR, lặp lại điều này cho mỗi lần truyền dữ liệu đi.
- Đợi cho đến khi cờ TC được set lên 1 thì quá trình truyền dữ liệu kết thúc. Nếu muốn disable USART thì nó phải thực hiện điều đó sau khi bit TC được đặt.

```
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint16_t Data;

	for(uint32_t i = 0; i < Len; i++)
	{
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TXE));

		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			//Lấy giá trị mà con trỏ đang trỏ tới để đưa vào thanh ghi DR
			Data = *(uint16_t *)pTxBuffer;
			pUSARTHandle->pUSARTx->DR = (Data & 0x01FF);

			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
				pTxBuffer++;
			}
		}
		else
		{
			pUSARTHandle->pUSARTx->DR = (*pTxBuffer & 0xFF);

			pTxBuffer++;
		}
	}

	while(!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TC));
}
```

#### UART Receive
- Enable USART bằng set bit UE trong thanh ghi USART_CR1 lên 1.
- Lập trình bit M trong thanh ghi USART_CR1 để xác định độ dài dữ liệu.
- Chọn stop bits trong thanh ghi USART_CR2.
- Chọn tốc độ truyền mong muốn bằng thanh ghi USART_BRR
- Set cờ RE trong thanh ghi USART_CR1 để enable khối nhận của USART, nó bắt đầu tìm kiếm bit START
- Nhận được kí tự thì đợi cho bit RXNE được thiết lập. Nếu bit RXNE được thiết lập thì dữ liệu của Shift register được chuyển sang thanh ghi receive.
- Ngắt được tạo nếu bit RXNEIE được thiết lập.
- Xóa bit RXNE được thực hiện bằng cách đọc thanh ghi USART_DR hoặc đặt trực tiếp bit RXNE bằng 0
- Bit RXNE phải được xóa trước khi kết thúc quá trình nhận kí tự tiếp theo để tránh lỗi overrun.

```
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	for(uint32_t i = 0; i < Len; i++)
	{
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE));

		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//Lưu giá trị thanh ghi DR vào vùng nhớ mà con trỏ đang trỏ tới
				*((uint16_t *)pRxBuffer) = (pUSARTHandle->pUSARTx->DR & (uint16_t)0x01FF);

				pRxBuffer++;
				pRxBuffer++;
			}
			else
			{
				*pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
				pRxBuffer++;
			}
		}
		else
		{
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				*pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
			}
			else
			{
				*pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0x7F);
			}

			pRxBuffer++;
		}
	}
}
```

#### UART Baurate
![6](/5_UART/6.png)

Ví dụ:

Tạo ra Baurate là 9600, fCk = 8MHz.
Vậy USARTDIV = 8/(9600*16).

![7](/5_UART/7.png)

Thanh ghi USART_BRR có 2 phần Fraction và Mantissa

Ví dụ: 104.1875 thì Fraction là 1875, Mantissa là 104

- Phần nguyên chúng ta chỉ cần ép kiểu sang uint32_t là được, sau đó dịch sang trái 4 bit
- Phần thập phân thì chúng ta lấy USARTDIV - phần nguyên, rồi nhân với 16, thêm vào 4 bit sau là được

```
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate)
{
	uint32_t PCLKx;
	float usartdiv;
	uint32_t M_part, F_part;

	uint32_t temp = 0;

	if(pUSARTx == USART1)
	{
		PCLKx = RCC_GetPCLK2Value();
	}else
	{
		PCLKx = RCC_GetPCLK1Value();
	}

	//sample

	usartdiv = (PCLKx/(16*BaudRate));

	M_part = (uint32_t)usartdiv;

	temp |= M_part << 4;

	F_part = usartdiv - M_part;
	F_part = (uint32_t)(F_part * 16);
	temp |= (F_part & 0xF);

	pUSARTx->BRR = temp;
}
```
#### UART Interrupt
##### Nguyên nhân gây ra ngắt
- Ngắt nhận dữ liệu (RXNE - Receive Not Empty Interrupt): Ngắt xảy ra khi dữ liệu mới được nhận và thanh ghi dữ liệu (DR) không còn rỗng.
	- Bit RXNE trong thanh ghi USART_SR.
	- Bit RXNEIE trong thanh ghi USART_CR1.
- Ngắt truyền dữ liệu (TXE - Transmit Data Register Empty Interrupt): Ngắt xảy ra khi thanh ghi dữ liệu (DR) trống và sẵn sàng nhận dữ liệu mới để truyền.
	- Bit TXE trong thanh ghi USART_SR.
	- Bit TXEIE trong thanh ghi USART_CR1.
- Ngắt truyền hoàn tất (TC - Transmission Complete Interrupt): Ngắt xảy ra khi dữ liệu cuối cùng trong thanh ghi dịch chuyển hoàn toàn ra khỏi bus truyền. Xóa bằng cách đọc thanh ghi SR và ghi 0 vào cờ TC.
	- Bit TC trong thanh ghi USART_SR.
	- Bit TCIE trong thanh ghi USART_CR1.
- Lỗi Parity (PE - Parity Error): Ngắt xảy ra khi phát hiện lỗi parity trong dữ liệu nhận. Đọc thanh ghi SR và DR để xóa lỗi.
	- Bit PE trong thanh ghi USART_SR.
	- Bit PEIE trong thanh ghi USART_CR1.
- Lỗi Overrun (ORE - Overrun Error): Xảy ra khi dữ liệu mới nhận ghi đè dữ liệu cũ chưa được đọc trong DR. Đọc thanh ghi DR để xóa lỗi.
	- Bit ORE trong thanh ghi USART_SR.
	- được xử lý trong ngắt nhận (RXNEIE).
- Lỗi Noise (NF - Noise Flag Error):
	- Bit NF trong thanh ghi USART_SR (bit 2).
	- được xử lý trong ngắt nhận (RXNEIE). Đọc thanh ghi SR và DR để xóa lỗi. 
- Ngắt IDLE (IDLE Line Detection Interrupt): Xảy ra khi phát hiện đường truyền ở trạng thái IDLE (không hoạt động trong khoảng thời gian xác định). Đọc thanh ghi SR và DR để xóa lỗi
	- Bit IDLE trong thanh ghi USART_SR.
	- Bit IDLEIE trong thanh ghi USART_CR1.

##### Luồng hoạt động của chương trình ngắt UART
- Khởi tạo truyền/nhận kích hoạt các ngắt liên quan.
- Khi một sự kiện xảy ra (TXE, CTS, RXNE, OVR, IDLE, PE, ...):
	- CPU nhảy vào hàm xử lí ngắt USART_IRQHandling.
	- Kiểm tra các trạng thái cờ trong SR và gọi các hàm xủ lí tương ứng.
	- Khi truyền/nhận xong, trạng thái USART được reset và callback được gọi để thông báo.
- CPU tiếp tục công việc chính.
