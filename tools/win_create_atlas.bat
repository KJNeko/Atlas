@ECHO OFF

ECHO Checking for Python Virtual Environment
if exist venv\ (
	ECHO Starting Virtual Environment
	call "venv\Scripts\activate"
) else (
	ECHO Creating Python Virtual Environment
	mkdir venv
	python -m venv venv
	call "venv\Scripts\activate"
	pip install -U pyinstaller
	REM QT Packages
	pip install pyside6
	REM Needed Packages
	pip install pillow
	pip install pandas
	pip install urllib3
	pip install colorama
)
ECHO Building Atlas. Please wait.....

	pyinstaller  --onedir --windowed --add-data "../atlas/assets/themes;./Resources/themes" --add-data "../atlas/assets/engines/types;./Resources/engines/types" --icon="../atlas/assets/icons/appicon.ico" --name="Atlas" "../app.py" --noconfirm --distpath="dist"

	ECHO Building Atlas Debug Console Please wait.....
	pyinstaller --onefile --icon="../atlas/assets/icons/appicon.ico" --name="Atlas Debug Console" "../atlas/debug_console.py" --noconfirm --distpath="dist/Atlas"
	
PAUSE