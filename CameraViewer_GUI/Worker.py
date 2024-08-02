
import sys
import cv2
import numpy as np
from enum import Enum
import serial
import time
from img_show import *
import threading
from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale, QThread, Signal,
    QMetaObject, QObject, QPoint, QRect, QTimer,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QComboBox, QFrame, QLabel,QFileDialog,
    QMainWindow, QMenuBar, QPushButton, QSizePolicy,QMessageBox,QErrorMessage,
    QStatusBar, QTextEdit, QWidget)

class mode(Enum):
    getframe = 1,
    getframes = 2,
    stopframe = 3,
    saveImage = 4

class Worker(QThread):
    def __init__(self, mode, *args, **kwargs):
        super().__init__()
        self.worker_object = WorkerObject(mode, *args, **kwargs)
        self.worker_object.moveToThread(self)

    def run(self):
        self.worker_object.start_task()


class WorkerObject(QObject):
    image_signal = Signal(np.ndarray)
    raw_signal = Signal(list)
    error = Signal(str)
    #finished = Signal()
    
    def __init__(self, button,*args,**kwargs):
        super().__init__()
        self.mode=button
        self.args=args
        self.kwargs=kwargs
        self.should_stop = False
        # Timer for timeout
        self.timeout_timer = None
        
    def start_task(self):
        self.run()
    
    def run(self):
        rgb_image=None
        raw=None
        if self.mode==mode.getframe:
            self.start_timeout_timer(10)#10 sec
            rgb_image,raw = self.getframe(*self.args,*self.kwargs)
            self.stop_timeout_timer()
        elif self.mode==mode.getframes:  
            rgb_image ,raw =self.getframes(*self.args,*self.kwargs)
        
        if (rgb_image is not None and rgb_image.size > 0) or (raw is not None and raw.size > 0):
            self.image_signal.emit(rgb_image)
            self.raw_signal.emit(raw)
    
    
    
    def getframe(self,port_enable,m_serial,m_width,m_height,m_format):
        # self.timeout_timer.start(10000)#10 sec
        # print("active:",self.timeout_timer.isActive())
        rgb_image = None 
        m_data = None 
        try: 
            if port_enable==False or m_serial==None:
                #QMessageBox.warning(self.parent_widget,"warning","serial port is not opened")
                self.error.emit("serial port is not opened")
                return None,None
                
            if m_serial.is_open==False:
                #QMessageBox.warning(self.parent_widget,"warning","serial port is not opened")
                self.error.emit("serial port is not opened")
                return None,None
            print("start\n")
            buf=""
            try:
                while not self.should_stop:
                    #print("stop flag1:",self.should_stop)
                    if m_serial.in_waiting > 0:
                        data= m_serial.read(m_serial.in_waiting).decode('ascii')
                        if data =="start":
                            while not self.should_stop:
                                #print("stop flag2:",self.should_stop)    
                                if m_serial.in_waiting > 0:
                                    buf += m_serial.read(m_serial.in_waiting).decode('ascii')
                                    if "finish" in buf:
                                        buf, _ = buf.split("finish", 1)
                                        m_data = [int(val, 16) for val in buf.split()]
                                        #print(self.m_data)
                                        data_,len_=raw2Img(m_width, m_height, m_data, m_format)
                                        if m_format==imagetype.RGB:
                                            rgb_image=RGB565_to_RGB888(data_, m_height, m_width)
                                        else: 
                                            rgb_image = yuv422_to_rgb(data_, m_width, m_height)
                                        break
                                elif self.should_stop : break
                            break
                    elif self.should_stop : break
                                   
                #plt.imshow(rgb_image)
                # cv2.imshow("img",rgb_image)
                # cv2.waitKey(0)
                # cv2.destroyAllWindows()
                # self.timeout_timer.stop() 
                # print("active:",self.timeout_timer.isActive())
                print("finished\n")     
                return rgb_image ,m_data  
            except serial.SerialException as e:
                self.error.emit(f"serial error occured:\n{e}")
                #QMessageBox.warning(self.parent_widget,"warning",f"serial error occured:\n{e}")
        except Exception as e:
            self.error.emit(f"An error occurred: {str(e)}")
            return None, None
    
    def getframes(self,port_enable,m_serial,m_width,m_height,m_format):
        # self.timeout_timer.start(10000)#10 sec
        # print("active:",self.timeout_timer.isActive())
        rgb_image = None 
        m_data = None 
        try: 
            if port_enable==False or m_serial==None:
                #QMessageBox.warning(self.parent_widget,"warning","serial port is not opened")
                self.error.emit("serial port is not opened")
                return None,None
                
            if m_serial.is_open==False:
                #QMessageBox.warning(self.parent_widget,"warning","serial port is not opened")
                self.error.emit("serial port is not opened")
                return None,None
            print("start\n")
            buf=""
            try:
                while not self.should_stop:
                    #print("stop flag1:",self.should_stop)
                    if m_serial.in_waiting > 0:
                        data= m_serial.read(m_serial.in_waiting).decode('ascii')
                        if data =="start":
                            while not self.should_stop:
                                #print("stop flag2:",self.should_stop)    
                                if m_serial.in_waiting > 0:
                                    buf += m_serial.read(m_serial.in_waiting).decode('ascii')
                                    if "finish" in buf:
                                        buf, _ = buf.split("finish", 1)
                                        m_data = [int(val, 16) for val in buf.split()]
                                        #print(self.m_data)
                                        data_,len_=raw2Img(m_width, m_height, m_data, m_format)
                                        if m_format==imagetype.RGB:
                                            rgb_image=RGB565_to_RGB888(data_, m_height, m_width)
                                        else: 
                                            rgb_image = yuv422_to_rgb(data_, m_width, m_height)
                                        break
                                elif self.should_stop : break
                            break
                    elif self.should_stop : break
                                   
                #plt.imshow(rgb_image)
                # cv2.imshow("img",rgb_image)
                # cv2.waitKey(0)
                # cv2.destroyAllWindows()
                # self.timeout_timer.stop() 
                # print("active:",self.timeout_timer.isActive())
                print("finished\n")     
                return rgb_image ,m_data  
            except serial.SerialException as e:
                self.error.emit(f"serial error occured:\n{e}")
                #QMessageBox.warning(self.parent_widget,"warning",f"serial error occured:\n{e}")
        except Exception as e:
            self.error.emit(f"An error occurred: {str(e)}")
            return None, None
    
    
    def start_timeout_timer(self, timeout):
        self.timeout_timer = threading.Timer(timeout, self.on_timeout)
        self.timeout_timer.start()

    def stop_timeout_timer(self):
        if self.timeout_timer:
            self.timeout_timer.cancel()
    
       
    def on_timeout(self):
        try:
            print('timeout')
            
            self.should_stop = True
            self.error.emit("frame read timed out.")
        except Exception as e:
            self.error.emit(f"An error occurred: {str(e)}")
            