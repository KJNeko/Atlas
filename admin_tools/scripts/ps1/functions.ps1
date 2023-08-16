
#Paths
$7zip_path = "C:\Program Files\7-Zip\7z.exe"
$git_path = "C:\Program Files\Git\git-bash.exe"
$sqlite_path = "C:\msys64\mingw64\bin\sqlite3.exe"
$cmake_path = "C:\Program Files\CMake\bin\cmake-gui.exe"
$mingw_path = "C:\mingw64\bin\gcc.exe"
$msys2_path = "C:\msys64\usr\bin\bash.exe"
$vscode_path = "$($env:LOCALAPPDATA)\Programs\Microsoft VS Code\Code.exe"
$python_path = "C:\Python311\python.exe"
#C:\Users\engineer\AppData\Local\Programs\Python\Python311
$py_win_path = "$($env:LOCALAPPDATA)\Programs\Python\Python311\python.exe"
$github_desktop_path = "$($env:LOCALAPPDATA)\GitHubDesktop\GitHubDesktop.exe"
$qt_path = "C:\Qt\6.4.3\mingw_64\bin\designer.exe"
$winget_path = "$($env:LOCALAPPDATA)\Microsoft\WindowsApps\winget.exe"

function CheckInstallation($File, $Name) {
    if (Test-Path -Path $File -PathType Leaf) {
        if ($Name -ne "no-out") {
            Write-Host "INFO:" $Name "INSTALLED" -ForegroundColor green
        }
        return $true
    }
    else {
        if ($Name -ne "no-out") {
            Write-Host "ERROR:"  $Name "NOT INSTALLED" -ForegroundColor red
        }
        #Write-Host $File
        return $false
    }
}

function CheckDependencies() {
    if (CheckInstallation $winget_path "winget" -eq $true) {
        $var_lbl_winget.Content = "INSTALLED"
        $var_lbl_winget.Foreground = "green"
        $var_lbl_winget_location.Content = $winget_path
        $var_btn_winget.IsEnabled = $false

    }
    else {

        $winget_msgbox = "Winget is required to Install missing pacakges. Would you like to install it now?"
        $result = [System.Windows.MessageBox]::Show($winget_msgbox, "Install Winget", 3, 32)
        if ($result -eq "Yes") {
            InstallWinget
        }
        if ($result -eq "No") {
            exit
        }
    }
    if (CheckInstallation $7zip_path "7zip" -eq $true) {
        $var_lbl_7zip.Content = "INSTALLED"
        $var_lbl_7zip.Foreground = "green"
        $var_lbl_7zip_location.Content = $7zip_path
        $var_btn_7zip.IsEnabled = $false
    }
    if (CheckInstallation $git_path "git" -eq $true) {
        $var_lbl_git.Content = "INSTALLED"
        $var_lbl_git.Foreground = "green"
        $var_lbl_git_location.Content = $git_path
        $var_btn_git.IsEnabled = $false
    }
    if (CheckInstallation $sqlite_path "sqlite" -eq $true) {
        $var_lbl_sqlite.Content = "INSTALLED"
        $var_lbl_sqlite.Foreground = "green"
        $var_lbl_sqlite_location.Content = $sqlite_path
        $var_btn_sqlite.IsEnabled = $false
    }
    if (CheckInstallation $cmake_path "cmake" -eq $true) {
        $var_lbl_cmake.Content = "INSTALLED"
        $var_lbl_cmake.Foreground = "green"
        $var_lbl_cmake_location.Content = $cmake_path
        $var_btn_cmake.IsEnabled = $false
    }
    if (CheckInstallation $mingw_path "mingw" -eq $true) {
        $var_lbl_mingw.Content = "INSTALLED"
        $var_lbl_mingw.Foreground = "green"
        $var_lbl_mingw_location.Content = $mingw_path
        $var_btn_mingw.IsEnabled = $false
    }

}
function InstallWinget() {
    $URL = "https://aka.ms/getwinget"
    $file = ([uri]$URL).Segments[-1]
    $Installer = $env:TEMP + "\" + $file + ".msixbundle"    
    Invoke-WebRequest -URI $URL -OutFile $Installer -UseBasicParsing
    &$Installer
}

