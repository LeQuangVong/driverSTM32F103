#### The key components SPI 
- SCK(Serial Clock): Một tính hiệu clock được tạo ra bởi master, đồng bộ quá trình truyền dữ liệu giữa master và slave.
- MOSI(Master Out Slave In): Đường dữ liệu được sử dụng để gửi dữ liệu từ master đến slave.
- MISO(Master In Slave Out): Đường dữ liệu được sử dụng để nhận dữ liệu từ slave đến master.
- SS(Slave Select):  Tín hiệu từ master để chọn slave sẽ kết nối. Khi SS xuống mức thấp thì slave được chọn sẵn sàng truyền hoặc nhận dữ liệu.
#### The important configuration for SPI
- Clock polarity(CPOL): Determines the idle state of the clock signal (SCK).
    - CPOL = 0: Clock ở mức thấp khi không hoạt động.
    - CPOL = 1: Clock ở mức cao khi không hoạt động.
- Clock phase(CPHA): Xác định cạnh xung clock mà dữ liệu được lấy mẫu.
    - CPHA = 0: Dữ liệu được lấy mẫu tại cạnh đầu tiên của xung clock (rising or falling depending on CPOL).
    - CPHA = 1: Dữ liệu được lấy mẫu tại cạnh thứ 2 của xung clock.

| CPOL | CPHA |                         Description                         |
|:----:|:----:|:-----------------------------------------------------------:|
|   0  |   0  | Dữ liệu được lấy mẫu ở cạnh lên đầu tiên, Clock bắt đầu ở mức thấp |
|   0  |   1  | Dữ liệu được lấy mẫu ở cạnh xuống đầu tiên, Clock bắt đầu ở mức thấp |
|   1  |   0  |  Dữ liệu được lấy mẫu ở cạnh lên đầu tiên, Clock bắt đầu ở mức cao |
|   1  |   1  | Dữ liệu được lấy mẫu ở cạnh xuống đầu tiên, Clock bắt đầu ở mức cao |

![7](/4_SPI/7.png)

- Clock speed(Frequency): Tốc độ chuyển đổi tín hiệu clock ảnh hưởng đến tốc độ truyền dữ liệu. Master điều khiển tần số xung nhịp nhưng tần số này phải nằm trong giới hạn được hỗ trợ bởi cả Master và Slave.
- Data Frame Format:Phụ thuộc vào số bit của mỗi khung truyền (typically 8 or 16 bits).
- LSB/MSB first: Xác định xem bit có trọng số thấp (LSB) hay bit có trọng số cao (MSB) truyền trước.

#### How to transfer data works
- Clock synchronization (SCK):
    - Master tạo tín hiệu clock để đồng bộ giao tiếp. Mỗi cạnh lên hay cạnh xuống ( tùy thuộc vào cấu hình CPOL và CPHA) sẽ dịch chuyển 1 bit dữ liệu trong shift register của cả master và slave.
    - Tín hiệu được chia sẻ ở cả master và slave, đảm bảo dữ liệu được truyền ở cùng tốc độ theo cả 2 hướng.
- Shift register slave and master:
    - Shift register là một bộ phận chính nơi mà dữ liệu được lưu trữ và truyền từng bit một.
    - Shift register của master giữ data để truyền đến slave.
    - Shift register của slave giữ data để truyền đến master.
    - Mỗi xung Clock dịch chuyển một bit từ Shift register của Master ra ngoài thông qua MOSI, đồng thời dịch chuyển một bit từ Shift register của Slave ra ngoài thông qua MISO.
- Data transfer process:
    - Khi master chọn slave bằng cách kéo chân SS của slave đó xuống mức thấp thì quá trình giao tiếp bắt đầu.

    ![1](/4_SPI/1.png)

    - Mỗi xung clock, một bit từ Shift register của master được truyền thông qua MOSI đến slave.
    - Đồng thời, 1 bit từ Shift register được truyền thông qua MISO đến master.

    ![2](/4_SPI/2.png)

    - Quá trình được tiếp tục cho đến khi tất cả các bits (8 bits hoặc 16 bits) được truyền xong.
#### SPI bus configuration discussion: Full duplex, Half-duplex, and Simplex
- Full duplex:
    - Giao tiếp 2 chiều đồng thời (gửi và nhận dữ liệu cùng một lúc).

![3](/4_SPI/3.png)

- Half-duplex:
    - Giao tiếp truyền nhận dữ liệu không thể xảy ra đồng thời, một thời điểm chỉ có thể truyền hoặc nhận dữ liệu.
    - Cần điện trở để đảm bảo tránh trạng thái lơ lửng khi không có thiết bị nào truyền nhận, giữ dây ở trạng thái cố định mức cao hoặc mức thấp, đảm bảo tín hiệu rõ ràng, giảm nguy cơ xung đột tín hiệu.

