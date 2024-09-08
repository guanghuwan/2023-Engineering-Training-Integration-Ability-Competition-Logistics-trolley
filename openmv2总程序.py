# Untitled - By: thinkpad - 周六 9月 16 2023
import sensor, image, time

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

red_threshold = (0, 100, 0, 127, 0, 127)
blue_threshold = (0, 78, -108, 0, -128, 0)
green_threshold = (0, 100, -100, 20, 20, 127)
k=1.3
int read1=uart.read()
while(read==1):
    clock.tick()
    img = sensor.snapshot().lens_corr(1.8)
    # 使用find_blobs方法来寻找符合阈值的区域，pixels_threshold和area_threshold可以过滤掉太小的区域
    blobs = img.find_blobs([blue_threshold], pixels_threshold=100, area_threshold=100)
    # 如果找到了至少一个区域，就按照面积从大到小排序
    if blobs:
        blobs.sort(key=lambda b: b.area(), reverse=True)
        # 取出最大的区域，即第一个元素
        max_blob = blobs[0]
        # 用方框框出最大的区域
        img.draw_rectangle(max_blob.rect(), color=(255, 255, 255))
        # 在最大区域的中心画一个十字，方便观察
        img.draw_cross(max_blob.cx(), max_blob.cy(), color=(255, 255, 255))
        #print("FPS %f" % clock.fps())
        while(blob.w()*k<blob.h()):
            uart.write(1)#令arduino驱动车右移一丢丢
        while(blob