function InstallQt() {
    $URL = "https://d13lb3tujbc8s0.cloudfront.net/onlineinstallers/qt-unified-windows-x64-4.6.0-online.exe"
    $file = ([uri]$URL).Segments[-1]

    #Download File
    $Installer = $env:TEMP + "\" + $file

    $fsize = [math]::Round(((Invoke-WebRequest -URI $URL -Method Head -UseBasicParsing).Headers.'Content-Length') / (1024 * 1024), 2)
    Write-Output "DOWNLOADING QT 6.4.3 SIZE: $fsize mb`nPLEASE WAIT......"
    Invoke-WebRequest -URI $URL -OutFile $Installer -UseBasicParsing

    Start-Process -FilePath $Installer -Wait
    #Set ENV
}

function InstallMinGW {
    $7zip_path = "C:\Program Files\7-Zip\7z.exe"
    $URL = "https://github.com/brechtsanders/winlibs_mingw/releases/download/13.2.0-16.0.6-11.0.0-ucrt-r1/winlibs-x86_64-posix-seh-gcc-13.2.0-llvm-16.0.6-mingw-w64ucrt-11.0.0-r1.7z"
    $file = ([uri]$URL).Segments[-1]
    $Out = "C:\"

    #Download File
    $Installer = $env:TEMP + "\" + $file

    $fsize = [math]::Round(((Invoke-WebRequest -URI $URL -Method Head -UseBasicParsing).Headers.'Content-Length') / (1024 * 1024), 2)
    Write-Output "DOWNLOADING MinGW SIZE: $fsize mb`nPLEASE WAIT......"
    Invoke-WebRequest -URI $URL -OutFile $Installer -UseBasicParsing
    #Extract
    Start-Process $7zip_path -args "x -y $Installer -o$Out"
}

function InstallSqlite3() {
    $7zip_path = "C:\Program Files\7-Zip\7z.exe"
    $sqlite3_include_path = "C:\Sqlite3\include"

    $sqlite3_lib_path = "C:\Sqlite3\lib"
    
    $sqlite3_dll = "https://www.sqlite.org/2023/sqlite-dll-win64-x64-3420000.zip"
    $sqlite3_amalgamation = "https://www.sqlite.org/2023/sqlite-amalgamation-3420000.zip"

    $dll = ([uri]$sqlite3_dll).Segments[-1]    
    #Write-Output $file
    $file = $env:TEMP + "\" + $dll
    $fsize = [math]::Round(((Invoke-WebRequest -URI $Sqlite3_dll -Method Head -UseBasicParsing).Headers.'Content-Length') / (1024 * 1024), 2)
    Write-Output "DOWNLOADING Sqlite3 x64 dll SIZE: $fsize mb`nPLEASE WAIT......"
    Invoke-WebRequest -URI $Sqlite3_dll -OutFile $file -UseBasicParsing
    &$7zip_path x -y $file "-o$sqlite3_lib_path"

    $cpp = ([uri]$sqlite3_amalgamation).Segments[-1]    
    #Write-Output $file
    $file = $env:TEMP + "\" + $cpp
    $fsize = [math]::Round(((Invoke-WebRequest -URI $Sqlite3_amalgamation -Method Head -UseBasicParsing).Headers.'Content-Length') / (1024 * 1024), 2)
    Write-Output "DOWNLOADING Sqlite3 Amalgamation SIZE: $fsize mb`nPLEASE WAIT......"
    Invoke-WebRequest -URI $Sqlite3_amalgamation -OutFile $file -UseBasicParsing

    &$7zip_path e -y $file "-o$sqlite3_include_path"
}

function CreateDirectories() {
    New-Item -ItemType Directory -Force -Path "C:\sqlite3" | Out-Null
    New-Item -ItemType Directory -Force -Path "C:\sqlite3\include" | Out-Null
    New-Item -ItemType Directory -Force -Path "C:\sqlite3\lib" | Out-Null
}