![4](/4_SPI/4.png)

- Simplex:
    - Chê độ này thì chỉ truyền hoặc chỉ nhận, master ở chế độ truyền, slave ở chế độ nhận hoặc master ở chế độ nhận, slave ở chế độ truyền.
    - Các chân SS ở tất cả chế độ tùy chọn vì đang xét chỉ giao tiếp 1 Slave.

![5](/4_SPI/5.png)
#### SPI block diagram
![6](/4_SPI/6.png)

- SPI có 4 chân là chân NSS, chân SCK, chân MISO, chân MOSI
- SPI có một Shift register, độ rộng của shift register có thể lên đến 16 bits. Do đó SPI hỗ trợ truyền dữ liệu 16 bits. Thanh ghi dịch được kết nối với các đường MISO và MOSI.
- Nếu SPI làm việc ở Master thì dữ liệu sẽ được chuyển ra khỏi Shift register và nó sẽ đi ra qua MOSI. Nếu SPi đang nhận dữ liệu thì dữ liệu từ bên ngoài thông qua MISO và nó sẽ đưa vào shift register. Baud rate điều khiển chân Clock nếu là master.Baud rate được cấu hình bởi các bit BR[2:0] trong thanh ghi điều khiển SPI_CR1.
- Có 2 bộ nhớ đệm là TX buffer và Rx buffer.
- Các bộ đệm này có thể được truy cập từ APB bus. Bất cứ khi nào Shift register nhận được dữ liệu thì dữ liệu đó sẽ được chuyển sang bộ đệm Rx. Sau đó đọc dữ liệu từ bộ đệm Rx. Để truyền một số dữ liệu, dữ liệu đó sẽ ghi vào bộ đệm Tx.
- Nếu Shift register không có dữ liệu có nghĩa là SPI không bận, mọi dữ liệu được truyền tải vào bộ đệm Tx sẽ được chuyển sang Shift register và quá trình truyền tải bắt đầu. Từ Shift register dữ liệu sẽ truyền hết ở chân MOSI.
#### Master Mode:
![8](/4_SPI/8.png)
```
temp |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

//serial clock speed (baud rate)
temp |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

//Software Slave Management (SSM)
if(pSPIHandle->SPIConfig.SPI_SSM == SPI_SSM_EN)
{
    temp |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;
}
else
{
    temp &= ~(pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM);
}

if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
{
    //Full-duplex: BIDIMODE = 0
    temp &=~ (1 << SPI_CR1_BIDIMODE);
}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
{
    //Half-duplex: BIDIMODE = 1
    temp |= (1 << SPI_CR1_BIDIMODE);
}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX)
{
    //Simplex receive only: BIDIMODE = 0, RXONLY = 1
    temp &=~ (1 << SPI_CR1_BIDIMODE);
    temp |= (1 << SPI_CR1_RXONLY);
}

//Data frame format (DFF): 8bits: DFF = 0, 16bits: DFF = 1
temp |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

//Clock polarity (CPOL)
temp |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

//Clock phase (CPHA)
temp |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

pSPIHandle->pSPIx->CR1 = temp;
```
#### Slave Mode:
![9](/4_SPI/9.png)

```
temp &= ~(pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR);
temp &= ~(pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM);
if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
{
    //Full-duplex: BIDIMODE = 0
    temp &=~ (1 << SPI_CR1_BIDIMODE);
}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
{
    //Half-duplex: BIDIMODE = 1
    temp |= (1 << SPI_CR1_BIDIMODE);
}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX)
{
    //Simplex receive only: BIDIMODE = 0, RXONLY = 1
    temp &=~ (1 << SPI_CR1_BIDIMODE);
    temp |= (1 << SPI_CR1_RXONLY);
}

//Data frame format (DFF): 8bits: DFF = 0, 16bits: DFF = 1
temp |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

//Clock polarity (CPOL)
temp |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

//Clock phase (CPHA)
temp |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

pSPIHandle->pSPIx->CR1 = temp;
```
#### SPI Transmit:
Kiểm tra cờ TXE trong thanh ghi SPI_SR có được set lên hay chưa, nếu được set lên thì bắt đầu chuyển dữ liệu từ Tx buffer qua Shift register.
```
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//wait until TXE Flag is set (TXE = 1)
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		//16bits: DFF = 1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			//access to 16bits(2 byte) data at the memory address pTxBuffer points to.
			pSPIx->DR = *((uint16_t *)pTxBuffer);
			Len--;//8bit
			Len--;//8bit
			(uint16_t *)pTxBuffer++;//2 bytes
		}
		else
		{
			//8bits DFF = 0
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}
}
```
#### SPI Receive:
Kiểm tra cờ RXNE trong thành ghi SPI_SR, nếu được set lên thì dữ liệu đã nhận xong và sẵn sàng đọc từ thanh ghi DR.
```
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			*((uint16_t *)pRxBuffer) = pSPIx->DR;
			Len--;
			Len--;
			(uint16_t *)pRxBuffer++;
		}
		else
		{
			*pRxBuffer = pSPIx->DR;
			Len--;
			pRxBuffer++;
		}
	}
}
```
#### SPI Interrupt
##### Nguyên nhân gây ra ngắt:
- TXE (Transmit Empty): Bộ đệm truyền trống, có thể tiếp nhận dữ liệu mới để gửi:
	- cờ TXE trong thanh ghi SPI_SR.
	- cờ ngắt TXEIE trong thanh ghi SPI_CR2.
