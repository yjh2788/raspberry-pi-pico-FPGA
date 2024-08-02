# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'CameraViewer.ui'
##
## Created by: Qt User Interface Compiler version 6.5.3
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QComboBox, QFrame, QLabel,
    QMainWindow, QMenuBar, QPushButton, QSizePolicy,
    QStatusBar, QTextEdit, QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(831, 418)
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
        self.textEdit_path.setGeometry(QRect(590, 20, 191, 31))
        self.pushButton_get_frames = QPushButton(self.centralwidget)
        self.pushButton_get_frames.setObjectName(u"pushButton_get_frames")
        self.pushButton_get_frames.setGeometry(QRect(500, 320, 111, 41))
        self.pushButton_open_port = QPushButton(self.centralwidget)
        self.pushButton_open_port.setObjectName(u"pushButton_open_port")
        self.pushButton_open_port.setGeometry(QRect(740, 60, 71, 71))
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
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 831, 22))
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
    # retranslateUi

