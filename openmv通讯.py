# 导入必要的模块
import pyb
import time

# 创建一个串口对象，指定引脚7为TX（发送）端
uart = pyb.UART(3,115200)

# 定义一个字符串变量
message = "1700"

# 循环发送字符串到引脚7
while True:
    # 用write方法发送字符串，注意要转换为字节类型
    uart.write(message.encode())
    # 用print方法打印发送的字符串，方便调试
    print( message)
    # 等待一秒
    time.sleep(1)

    #uart.write("#000P1700T0500!")
    #uart.write("#000PVER")