- RXNE (Receive Not Empty): Bộ đệm nhận có dữ liệu mới từ SPI, sẵn sàng để đọc.
	- cờ RXNE trong thanh ghi SPI_SR.
	- cờ ngắt RXNEIE trong thanh ghi SPI_CR2.
- MODF (Mode fault): Lỗi chế độ khi Master và Slave không đồng bộ.
	- cờ MODF trong thanh ghi SPI_SR.
	- cờ ngắt ERRIE trong thanh ghi SPI_CR2.
- OVR (Overrun Error): lỗi tràn bộ nhớ đệm nhận dữ liệu, không thể xử lí dữ liệu mới đến.
	- cờ OVR trong thanh ghi SPI_SR.
	- cờ ngắt ERRIE trong thanh ghi SPI_CR2.
- CRCERR (CRC Error): Lỗi CRC khi dữ liệu nhận không hợp lệ.
	- cờ CRCERR trong thanh ghi SPI_SR.
	- cờ ngắt ERRIE trong thanh ghi SPI_CR2.
##### Luồng hoạt động của chương trình ngắt SPI
- Khởi tạo truyền/nhận kích hoạt các ngắt liên quan.
- Khi một sự kiện xảy ra (TXE, RXNE, OVR, MODF, CRC):
	- CPU nhảy vào hàm xử lí ngắt SPI_IRQHandling.
	- Kiểm tra các trạng thái cờ trong SR và gọi các hàm xủ lí tương ứng.
	- Khi truyền/nhận xong, trạng thái SPI được reset và callback được gọi để thông báo.
- CPU tiếp tục công việc chính.
##### Transmit Empty (TXE)
![10](/4_SPI/11.png) 

- Thay vì CPU phải chờ trạng thái bộ đệm (TXE) để gửi từng byte dữ liệu, ngắt sẽ được sử dụng để xử lý từng byte dữ liệu trong nền, tiết kiệm tài nguyên CPU.

- Luồng hoạt động của hàm truyền dữ liệu qua ngắt:
	- Kiểm tra trạng thái hiện tại của SPI xem có đang thực hiện quá trình truyền khác không.
	- Nếu đang sẵn sàng:
		- Lưu con trỏ của bộ nhớ đệm cần truyền.
		- Lưu độ dài của dữ liệu cần truyền.
		- Cập nhật lại trạng thái đang bận truyền.
		- Kích hoạt ngắt TXE bằng bit TXEIE.
		- Trả về trạng thái bắt đầu truyền thành công
	- Nếu đang bận:
		- Trả về trạng thái bận.

```
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->RxState;
	if(state != SPI_BUSY_IN_RX)
	{
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;
		pSPIHandle->RxState = SPI_BUSY_IN_RX;
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);
	}
	return state;
}
```

##### Receive buffer not empty flag (RXNE)
![12](/4_SPI/12.png)

- Luồng hoạt động của hàm nhận dữ liệu qua ngắt:
	- Kiểm tra trạng thái hiện tại của SPI xem có đang thực hiện quá trình nhận nào khác không.
	- Nếu đang sẵn sàng:
		- Lưu con trỏ của bộ nhớ đệm nhận.
		- Lưu độ dài của dữ liệu nhận. 
		- Cập nhật lại trạng thái đang bận nhận.
		- Kích hoạt ngắt RXNE bằng bit RXNEIE.
		- Trả về trạng thái bắt đầu nhận thành công
	- Nếu đang bận:
		- Trả về trạng thái bận.

```
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->RxState;
	if(state != SPI_BUSY_IN_RX)
	{
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;
		pSPIHandle->RxState = SPI_BUSY_IN_RX;
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);
	}
	return state;
}
```

##### Hàm xử lí ngắt
- Kiểm tra trạng thái của các cờ trong SR và CR2, gọi hàm xử lí tương ứng

