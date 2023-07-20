@echo off

GOTO MAIN_MENU

:MAIN_MENU
test&cls
ECHO ------------------------------------------------------------
ECHO -                  ATLAS TOOLKIT V 1.0                     -
ECHO ------------------------------------------------------------
ECHO.
ECHO MAIN MENU
ECHO.
ECHO 1. DEV SETUP [NOT IMPLEMENTED]
ECHO 2. IMAGE DOWNLOADER
ECHO 3. EXIT
ECHO.

set /p ans_1= SELECT FROM THE OPTIONS ABOVE: 

if /i "%ans_1:~,1%" EQU "1" GOTO MAIN_MENU
if /i "%ans_1:~,1%" EQU "2" GOTO IMAGE_MENU
if /i "%ans_1:~,1%" EQU "3" exit /b

:IMAGE_MENU
test&cls
ECHO ------------------------------------------------------------
ECHO -                  ATLAS TOOLKIT V 1.0                     -
ECHO ------------------------------------------------------------
ECHO.
ECHO IMAGE DOWNLOADER MENU
ECHO NOTE (PYTHON MUST BE INSTALLED)
ECHO.
ECHO 1. AUDIT IMAGES [NOT IMPLEMENTED]
ECHO 2. DOWNLOAD BANNER IMAGES [NOT IMPLEMENTED]
ECHO 3. DONWLOAD PREVIEW IMAGES [NOT IMPLEMENTED]
ECHO 4. DOWNLOAD ALL AVAILABLE IMAGES
ECHO 5. BACK TO MAIN MENU
ECHO.

set /p ans_2= SELECT FROM THE OPTIONS ABOVE: 
if /i "%ans_2:~,1%" EQU "1" GOTO IMAGE_MENU
if /i "%ans_2:~,1%" EQU "2" GOTO IMAGE_MENU
if /i "%ans_2:~,1%" EQU "3" GOTO IMAGE_MENU
if /i "%ans_2:~,1%" EQU "4" GOTO RUN_IMAGED_DOWNLOADER
if /i "%ans_2:~,1%" EQU "5" GOTO MAIN_MENU

:RUN_IMAGED_DOWNLOADER
test&cls
ECHO RUNNING SCRIPT 
python.exe "py\Image_Downloader.py"



PAUSE