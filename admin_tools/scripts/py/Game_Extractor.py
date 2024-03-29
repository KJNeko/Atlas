/*
color1 - rgb(0, 0, 0): Background Color. This will be the darkest
color2 - rgb(25, 25, 28): Default color for all Widgets
color3 - rgb(30, 31, 34):
color4 - rgb(35, 36, 40): 
color5 - rgb(43, 45, 49): Tree Widget, Button Main
color6 - rgb(49, 51, 56): ListView
color7 - rgb(56, 58, 64): Selected item color
color8 - rgb(64, 66, 73): Button Selected
color9 - rgb(81, 83, 90): Button Border
colorA - rgb(97, 99, 105):
colorB - TBD
colorC - rgb(210,210,210): Default text color
colorD - rgb(255, 255, 255): Selected text color
*/

/* BASE WIDGETS */
QWidget {
    background-color: rgb(25, 25, 28);
    border: none;
}

QFrame {
    background-color: rgb(25, 25, 28);
    border: none;
}

/*QTreeView*/
QTreeView {
    border-top-right-radius: 10px;
    /*border-top-left-radius: 5px;*/
    background-color: rgb(35, 36, 40);
    color: rgb(210, 210, 210);
    selection-background-color: rgb(64, 66, 73);
    /* Used on Mac */
    selection-color: white;
    /* Used on Mac */
    show-decoration-selected: 1;
}

QHeaderView::section {
    background-color: rgb(35, 36, 40);
    color: rgb(210, 210, 210);
    border: 1px solid black;
    border-bottom: none;
    border-right: none;
    border-left: none;
}

QTreeView::item {
    border-top-color: transparent;
    border-bottom-color: transparent;
}

QTreeView::item:hover {
    /*background-color: rgb(56, 58, 64);*/
    background-color: rgb(64, 66, 73);
    /* Used on Windows */
    color: rgb(64, 66, 73)
}

QTreeView::item:selected {
    /*background-color: rgb(56, 58, 64);*/
    background-color: rgb(64, 66, 73);
    /* Used on Windows */
    color: rgb(64, 66, 73)
}

/*QLabel*/
QLabel {
    border: none;
    background-color: none;
    color: rgb(210, 210, 210);
}

/*QTextEdit*/
QTextEdit {
    background-color: transparent;
    color: rgb(210, 210, 210);
}

/*QToolButton*/
QToolButton {
    border: none;
    color: rgb(210, 210, 210);
}

/*QStackedWidget*/
QStackedWidget {
    border: none;
    /*Specific case for main window*/
}

QStackedWidget>QWidget>QFrame {
    background-color: rgb(49, 51, 56);
    border: none;
}



/* Line Edit*/
QLineEdit {
    background-color: rgb(81, 83, 90);
    border: none;
    border-radius: 5px;
    margin-right: 5px;
    color: #3097D1;
}

#settingsPaths>QLineEdit {
    background-color: rgb(81, 83, 90);
    border: none;
    border-radius: 5px;
    margin-right: 5px;
    color: #3097D1;
}

#ThreadingWidget>QWidget {
    background-color: none;
}

/*GroupBox*/
QGroupBox {
    background-color: none;
    border: 2px solid gray;
    border-color: rgb(255, 255, 255);
    border-radius: 10px;
    margin-top: 1ex;
}

QGroupBox>QLabel {
    background-color: none;
}

QGroupBox::title {
    subcontrol-origin: margin;
    color: rgb(255, 255, 255);
    left: 7px;
    padding: 0px 5px 0px 5px;
}

/* Buttons */
QPushButton {
    background-color: rgb(35, 36, 40);
    color: rgb(210, 210, 210);
    border: 1px solid;
    border-color: rgb(81, 83, 90);
    width: 75px;
    height: 30px;
}

QPushButton:disabled {
    background-color: rgb(43, 45, 49, 50);
    color: rgb(210, 210, 210, 50);
    border: 1px solid rgb(210, 210, 210, 50);
}

QPushButton:hover {
    background-color: rgb(64, 66, 73);
}

QPushButton:hover:pressed {
    background-color: rgb(64, 66, 73);
}

/*QSpingBox*/
QSpinBox {
    background-color: rgb(81, 83, 90);
    color: rgb(210, 210, 210);
}

/*QComboBox*/
QComboBox {
    background-color: rgb(81, 83, 90);
    color: rgb(210, 210, 210);
}

QComboBox QAbstractItemView {
    background-color: rgb(81, 83, 90);
    color: rgb(210, 210, 210);
}

QComboBox::item {
    background-color: rgb(81, 83, 90);
}

QComboBox::item:selected {
    background-color: rgb(81, 83, 90);
}

QComboBox:disabled {
    color: rgb(210, 210, 210, 50);
}

/* ToolButtons */
QToolButton {
    background-color: transparent;
    color: rgb(210, 210, 210);
}

/* Checkbox */
QCheckBox {
    background-color: transparent;
    color: rgb(210, 210, 210);
    border: none;
    border-radius: 5px;

}

QCheckBox:disabled {
    color: rgb(210, 210, 210, 50);
}

/* MENU BAR*/
QMenuBar {
    background-color: rgb(25, 25, 28);
    color: rgb(210, 210, 210);
    border-color: rgb(0, 0, 0);
    border-width: 1px;
    border-style: outset;
    border-top: none;
    border-right: none;
    border-left: none;
}