```
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp1, temp2;
	temp1 = pSPIHandle->pSPIx->SR & (1 << SPI_SR_TXE);//update Event flag

	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);//update Enable control bit

	if(temp1 && temp2)
	{
		SPI_TXE_Interrupt_Handle(pSPIHandle);
	}

	temp1 = pSPIHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if(temp1 && temp2)
	{
		SPI_RXNE_Interrupt_Handle(pSPIHandle);
	}

	temp1 = pSPIHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

	if(temp1 && temp2)
	{
		SPI_OVR_ERR_Interrupt_Handle(pSPIHandle);
	}
}
```
##### Xử lí ngắt TXE
- Luồng xử lí:
	- Kiểm tra xem dữ liệu được truyền theo khung 16bit hay 8 bit
	- Lấy giá trị tại địa chỉ mà con trỏ pTxBuffer đang trỏ tới:
		- Với khung truyền 16 bit thì lấy giá trị 2 byte 1 lần
		- Với khung truyền 8 bit thì lấy giá trị 1 byte 1 lần.
	- Nếu dữ liệu được truyền xong:
		- Sẽ thực hiện dừng quá trình truyền.
		- Thông báo quá trình truyền hoàn tất.
```
static void SPI_TXE_Interrupt_Handle(SPI_Handle_t *pSPIHandle)
{
	if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
	{
		pSPIHandle->pSPIx->DR = *((uint16_t *)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen --;
		pSPIHandle->TxLen --;
		(uint16_t *)pSPIHandle->pTxBuffer++;
	}
	else
	{
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen --;
		pSPIHandle->pTxBuffer++;
	}

	if(!pSPIHandle->TxLen)
	{
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT);
	}
}
```
##### Xử lí ngắt RXNE
- Luồng xử lí:
	- Kiểm tra xem dữ liệu được nhận theo khung 16bit hay 8 bit
	- Lấy địa chỉ con trỏ pRxBuffer và truy cập vùng bộ nhớ mà con trỏ trỏ tới, ghi dữ liệu vào đó:
		- Với khung truyền 16 bit thì ghi giá trị 2 byte 1 lần
		- Với khung truyền 8 bit thì ghi giá trị 1 byte 1 lần.
	- Nếu dữ liệu được nhận xong:
		- Sẽ thực hiện dừng quá trình nhận.
		- Thông báo quá trình nhận hoàn tất.

```
static void SPI_RXNE_Interrupt_Handle(SPI_Handle_t *pSPIHandle)
{
	if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
	{
		*((uint16_t *)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen --;
		pSPIHandle->RxLen --;
		(uint16_t *)pSPIHandle->pRxBuffer++;
	}
	else
	{
		*(pSPIHandle->pRxBuffer) = (uint8_t)pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen --;
		pSPIHandle->pRxBuffer++;
	}

	if(!pSPIHandle->RxLen)
	{
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);
	}
}
```

##### Xử lí ngắt Overrun ERROR
![13](/4_SPI/13.png)

- Lỗi xảy ra khi dữ liệu mới ghi vào thanh ghi dữ liệu (DR) trước khi dữ liệu cũ được đọc. Điều này thường liên quan đến SPI Slave, khi Master gửi dữ liệu liên tiếp mà Slave không đọc kịp.
- Xử lí bằng cách:
	- Đọc thanh ghi DR và SR để xóa lỗi.
	- Thông báo xóa lỗi xong để CPU tiếp tục chươn trình.
- Để tiếp tục chương trình thì cần yêu cầu master tiếp tục gửi dữ liệu lại.
```
static void SPI_OVR_ERR_Interrupt_Handle(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp;
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
		(void)temp;// Tránh cảnh báo của trình biên dịch là biến temp được khai báo nhưng không được sử dụng.
	}
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}
```
##### Kết thúc quá trình ngắt SPI
- Kết thúc quá trình truyền dữ liệu bằng ngắt:
	- tắt ngắt TXEIE
	- Đặt con trỏ truyền dữ liệu về NULL
	- Đặt độ dài của dữ liệu truyền dữ liệu về 0
	- Đặt trạng thái của quá trình truyền về trạng thái sẵn sàng
	```
	void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
	{
	pSPIHandle->pSPIx->CR2 &=~ (1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
	}
	```
- Kết thúc quá trình nhận dữ liệu bằng ngắt:
	- Tắt ngắt RXNEIE.
	- Đặt con trỏ nhận dữ liệu về NULL
	- Đặt độ dài của dữ liệu nhận về 0
	- Đặt trạng thái của quá trình nhận về trạng thái sẵn sàng.

	```
	void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
	{
	pSPIHandle->pSPIx->CR2 &=~ (1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
	}
	```
	
