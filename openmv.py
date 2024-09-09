<<<<<<< HEAD
enable_lens_corr = False # turn on for straighter lines...打开以获得更直的线条…

import sensor, image, time,pyb#包含模块
from pyb import UART

#物料阈值数组
rgb_thr = [(14, 69, 19, 89, -40, 44),  # 红阈值
           (12, 91, -80, -13, -17, 34),  # 绿阈值
           (15, 82, -23, 19, -58, -12)] # 蓝阈值

#########################################################

max_S=45000 #色块的外框的面积
min_S=15000   #色块的外框的面积

a = pyb.Pin("P1", pyb.Pin.IN)
b = pyb.Pin("P2", pyb.Pin.IN)
c = pyb.Pin("P3", pyb.Pin.IN)

uart = pyb.UART(3, 9600, timeout_char = 100)#创建串口对象
uart.init(9600, bits=8, parity=None, stop=1, timeout_char=1000) # 使用给定参数初始化

led_red=pyb.LED(1)#创建一个LED对象
led_green=pyb.LED(2)#创建一个LED对象
led_blue=pyb.LED(3)#创建一个LED对象

led_red.on()
led_green.on()
led_blue.on()

#sensor.reset() #初始化相机传感器
#sensor.set_pixformat(sensor.RGB565) #灰度更快
#sensor.set_framesize(sensor.QVGA)
#sensor.skip_frames(time = 2000)


sensor.reset()# 初始化传感器
sensor.set_pixformat(sensor.RGB565)#图像格式设置为彩色，每个像素16bit
sensor.set_framesize(sensor.QVGA)#图像像素320x240
sensor.skip_frames(time = 2000)#跳过2000帧，等待感光元件变稳定
clock = time.clock()

def red_circle():#识别圆
    for c in img.find_circles(roi=(30,0,290,240),threshold = 3300, x_margin = 10, y_margin = 10, r_margin = 10,r_min = 20, r_max = 30, r_step = 2):
        img.draw_circle(c.x(), c.y(), c.r(), color = (255, 0, 0))
        print(c)
        print('cir 1 x:',c.x(),'y:',c.y(),'r:',c.r())
    #for c in img.find_circles(roi=(30,0,290,240),threshold = 3000, x_margin = 20, y_margin = 20, r_margin = 30,
            #r_min = 55, r_max = 100, r_step = 2):
        #area = (c.x()-c.r(), c.y()-c.r(), 2*c.r(), 2*c.r())
        ##area为识别到的圆的区域，即圆的外接矩形框
        ##statistics = img.get_statistics(roi=area)#像素颜色统计
        ##print(statistics)
        #print('x:',c.x(),'y:',c.y(),'r:',c.r())

        #img.draw_rectangle(area, color = (255, 255, 255))            #将圆用白色的矩形框出来

        uart.write('A'+str(c.x())+','+str(c.y())+','+str(c.r())+'\r\n')
        return 1

def line(): #识别直线
    for l in img.find_line_segments(roi=[0,20,320,180],merge_distance = 10, max_theta_difference  = 2):
        if (l.length() >= 150):
            img.draw_line(l.line(), color = (255, 0, 0))
            print(l)
            print("l.theta:",l.theta())#打印坐标
            uart.write('A'+str(l.theta())+','+str(l.rho())+'\r\n')
            return 1


