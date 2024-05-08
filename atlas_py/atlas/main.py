from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtWidgets import (QApplication, QMainWindow, QSizePolicy, QStatusBar, QWidget,QSplashScreen)
from PySide6.QtGui import (QPixmap)

def main():
    app = QApplication([])
    splashscreen = QPixmap(":/images/assets/Atlas_logo_v2.svg")
    splash = QSplashScreen(splashscreen.scaled(200,200,Qt.KeepAspectRatio))
    splash.show()
    app.processEvents()

main()

#window = QMainWindow()
#window.show()
#splash.finish(window)
#sys.exit(app.exec())
