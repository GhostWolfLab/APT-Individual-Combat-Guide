import sys
from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5.QtWebEngineWidgets import QWebEngineView
from PyQt5.QtCore import QUrl

class WebViewApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("WebView Login")
        self.setGeometry(100, 100, 800, 600)

        self.browser = QWebEngineView()
        self.browser.setUrl(QUrl("http://localhost:5000/login"))  #加载登录页面
        self.setCentralWidget(self.browser)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = WebViewApp()
    window.show()
    sys.exit(app.exec_())