def wedges_1():#识别物块颜色,处理后串口发送
    i=0                      #area_threshold 面积阈值  pixels_threshold 像素个数阈值  （小于会被滤掉）
    #for blob in img.find_blobs([rgb_thr[0]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):#for循环把所有的色块找一遍，margin 合并
        #if blob.area()<max_S and blob.area()>min_S and blob.cx()>120 and blob.cx()<200 and blob.cy()>110 and blob.cy()<200:#面积判断 blob.area() 返回色块的外框的面积
            #img.draw_rectangle(blob.rect())#红  在图像中画一个矩形框  blob.rect() 返回这个色块的外框——矩形元组(x, y, w, h)，可以直接在image.draw_rectangle中使用
            #img.draw_cross(blob.cx(), blob.cy()) # 在图像中画一个十字
            #print("面积:",blob.area())
            #print("red:(",blob.cx(),",",blob.cy(),")")#打印坐标
            #i=1
            #uart.write('A'+str(i)+','+str(blob.cx())+','+str(blob.cy())+'\r\n')

    #for blob in img.find_blobs([rgb_thr[1]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):
        #if blob.area()<max_S and blob.area()>min_S and blob.cx()>120 and blob.cx()<200 and blob.cy()>110 and blob.cy()<200:#面积判断 blob.area() 返回色块的外框的面积
            #img.draw_rectangle(blob.rect())#绿
            #img.draw_cross(blob.cx(), blob.cy())
            #print("面积:",blob.area())
            #print("green:(",blob.cx(),",",blob.cy(),")")#打印坐标
            #i=2
            #uart.write('A'+str(i)+','+str(blob.cx())+','+str(blob.cy())+'\r\n')

    #for blob in img.find_blobs([rgb_thr[2]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):
        #if blob.area()<max_S and blob.area()>min_S and blob.cx()>120 and blob.cx()<200 and blob.cy()>110 and blob.cy()<200:#面积判断 blob.area() 返回色块的外框的面积
            #img.draw_rectangle(blob.rect())#蓝
            #img.draw_cross(blob.cx(), blob.cy())
            #print("面积:",blob.area())
            #print("blue:(",blob.cx(),",",blob.cy(),")")#打印坐标
            #i=3
            #uart.write('A'+str(i)+','+str(blob.cx())+','+str(blob.cy())+'\r\n')

    for blob in img.find_blobs([rgb_thr[0]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):#for循环把所有的色块找一遍，margin 合并
        if blob.area()<max_S and blob.area()>min_S :#面积判断 blob.area() 返回色块的外框的面积
            img.draw_rectangle(blob.rect())#红  在图像中画一个矩形框  blob.rect() 返回这个色块的外框——矩形元组(x, y, w, h)，可以直接在image.draw_rectangle中使用
            img.draw_cross(blob.cx(), blob.cy()) # 在图像中画一个十字
            print("面积:",blob.area())
            print("red:(",blob.cx(),",",blob.cy(),")")#打印坐标
            i=1
            uart.write('A'+str(i)+','+str(blob.cx())+','+str(blob.cy())+'\r\n')

    for blob in img.find_blobs([rgb_thr[1]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):
        if blob.area()<max_S and blob.area()>min_S :#面积判断 blob.area() 返回色块的外框的面积
            img.draw_rectangle(blob.rect())#绿
            img.draw_cross(blob.cx(), blob.cy())
            print("面积:",blob.area())
            print("green:(",blob.cx(),",",blob.cy(),")")#打印坐标
            i=2
            uart.write('A'+str(i)+','+str(blob.cx())+','+str(blob.cy())+'\r\n')

    for blob in img.find_blobs([rgb_thr[2]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):
        if blob.area()<max_S and blob.area()>min_S :#面积判断 blob.area() 返回色块的外框的面积
            img.draw_rectangle(blob.rect())#蓝
            img.draw_cross(blob.cx(), blob.cy())
            print("面积:",blob.area())
            print("blue:(",blob.cx(),",",blob.cy(),")")#打印坐标
            i=3
            uart.write('A'+str(i)+','+str(blob.cx())+','+str(blob.cy())+'\r\n')
        #uart.write('A'+str(c.x())+','+str(c.y())+','+str(c.r())+'\r\n')
    #uart.write('A'+str(i)+','+str(blob.cx())+','+str(blob.cy())'\r\n')
    return i

while(True):
    b.value()=0 and c.value()=1
    img = sensor.snapshot().lens_corr(1.8)
    #wedges_1()    ##识别物块颜色,
    #red_circle()  #识别圆
    #time.sleep_ms(10)
    if b.value()==0 and c.value()==0 :
        i=line()  #识别直线
        time.sleep_ms(1)

    if b.value()==0 and c.value()==1 :
        i=wedges_1()    ##识别物块颜色,
        time.sleep_ms(10)

    if b.value()==1 and c.value()==0 :
        i=red_circle()  #识别圆
        time.sleep_ms(10)













=======
enable_lens_corr = False # turn on for straighter lines...打开以获得更直的线条…

import sensor, image, time,pyb#包含模块
from pyb import UART

#物料阈值数组
rgb_thr = [(14, 69, 19, 89, -40, 44),  # 红阈值
           (12, 91, -80, -13, -17, 34),  # 绿阈值
           (15, 82, -23, 19, -58, -12)] # 蓝阈值

#########################################################

max_S=45000 #色块的外框的面积
min_S=15000   #色块的外框的面积

a = pyb.Pin("P1", pyb.Pin.IN)
b = pyb.Pin("P2", pyb.Pin.IN)
c = pyb.Pin("P3", pyb.Pin.IN)

uart = pyb.UART(3, 9600, timeout_char = 100)#创建串口对象
uart.init(9600, bits=8, parity=None, stop=1, timeout_char=1000) # 使用给定参数初始化

led_red=pyb.LED(1)#创建一个LED对象
led_green=pyb.LED(2)#创建一个LED对象
led_blue=pyb.LED(3)#创建一个LED对象

led_red.on()
led_green.on()
led_blue.on()

#sensor.reset() #初始化相机传感器
#sensor.set_pixformat(sensor.RGB565) #灰度更快
#sensor.set_framesize(sensor.QVGA)
#sensor.skip_frames(time = 2000)


sensor.reset()# 初始化传感器
sensor.set_pixformat(sensor.RGB565)#图像格式设置为彩色，每个像素16bit
sensor.set_framesize(sensor.QVGA)#图像像素320x240
sensor.skip_frames(time = 2000)#跳过2000帧，等待感光元件变稳定
clock = time.clock()

def red_circle():#识别圆
    for c in img.find_circles(roi=(30,0,290,240),threshold = 3300, x_margin = 10, y_margin = 10, r_margin = 10,r_min = 20, r_max = 30, r_step = 2):
        img.draw_circle(c.x(), c.y(), c.r(), color = (255, 0, 0))
        print(c)
        print('cir 1 x:',c.x(),'y:',c.y(),'r:',c.r())
    #for c in img.find_circles(roi=(30,0,290,240),threshold = 3000, x_margin = 20, y_margin = 20, r_margin = 30,
            #r_min = 55, r_max = 100, r_step = 2):
        #area = (c.x()-c.r(), c.y()-c.r(), 2*c.r(), 2*c.r())
        ##area为识别到的圆的区域，即圆的外接矩形框
        ##statistics = img.get_statistics(roi=area)#像素颜色统计
        ##print(statistics)
        #print('x:',c.x(),'y:',c.y(),'r:',c.r())

        #img.draw_rectangle(area, color = (255, 255, 255))            #将圆用白色的矩形框出来

        uart.write('A'+str(c.x())+','+str(c.y())+','+str(c.r())+'\r\n')
        return 1

def line(): #识别直线
    for l in img.find_line_segments(roi=[0,20,320,180],merge_distance = 10, max_theta_difference  = 2):
        if (l.length() >= 150):
            img.draw_line(l.line(), color = (255, 0, 0))
            print(l)
            print("l.theta:",l.theta())#打印坐标
            uart.write('A'+str(l.theta())+','+str(l.rho())+'\r\n')
            return 1


def wedges_1():#识别物块颜色,处理后串口发送
    i=0                      #area_threshold 面积阈值  pixels_threshold 像素个数阈值  （小于会被滤掉）
    #for blob in img.find_blobs([rgb_thr[0]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):#for循环把所有的色块找一遍，margin 合并
        #if blob.area()<max_S and blob.area()>min_S and blob.cx()>120 and blob.cx()<200 and blob.cy()>110 and blob.cy()<200:#面积判断 blob.area() 返回色块的外框的面积
            #img.draw_rectangle(blob.rect())#红  在图像中画一个矩形框  blob.rect() 返回这个色块的外框——矩形元组(x, y, w, h)，可以直接在image.draw_rectangle中使用
            #img.draw_cross(blob.cx(), blob.cy()) # 在图像中画一个十字
            #print("面积:",blob.area())
            #print("red:(",blob.cx(),",",blob.cy(),")")#打印坐标
            #i=1
            #uart.write('A'+str(i)+','+str(blob.cx())+','+str(blob.cy())+'\r\n')

    #for blob in img.find_blobs([rgb_thr[1]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):
        #if blob.area()<max_S and blob.area()>min_S and blob.cx()>120 and blob.cx()<200 and blob.cy()>110 and blob.cy()<200:#面积判断 blob.area() 返回色块的外框的面积
            #img.draw_rectangle(blob.rect())#绿
            #img.draw_cross(blob.cx(), blob.cy())
            #print("面积:",blob.area())
            #print("green:(",blob.cx(),",",blob.cy(),")")#打印坐标
            #i=2
            #uart.write('A'+str(i)+','+str(blob.cx())+','+str(blob.cy())+'\r\n')

    #for blob in img.find_blobs([rgb_thr[2]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):
        #if blob.area()<max_S and blob.area()>min_S and blob.cx()>120 and blob.cx()<200 and blob.cy()>110 and blob.cy()<200:#面积判断 blob.area() 返回色块的外框的面积
            #img.draw_rectangle(blob.rect())#蓝
            #img.draw_cross(blob.cx(), blob.cy())
            #print("面积:",blob.area())
            #print("blue:(",blob.cx(),",",blob.cy(),")")#打印坐标
            #i=3
            #uart.write('A'+str(i)+','+str(blob.cx())+','+str(blob.cy())+'\r\n')

    for blob in img.find_blobs([rgb_thr[0]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):#for循环把所有的色块找一遍，margin 合并
        if blob.area()<max_S and blob.area()>min_S :#面积判断 blob.area() 返回色块的外框的面积
            img.draw_rectangle(blob.rect())#红  在图像中画一个矩形框  blob.rect() 返回这个色块的外框——矩形元组(x, y, w, h)，可以直接在image.draw_rectangle中使用
            img.draw_cross(blob.cx(), blob.cy()) # 在图像中画一个十字
            print("面积:",blob.area())
            print("red:(",blob.cx(),",",blob.cy(),")")#打印坐标
            i=1
            uart.write('A'+str(i)+','+str(blob.cx())+','+str(blob.cy())+'\r\n')

    for blob in img.find_blobs([rgb_thr[1]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):
        if blob.area()<max_S and blob.area()>min_S :#面积判断 blob.area() 返回色块的外框的面积
            img.draw_rectangle(blob.rect())#绿
            img.draw_cross(blob.cx(), blob.cy())
            print("面积:",blob.area())
            print("green:(",blob.cx(),",",blob.cy(),")")#打印坐标
            i=2
            uart.write('A'+str(i)+','+str(blob.cx())+','+str(blob.cy())+'\r\n')

    for blob in img.find_blobs([rgb_thr[2]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):
        if blob.area()<max_S and blob.area()>min_S :#面积判断 blob.area() 返回色块的外框的面积
            img.draw_rectangle(blob.rect())#蓝
            img.draw_cross(blob.cx(), blob.cy())
            print("面积:",blob.area())
            print("blue:(",blob.cx(),",",blob.cy(),")")#打印坐标
            i=3
            uart.write('A'+str(i)+','+str(blob.cx())+','+str(blob.cy())+'\r\n')
        #uart.write('A'+str(c.x())+','+str(c.y())+','+str(c.r())+'\r\n')
    #uart.write('A'+str(i)+','+str(blob.cx())+','+str(blob.cy())'\r\n')
    return i

while(True):
    b.value()=0 and c.value()=1
    img = sensor.snapshot().lens_corr(1.8)
    #wedges_1()    ##识别物块颜色,
    #red_circle()  #识别圆
    #time.sleep_ms(10)
    if b.value()==0 and c.value()==0 :
        i=line()  #识别直线
        time.sleep_ms(1)

    if b.value()==0 and c.value()==1 :
        i=wedges_1()    ##识别物块颜色,
        time.sleep_ms(10)

    if b.value()==1 and c.value()==0 :
        i=red_circle()  #识别圆
        time.sleep_ms(10)













>>>>>>> 71f28a362d2e1a2a0341dd9637061e1b1d6cb77a
