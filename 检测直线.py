enable_lens_corr = False # turn on for straighter lines...打开以获得更直的线条…

import sensor, image, time

sensor.reset()
sensor.set_pixformat(sensor.RGB565) # 灰度更快
sensor.set_framesize(sensor.QQVGA)
sensor.set_windowing((50,10,80,50)) #取右下角的160*120区域
sensor.skip_frames(time = 2000)
clock = time.clock()
global xi
xi=0

theta_margin = 10 # 设置与平行方向的最大角度差异

while(True):
    clock.tick()
    img = sensor.snapshot()
    if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...

    # `merge_distance`控制附近行的合并。 在0（默认），没有合并。
    # 在1处，任何距离另一条线一个像素点的线都被合并...等等，
    # 因为你增加了这个值。 您可能希望合并线段，因为线段检测会产生大量
    # 的线段结果。

    # `max_theta_diff` 控制要合并的任何两线段之间的最大旋转差异量。
    # 默认设置允许15度。

    for l in img.find_line_segments(merge_distance = 0, max_theta_diff = 5,min_length =500):
        if abs(l.theta() - 90) <= theta_margin: # 只选择与平行方向角度正负10度的直线
            img.draw_line(l.line(), color = (255, 0, 0))
            xi=1
    if xi=0:
        print(xi)
    xi=0


   # print("FPS %f" % clock.fps())
