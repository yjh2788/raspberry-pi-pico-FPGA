import sys
import cv2
import enum

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QComboBox, QFrame, QLabel,QFileDialog,
    QMainWindow, QMenuBar, QPushButton, QSizePolicy,
    QStatusBar, QTextEdit, QWidget)

class mode(enum):
    getframe = 1
    getframes = 2
    stopframe = 3

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(818, 418)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.frame_img = QFrame(self.centralwidget)
        self.frame_img.setObjectName(u"frame_img")
        self.frame_img.setGeometry(QRect(10, 10, 480, 360))
        self.frame_img.setFrameShape(QFrame.Box)
        self.frame_img.setFrameShadow(QFrame.Raised)
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
        self.pushButton_get_frame.setGeometry(QRect(500, 150, 81, 31))
        self.pushButton_stop_frame = QPushButton(self.centralwidget)
        self.pushButton_stop_frame.setObjectName(u"pushButton_stop_frame")
        self.pushButton_stop_frame.setGeometry(QRect(590, 190, 81, 31))
        self.pushButton_save_image = QPushButton(self.centralwidget)
        self.pushButton_save_image.setObjectName(u"pushButton_save_image")
        self.pushButton_save_image.setGeometry(QRect(590, 150, 81, 31))
        self.pushButton_save_dir = QPushButton(self.centralwidget)
        self.pushButton_save_dir.setObjectName(u"pushButton_save_dir")
        self.pushButton_save_dir.setGeometry(QRect(500, 20, 81, 31))
        self.textEdit_path = QTextEdit(self.centralwidget)
        self.textEdit_path.setObjectName(u"textEdit_path")
        self.textEdit_path.setGeometry(QRect(590, 20, 191, 31))
        self.pushButton_get_frames = QPushButton(self.centralwidget)
        self.pushButton_get_frames.setObjectName(u"pushButton_get_frames")
        self.pushButton_get_frames.setGeometry(QRect(500, 190, 81, 31))
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 818, 22))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)

        QMetaObject.connectSlotsByName(MainWindow)
        self.setup()
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
    # retranslateUi


    def setup(self):
        self.pushButton_get_frame.clicked.connect()
        self.pushButton_get_frames.clicked.connect()
        self.pushButton_stop_frame.clicked.connect()
        self.pushButton_save_image.clicked.connect(self.save_image_to_file)
        self.pushButton_save_dir.clicked.connect()
        
    def save_image_to_file(self):
        initial_dir = "/path/to/your/initial/directory"
        options = QFileDialog.Options()
        file_name, _ = QFileDialog.getSaveFileName(self, "Save Image", initial_dir, "PNG Files (*.png);;All Files (*)", options=options)



if __name__=="__main__":
    app = QApplication(sys.argv)
    mainWin = Ui_MainWindow()
    mainWin.show()
    sys.exit(app.exec())