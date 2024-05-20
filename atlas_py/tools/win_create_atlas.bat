@ECHO OFF

ECHO Building Atlas. Please wait.....

pyinstaller --windowed --add-data "../atlas/assets/themes;./Resources/themes" --icon="../atlas/assets/icons/appicon.ico" --name="Atlas" "../atlas/main.py" --noconfirm --distpath="dist"

ECHO Building Atlas Debug Console Please wait.....
pyinstaller --onefile --icon="../atlas/assets/icons/appicon.ico" --name="Atlas Debug Console" "../atlas/debug_console.py" --noconfirm --distpath="dist/Atlas"

ECHO Creating Additional Folders
REM mkdir "./dist/Hive Collector/_scripts"

REM SET "wkdir=%~dp0"

REM copy "%wkdir%install_files\serviceInstall.bat" "%wkdir%dist\Hive Collector\serviceInstall.bat"
REM copy "%wkdir%install_files\serviceUninstall.bat" "%wkdir%dist\Hive Collector\serviceUninstall.bat"
REM copy "%wkdir%install_files\nssm.exe" "%wkdir%dist\Hive Collector\nssm.exe"
REM copy "%wkdir%install_files\settings.ini" "%wkdir%dist\Hive Collector\settings.ini"


PAUSE