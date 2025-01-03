import sys
import cv2
import glob
import os
import time
import numpy as np
from enum import Enum
import serial

from img_show import *
from Worker import *


from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale, QThread, Signal,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QComboBox, QFrame, QLabel,QFileDialog,
    QMainWindow, QMenuBar, QPushButton, QSizePolicy, QMessageBox, QErrorMessage, QDialog,
    QStatusBar, QTextEdit, QWidget)

      

class Ui_MainWindow(object):
    port_enable=False
    m_serial=None
    m_data=[]
    m_width=160
    m_height=120
    m_resolution=resolution.QQVGA
    m_format=imagetype.YUV
    worker_running=False
    
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(820, 418)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.comboBox_port = QComboBox(self.centralwidget)
        self.comboBox_port.setObjectName(u"comboBox_port")
        self.comboBox_port.setGeometry(QRect(590, 60, 141, 31))
        self.comboBox_baud = QComboBox(self.centralwidget)
        self.comboBox_baud.setObjectName(u"comboBox_baud")
        self.comboBox_baud.setGeometry(QRect(590, 100, 141, 31))
        self.label_port = QLabel(self.centralwidget)
        self.label_port.setObjectName(u"label_port")
        self.label_port.setGeometry(QRect(500, 60, 81, 31))
        self.label_port.setFrameShape(QFrame.Box)
        self.label_port.setAlignment(Qt.AlignCenter)
        self.label_baud = QLabel(self.centralwidget)
        self.label_baud.setObjectName(u"label_baud")
        self.label_baud.setGeometry(QRect(500, 100, 81, 31))
        self.label_baud.setFrameShape(QFrame.Box)
        self.label_baud.setAlignment(Qt.AlignCenter)
        self.pushButton_get_frame = QPushButton(self.centralwidget)
        self.pushButton_get_frame.setObjectName(u"pushButton_get_frame")
        self.pushButton_get_frame.setGeometry(QRect(500, 270, 111, 41))
        self.pushButton_stop_frame = QPushButton(self.centralwidget)
        self.pushButton_stop_frame.setObjectName(u"pushButton_stop_frame")
        self.pushButton_stop_frame.setGeometry(QRect(620, 320, 111, 41))
        self.pushButton_save_image = QPushButton(self.centralwidget)
        self.pushButton_save_image.setObjectName(u"pushButton_save_image")
        self.pushButton_save_image.setGeometry(QRect(620, 270, 111, 41))
        self.pushButton_save_dir = QPushButton(self.centralwidget)
        self.pushButton_save_dir.setObjectName(u"pushButton_save_dir")
        self.pushButton_save_dir.setGeometry(QRect(500, 20, 81, 31))
        self.textEdit_path = QTextEdit(self.centralwidget)
        self.textEdit_path.setObjectName(u"textEdit_path")
        self.textEdit_path.setGeometry(QRect(590, 20, 221, 31))
        self.pushButton_get_frames = QPushButton(self.centralwidget)
        self.pushButton_get_frames.setObjectName(u"pushButton_get_frames")
        self.pushButton_get_frames.setGeometry(QRect(500, 320, 111, 41))
        self.pushButton_open_port = QPushButton(self.centralwidget)
        self.pushButton_open_port.setObjectName(u"pushButton_open_port")
        self.pushButton_open_port.setGeometry(QRect(740, 60, 71, 31))
        self.label_resolution = QLabel(self.centralwidget)
        self.label_resolution.setObjectName(u"label_resolution")
        self.label_resolution.setGeometry(QRect(500, 180, 81, 31))
        self.label_resolution.setFrameShape(QFrame.Box)
        self.label_resolution.setAlignment(Qt.AlignCenter)
        self.comboBox_resolution = QComboBox(self.centralwidget)
        self.comboBox_resolution.setObjectName(u"comboBox_resolution")
        self.comboBox_resolution.setGeometry(QRect(590, 180, 141, 31))
        self.label_Format = QLabel(self.centralwidget)
        self.label_Format.setObjectName(u"label_Format")
        self.label_Format.setGeometry(QRect(500, 220, 81, 31))
        self.label_Format.setFrameShape(QFrame.Box)
        self.label_Format.setAlignment(Qt.AlignCenter)
        self.comboBox_Format = QComboBox(self.centralwidget)
        self.comboBox_Format.setObjectName(u"comboBox_Format")
        self.comboBox_Format.setGeometry(QRect(590, 220, 141, 31))
        self.label_img = QLabel(self.centralwidget)
        self.label_img.setObjectName(u"label_img")
        self.label_img.setGeometry(QRect(10, 10, 480, 360))
        self.label_img.setFrameShape(QFrame.Panel)
        self.pushButton_close_port = QPushButton(self.centralwidget)
        self.pushButton_close_port.setObjectName(u"pushButton_close_port")
        self.pushButton_close_port.setGeometry(QRect(740, 100, 71, 31))
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 820, 22))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
        self.label_port.setText(QCoreApplication.translate("MainWindow", u"PORT", None))
        self.label_baud.setText(QCoreApplication.translate("MainWindow", u"Baudrate", None))
        self.pushButton_get_frame.setText(QCoreApplication.translate("MainWindow", u"get frame", None))
        self.pushButton_stop_frame.setText(QCoreApplication.translate("MainWindow", u"stop frame", None))
        self.pushButton_save_image.setText(QCoreApplication.translate("MainWindow", u"save image", None))
        self.pushButton_save_dir.setText(QCoreApplication.translate("MainWindow", u"save dir", None))
        self.pushButton_get_frames.setText(QCoreApplication.translate("MainWindow", u"get frames", None))
        self.pushButton_open_port.setText(QCoreApplication.translate("MainWindow", u"open Port", None))
        self.label_resolution.setText(QCoreApplication.translate("MainWindow", u"Resolution", None))
        self.label_Format.setText(QCoreApplication.translate("MainWindow", u"Format", None))
        self.label_img.setText("")
        self.pushButton_close_port.setText(QCoreApplication.translate("MainWindow", u"close Port", None))
    # retranslateUi



    def setup(self):
        initial_dir = "C:/Users/spc/Desktop/"
        self.textEdit_path.setText(initial_dir)
        bauds=["9600","11400","19200","28800","38400","56000","57600","76800","115200","230400"]
        ports=["COM1","COM2","COM3","COM4","COM5","COM6","COM7","COM8","COM9"]
        resolutions=["QQVGA","QVGA","VGA","CIF","QCIF"]
        Formats=["RGB","YUV"]
        self.comboBox_Format.addItems(Formats)
        self.comboBox_resolution.addItems(resolutions)
        self.comboBox_baud.addItems(bauds)
        self.comboBox_port.addItems(ports)
        fmt=self.comboBox_Format.currentText()
        self.m_format=imagetype[fmt]
        res=self.comboBox_resolution.currentText()
        self.m_resolution=resolution[res]
        
        self.pushButton_open_port.clicked.connect(self.open_port)
        self.pushButton_close_port.clicked.connect(self.close_port)
        #self.pushButton_get_frame.clicked.connect(self.getframe)
        self.pushButton_get_frame.clicked.connect(lambda: self.start_task(mode.getframe,self.port_enable,self.m_serial,self.m_width,self.m_height,self.m_format))
        self.pushButton_get_frames.clicked.connect(lambda: self.start_task(mode.getframes,self.port_enable,self.m_serial,self.m_width,self.m_height,self.m_format))
        self.pushButton_stop_frame.clicked.connect(self.stop_frame)
        self.comboBox_Format.currentIndexChanged.connect(self.Format_change)
        self.comboBox_resolution.currentIndexChanged.connect(self.resolution_change)
        
        self.pushButton_save_image.clicked.connect(self.save_image_to_file)
        self.pushButton_save_dir.clicked.connect(self.set_path)
    
    #-------------------------------------------------Meltithread functions-----------------------------------------------------#
    def start_task(self,mode,*args,**kwargs):
        if self.worker_running:
            QMessageBox.warning(self.centralwidget, "Task Running", "A task is already running. Please wait for it to complete.")
            return

        self.worker_running = True
        self.worker = Worker(mode,*args,**kwargs)
        self.worker.worker_object.image_signal.connect(self.load_and_display_image)
        self.worker.worker_object.error.connect(self.handle_error)
        self.worker.finished.connect(self.task_finished)
        self.worker.start()

    def task_finished(self):
        self.worker_running = False
        #QMessageBox.information(self.centralwidget, "Task Finished", "The task has been completed.")
    #------------------------------------------------callback functions---------------------------------------------------------#    
    def save_image_to_file(self):
        path=self.textEdit_path.toPlainText()
        pixmap = self.label_img.pixmap()
        if pixmap:
            # Save QPixmap as PNG
            fnum=count_png_files_in_directory(path)+1
            pixmap.save(path+f"/{fnum}.PNG", 'PNG')
            QMessageBox.information(self.centralwidget, "success", f"Image is successfully saved in {path}/{fnum}.PNG.")
        else:
            QMessageBox.warning(self.centralwidget, "No image", "There is no image.")
            
    def set_path(self):
        initial_dir = "C:/Users/spc/Desktop/"
        options = QFileDialog.Options()
        file_name = QFileDialog.getExistingDirectory(self.centralwidget, "set save path", initial_dir, options=options)
        if file_name:
            self.textEdit_path.setText(file_name)
    
    def open_port(self):
        
        if self.worker_running:
            QMessageBox.warning(self.centralwidget, "Task Running", "A task is already running. Please wait for it to complete.")
            return
        
        #in case of serial port is opened
        if self.port_enable:
            QMessageBox.warning(self.centralwidget, "Port opened", "Port is opened. Disconnect Port first")
            return
            
        port=self.comboBox_port.currentText()
        baud=self.comboBox_baud.currentText()
        
        try:
            self.m_serial = serial.Serial(port, baud)
        except serial.SerialException as e:
            self.port_enable=False
            QMessageBox.warning(self.centralwidget,"warning",f"Could not open serial port {port}:\n{e}")
        else:
            self.port_enable=True
            QMessageBox.warning(self.centralwidget,"success",f"successfully opened serial port {port}")
    
    def close_port(self):
        if self.worker_running:
            QMessageBox.warning(self.centralwidget, "Task Running", "A task is already running. Please wait for it to complete.")
            return
        #in case of serial port is opened
        try:
            if self.m_serial is not None:
                if self.m_serial.is_open or self.port_enable:
                    self.m_serial.close()
                    self.port_enable=False
                    QMessageBox.warning(self.centralwidget, "success", "Port is closed successfully")
                else: 
                    QMessageBox.warning(self.centralwidget, "warning", "There is no opened Port.")
            else:
                QMessageBox.warning(self.centralwidget, "warning", "There is no opened Port.")
        
        except Exception as e:
            self.error.emit(f"An error occurred: {str(e)}")
        
        
    def getframe(self):
        if self.port_enable==False or self.m_serial==None:
            QMessageBox.warning(self.centralwidget,"warning","serial port is not opened")
            return -1
            
        if self.m_serial.is_open==False:
            QMessageBox.warning(self.centralwidget,"warning","serial port is not opened")
            return -1;
        print("start\n")
        buf=""
        try:
            while True:
                if self.m_serial.in_waiting > 0:
                    data= self.m_serial.read(self.m_serial.in_waiting).decode('ascii')
                    if data =="start":
                        while True:
                            if self.m_serial.in_waiting > 0:
                                buf += self.m_serial.read(self.m_serial.in_waiting).decode('ascii')
                                if "finish" in buf:
                                    buf, _ = buf.split("finish", 1)
                                    self.m_data = [int(val, 16) for val in buf.split()]
                                    #print(self.m_data)
                                    data_,len_=raw2Img(self.m_width, self.m_height, self.m_data, self.m_format)
                                    if self.m_format==imagetype.RGB:
                                        rgb_image=RGB565_to_RGB888(data_, self.m_height, self.m_width)
                                    else: 
                                        rgb_image = yuv422_to_rgb(data_, self.m_width, self.m_height)
                                        
                                    self.load_and_display_image(rgb_image)
                                    break
                        break

            #plt.imshow(rgb_image)
            # cv2.imshow("img",rgb_image)
            # cv2.waitKey(0)
            # cv2.destroyAllWindows()
            print("finished\n")        
        except serial.SerialException as e:
            QMessageBox.warning(self.centralwidget,"warning",f"serial error occured:\n{e}")
    
    def load_and_display_image(self, cv_image):
        
        if cv_image is None:
            QMessageBox.warning(self.centralwidget,"warning", "image is empty")
            return

        # BGR to RGB
        rgb_image = cv2.cvtColor(cv_image, cv2.COLOR_BGR2RGB)

        # Convert to QImage
        height, width, channels = rgb_image.shape
        q_image = QImage(rgb_image.data, width, height, width * channels, QImage.Format_RGB888)

        # Conver to Qpixmap
        pixmap = QPixmap.fromImage(q_image)
        scaled_pixmap = pixmap.scaled(self.label_img.size(), Qt.AspectRatioMode.KeepAspectRatio, Qt.TransformationMode.SmoothTransformation)
        self.label_img.setPixmap(scaled_pixmap)
        
    def stop_frame(self):
       global STOP_FRAME 
       STOP_FRAME.set()
    
    def Format_change(self):
        fmt=self.comboBox_Format.currentText()
        self.m_format=imagetype[fmt]

    def resolution_change(self):
        fmt=self.comboBox_resolution.currentText()
        self.m_resolution=resolution[fmt]
        
        if self.m_resolution==resolution.QQVGA:
            self.m_width, self.m_height = qqvga
        elif self.m_resolution==resolution.QVGA:
            self.m_width, self.m_height = qvga
        elif self.m_resolution==resolution.VGA:
            self.m_width, self.m_height = vga
        elif self.m_resolution==resolution.CIF:
            self.m_width, self.m_height = cif
        elif self.m_resolution==resolution.QCIF:
            self.m_width, self.m_height = qcif
        
        
              
#----------------------------------normal functions--------------------------------------------------#            
    def messagebox(self,message):
        QMessageBox.information(self.centralwidget,"message",message)

    def handle_error(self, message):
        QMessageBox.warning(self.centralwidget, "Warning", message)   
        
def count_png_files_in_directory(directory_path):
    try:
        # Find PNG files
        file_pattern = os.path.join(directory_path, '*.png')
        png_files = glob.glob(file_pattern)
        return len(png_files)
    except Exception as e:
        print(f"Error: {e}")
        return 0  
        
if __name__=="__main__":
    app = QApplication.instance()
    if app is None:
        app = QApplication(sys.argv)
    else:
        print("QApplication instance already exists.")
    MainWindow = QMainWindow()

    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)  
    ui.setup()
    
    MainWindow.show()
    sys.exit(app.exec())