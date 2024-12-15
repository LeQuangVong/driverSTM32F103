#### EXTI
External interrupts (EXTI) allow the system to respond to external events triggered on GPIO pins. The EXTI controller provides interrupt lines for each GPIO pin, enabling the MCU to detect changes such as rising edges, falling edges, or both on external signals.
#### Sơ đồ khối chức năng của ngắt ngoài:
![4](/3_EXTI/4.png)
- Khối Edge detect circuit, kết nối trực tiếp với các chân I/O (Input line), dùng để phát hiện sườn lên/sườn xuống trên các chân I/O tương ứng được cài đặt chế độ ngắt ngoài.
- Hai khối Rising/Falling trigger selection register dùng để chọn sườn lên/sườn xuống hoặc cả hai để phát hiện ngắt.
- Khối Software Interrupt Event register dùng để tạo các ngắt/sự kiện bằng software.
- Interrupt mask register, dùng để cho phép che/không che ngắt. Hai chức năng ngắt ở trên (Software hoặc Edge Trigger) khi xảy ra sẽ cần kiểm tra xem ngắt có bị che hay không.
- Nếu ngắt xảy ra mà không bị che, chúng sẽ được xử lý bằng thanh ghi Pending Request Register. Và sau đó gửi yêu cầu ngắt tương ứng đến bộ điều khiển ngắt NVIC để chờ xử lý.
#### EXTI Lines
Each GPIO pin in STM32F103 is connected to an EXTI line. The STM32F103 has up to 16 EXTI lines (EXTI0 to EXTI15), which map to GPIO pins as follows:
![1](/3_EXTI/1.png)
These EXTI lines allow different GPIO ports to share the same external interrupt line.
- Chọn các line ngắt từ thanh ghi AFIO_EXTICR1 -> AFIO_EXTICR4
#### EXTI Trigger Conditions
- `Rising Edge`: When the signal transitions from `LOW` to `HIGH`.
- `Falling Edge`: When the signal transitions from `HIGH` to `LOW`.
- `Both Edges`: When the signal transitions on `both rising` and `falling` edges.
#### NVIC (Nested Vectored Interrupt Controller) Configuration
- là bộ điều khiển ngắt lồng nhau, là một ngoại vi của Core CortexM.
- Khi các ngắt lồng nhau xảy ra, bộ xử lí sẽ tạm dừng việc thực thi Interrupt handler có mức ưu tiên thấp hơn khi một interrupt hander có mức ưu tiên cao hơn xảy ra.
##### Bộ thanh ghi NVIC:
-  ISER - Interrupt Set-Enable Registers:
    Có 8 thanh ghi để kích hoạt ngắt, NVIC_ISER0 => NVIC_ISER7. Mỗi thanh ghi có kích thước là 32 bits => Mỗi thanh ghi có thể kích hoạt 32 ngắt khác nhau.
    ![5](/3_EXTI/5.png)
-  ICER - Interrupt Clear-Enable Registers:
    Có 8 thanh ghi để kích hoạt ngắt. NVIC_ICER0 => NVIC_ICER7. Mỗi thanh ghi có kích thước là 32 bits => Mỗi thanh ghi có thể tắt 32 ngắt khác nhau.
    ![6](/3_EXTI/6.png)
-   ISPR - Interrupt Set-Pending Registers:
    Có 8 thanh ghi để kích hoạt ngắt. NVIC_ISPR0 => NVIC_ISPR7, dùng để buộc ngắt vào trạng thái chờ xử lý. Bộ xử lý sẽ kiểm tra mức độ ưu tiên của các ngắt đang chờ xử lý để lần lượt thực thi chúng.
    ![7](/3_EXTI/7.png)
-   ICPR - Interrupt Clear-Pending Registers:
    Có 8 thanh ghi để kích hoạt ngắt. NVIC_ICPR0 => NVIC_ICPR7, dùng để loại bỏ trạng thái chờ xử lý. Đọc nó để xem ngắt nào đang chờ xử lý. 
    ![8](/3_EXTI/8.png)
-   IPR - Interrupt Priority Registers:
    NVIC_IPR0 => NVIC_IPR59 cung cấp một trường ưu tiên 8-bit cho mỗi ngắt và mỗi thanh ghi giữ 4 trường ưu tiên này. Các thanh ghi này có thể truy cập từng byte
    ![9](/3_EXTI/9.png)
##### Cách hoạt động NVIC với ngắt
- Khi có sự kiện ngắt xảy ra, ngoại vi sẽ kích hoạt một tín hiệu ngắt trên đường IRQ này.
- NVIC sẽ đặt nó vào trạng thái chờ - pending.
- Bộ NVIC sẽ lần lượt thực hiện các pending interrupts, tùy thuộc vào mức độ ưu tiên.
- Khi đến lượt của một Interrupt, NVIC sẽ gửi tín hiệu ngắt đến CPU.
- CPU nhận tín hiệu ngắt từ NVIC nếu ngắt tới có mức độ ưu tiên cao hơn ISR đang hoạt động.
- Khi ngắt tới được CPU chấp nhận, CPU sẽ lấy địa chỉ chương trình phục vụ ngắt tương ứng với IRQ number từ bảng Vector ngắt. Sau đó thực thi ngắt.
##### Interrupt Priority
- Interrupt Priority là gì?
    - Mức độ ưu tiên - Priority được coi là mức độ khẩn cấp của ngắt, tức là quy định ngắt nào cần được thực hiện trước.
    - Giá trị mức độ ưu tiên – priority value là thước đo mức độ khẩn cấp của ngắt, còn gọi là mức độ ưu tiên – priority levels.
    -  Với Cortex Mx, giá trị ưu tiên này càng nhỏ thì mức độ ưu tiên càng lớn. Chẳng hạn, các exception của hệ thống đa số có mức ưu tiên nhỏ hơn 0 (trong đó Reset Handler có priority value nhỏ nhất, tương đương với mức độ ưu tiên lớn nhất).
    - Nếu hai ngắt xảy ra cùng lúc thì NVIC sẽ tiếp nhận thực thi ngắt có mức độ ưu tiên cao hơn (priority value thấp hơn), và đưa ngắt còn lại vào trạng thái chờ - Pending.
- Interrupt Priority Registers:
    Mỗi thanh ghi NVIC_IPR chia làm 4 phần, mỗi phần 8 bit để kiểm soát mức độ ưu tiên cho mỗi IRQ. Chỉ một số bit được sử dụng để kiểm soát mức độ ưu tiên
    - Có 2 trường hợp: sử dụng 3 bit cao thì sẽ có 8 giá trị priority, sử dụng 4 bit cao thì sẽ có 16 giá trị priority.
