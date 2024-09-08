# Untitled - By: thinkpad - 周五 8月 18 2023

import sensor, image, time
from pyb import UART
Mo=0#模式变换的数字

def MOSHI():
    if Mo=0#当为模式1时
        sensor.reset()
        sensor.set_pixformat(sensor.RGB565)
        sensor.set_framesize(sensor.QQVGA) # can be QVGA on M7...
        sensor.skip_frames(30)
        sensor.set_auto_gain(False) # must turn this off to prevent image washout...
        uart = UART(3, 115200)
        name=0

        while(not name):
            img = sensor.snapshot()
            img.lens_corr(1.8) # strength of 1.8 is good for the 2.8mm lens.
            res = img.find_qrcodes() #寻找二维码
            if len(res) > 0:
                img.draw_string(2,2, res[0].payload(), color=(0,128,0), scale=2)
                print(res[0].payload())
                name=res[0].payload()
            str(name)
            uart.write(name)
            uart.write("\n")
            time.sleep_ms(1000)
            Mo=Mo+1
            break#从循环中跳出
