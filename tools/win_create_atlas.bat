@ECHO OFF

ECHO Building Atlas. Please wait.....

pyinstaller  --onedir --windowed --add-data "../atlas/assets/themes;./Resources/themes" --icon="../atlas/assets/icons/appicon.ico" --name="Atlas" "../app.py" --noconfirm --distpath="dist"

ECHO Building Atlas Debug Console Please wait.....
pyinstaller --onefile --icon="../atlas/assets/icons/appicon.ico" --name="Atlas Debug Console" "../atlas/debug_console.py" --noconfirm --distpath="dist/Atlas"

PAUSE