QMenuBar::item:selected {
    background-color: rgb(35, 36, 40);
    color: rgb(255, 255, 255);
}

QMenu {
    background-color: rgb(35, 36, 40);
    color: rgb(255, 255, 255);
}

QMenu::item:selected {
    background-color: rgb(56, 58, 64);
    color: rgb(255, 255, 255);
}

/* STATUS BAR*/

QStatusBar {
    background-color: rgb(25, 25, 28);
    color: rgb(210, 210, 210);
    border: none;
}

QSizeGrip {
    background-color: transparent;
    border: none;
    image: url(:/images/assets/sizegrip.svg);
    width: 24;
    height: 24;
}

/*UI SPECIFIC FOR BORDERS*/
QStackedWidget {
    border-color: rgb(0, 0, 0);
    border-width: 2px;
    border-style: outset;
    border-right: none;
    border-top: none;
    border-bottom: none;
}

QTreeView {
    border-color: rgb(0, 0, 0);
    border-width: 2px;
    border-style: outset;
    border-right: none;
    border-top: none;
    border-left: none;
}

/*UI SPECIFIC FOR COLORS*/
#HomeFrame {
    background-color: rgb(43, 45, 49);
    border: none;
}

#NavFrame {
    background-color: rgb(43, 45, 49);
}

/*This is so the wide banner will show up in game view.*/
#bannerFrame {
    background-color: transparent;
}

#GameWidget {
    background-color: rgb(255, 255, 255);
    border-bottom: 2px solid rgb(0, 0, 0);
}

#SearchIcon {
    background-color: rgb(56, 58, 64);
    border: none;
    margin-left: 4px;
    border-top-left-radius: 5px;
    border-bottom-left-radius: 5px;
}

#SearchBox {
    background-color: rgb(56, 58, 64);
    border: none;
    border-top-right-radius: 5px;
    border-bottom-right-radius: 5px;
    border-bottom-left-radius: 0px;
    border-top-left-radius: 0px;
    margin-right: 0px;
    color: rgb(210, 210, 210);
}

#AddGame {
    background-color: transparent;
    color: rgb(210, 210, 210);
}

#AddGame::hover {
    background-color: transparent;
    color: rgb(255, 255, 255);
}

#SettingsMenu {
    background-color: rgb(35, 36, 40);
}

#SettingsMenu>QWidget {
    background-color: rgb(35, 36, 40);
}

#SettingsMenu>QWidget>QPushButton {
    border: none;
}

#NotificationManagerUI {
    border: 1px solid rgb(0, 0, 0);
}

/*GAME DETAIL VEIW*/
#bannerDetailFrame {
    background-color: rgb(49, 51, 56);
}

#InfoPanel {
    background-color: rgb(35, 36, 40);
}

#previewList {
    background-color: rgb(35, 36, 40);
}

#recordView {
    background-color: rgb(49, 51, 56);
}

#Description {
    background: none;
}

#Info {
    background: none;
}

#Links {
    background: none;
}

#btnHideShow {
    height: 30;
    width: 30;
}

#Header {
    background-color: rgb(49, 51, 56);
    border-left: 2px solid rgb(0, 0, 0);
}

#lblSettingsHeader {
    border-bottom: 1px solid rgba(120, 120, 120, 100);
    border-style: outset;
    padding-bottom: 5px;
}

#StatusFrame {
    border-top: 2px solid rgb(0, 0, 0);
}

#gamesTree {
    border: none;
}

#gameText {
    background-color: rgb(81, 83, 90);
}

/*END CUSTOM COLORS*/
QScrollBar:vertical {
    border-style: none;
    background-color: rgb(30, 31, 34);
    width: 15px;
}

QScrollBar::sub-page:vertical {
    background: rgb(30, 31, 34);
}

QScrollBar::add-page:vertical {
    background: rgb(30, 31, 34);
}

QScrollBar::handle:vertical {
    background-color: rgb(81, 83, 90);
    min-height: 100px;
    width: 5px;
    max-width: 5px;
    border-radius: 7px;
    border: 7px solid;
    border-color: transparent;
}

QScrollBar::add-line:vertical {
    width: 0px;
    height: 0px;
}

QScrollBar::sub-line:vertical {
    width: 0px;
    height: 0px;
}

QStackedWidget>QWidget {
    background-color: rgb(49, 51, 56);
}

/*Dialog Specific DO NOT CHANGE THESE SETTINGS*/
QStackedWidget>QWidget>QFrame>QFrame {
    background-color: none;
    border: none;
}

QStackedWidget>QWidget>QWidget {
    background-color: none;
}

QFrame>QLabel {
    background-color: none;
    border: none;
}

#settingsButtons>QPushButton {
    background-color: none;
}

#settingsButtons>QPushButton:focus {
    background-color: rgb(64, 66, 73);
}

#settingsButtons>QPushButton:hover {
    background-color: rgb(64, 66, 73);
}

QChartView {
    background-color: rgb(64, 66, 73);
}

QTabBar::tab {
    background-color: rgb(81, 83, 90);
    color: rgb(210, 210, 210);
}

QTabBar::tab:selected {
    background-color: rgb(49, 51, 56);
}

QTabWidget {
    background-color: rgb(81, 83, 90);
}

QTabWidget>QWidget {
    background-color: rgb(81, 83, 90);
    border: none;
}

QListWidget {
    background-color: rgb(81, 83, 90);
    color: rgb(210, 210, 210);
}