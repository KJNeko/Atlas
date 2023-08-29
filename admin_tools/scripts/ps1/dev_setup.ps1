﻿#towerwatchman 7.7.2023
#Windows installer for Atlas Development

#Set-ExecutionPolicy "Unrestricted"
#Set-ExecutionPolicy -Scope CurrentUser "Unrestricted"

#Install-Module -Name Microsoft.WinGet.Client -Scope CurrentUser

#For webrequest
$ProgressPreference = 'SilentlyContinue'

#Variables
#REQUIRED
$global:RequirementsMet = $false
$global:7zip_installed = $false
$global:git_installed = $false
$global:sqlite3_installed = $false
$global:cmake_installed = $false
$global:mingw_installed = $false
$global:qt_installed = $false
$global:winget_installed = $false

#OPTIONAL
$global:vscode_installed = $false
$global:python_installed = $false
$global:github_desktop = $false

#Paths
$7zip_path = "C:\Program Files\7-Zip\7z.exe"
$git_path = "C:\Program Files\Git\git-bash.exe"
$sqlite3_include_path = "C:\Sqlite3\include"
$sqlite3_lib_path = "C:\Sqlite3\lib"
$cmake_path = "C:\Program Files\CMake\bin\cmake-gui.exe"
$vscode_path = "$($env:LOCALAPPDATA)\Programs\Microsoft VS Code\Code.exe"
$python_path = "C:\Python311\python.exe"
#C:\Users\engineer\AppData\Local\Programs\Python\Python311
$py_win_path = "$($env:LOCALAPPDATA)\Programs\Python\Python311\python.exe"
$github_desktop_path = "$($env:LOCALAPPDATA)\GitHubDesktop\GitHubDesktop.exe"
$mingw_path = "C:\mingw64\bin\gcc.exe"
$qt_path = "C:\Qt\6.4.3\mingw_64\bin\designer.exe"
$winget_path = "$($env:LOCALAPPDATA)\Microsoft\WindowsApps\winget.exe"

#Links
$sqlite3_dll = "https://www.sqlite.org/2023/sqlite-dll-win64-x64-3420000.zip"
$sqlite3_amalgamation = "https://www.sqlite.org/2023/sqlite-amalgamation-3420000.zip"
$mingw_uri = "https://github.com/brechtsanders/winlibs_mingw/releases/download/12.2.0-16.0.0-10.0.0-ucrt-r5/winlibs-x86_64-posix-seh-gcc-12.2.0-llvm-16.0.0-mingw-w64ucrt-10.0.0-r5.7z"
$qt_uri = "https://d13lb3tujbc8s0.cloudfront.net/onlineinstallers/qt-unified-windows-x64-4.6.0-online.exe"
$winget_uri = "https://aka.ms/getwinget"

#Add cmake path var to ENV
function Init {

    Write-Output "THIS SCRIPT REQUIRES WINDOWS PACKAGE MANAGER 1.5 AND AT LEAST`nWINDOWS 10 VERSION (1809+) IN ORDER TO RUN PROPERLY`n"
    Write-Output "YOUR SYSTEM WILL BE CHECKED FOR ALL REQUIRED DEPENDENCIES"
    Write-Output "PRESS ANY KEY TO CONTINUE" 
    
    $key = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

    Write-Output "`nINFO: STARTING DEPENDENCY CHECK"
    Write-Output "INFO: CHECKING REQUIRED DEPENDENCIES"

    #Required
    $global:winget_installed = CheckInstallation $winget_path "Winget"
    $global:7zip_installed = CheckInstallation $7zip_path "7zip"
    $global:git_installed = CheckInstallation $git_path "Git"
    $global:sqlite3_installed = (CheckInstallation $sqlite3_include_path"\sqlite3.h" "Sqlite3") -and (CheckInstallation $sqlite3_lib_path"\sqlite3.dll" "Sqlite3" )
    $global:cmake_installed = CheckInstallation $cmake_path "CMake"
    $global:mingw_installed = CheckInstallation $mingw_path "MinGW"
    $global:qt_installed = CheckInstallation $qt_path "QT 6.4.3"

    Write-Output "INFO: CHECKING OPTIONAL DEPENDENCIES"
    #optional
    $global:python_installed = (CheckInstallation $py_win_path 'no-out') -or (CheckEnv 'no-out') -or (CheckInstallation $python_path 'no-out')
    if ($Name -ne "no-out") {
        Write-Host "INFO: Python ALREADY INSTALLED" -ForegroundColor green
    }
    else {
        Write-Host "ERROR: Python NOT INSTALLED" -ForegroundColor red
    }
    $global:vscode_installed = CheckInstallation $vscode_path 'Visual Studio Code'
    $global:github_desktop = CheckInstallation $github_desktop_path 'Github Desktop'
    
    $global:RequirementsMet = $global:7zip_installed -and $global:git_installed -and $global:sqlite3_installed -and $global:cmake_installed -and $global:mingw_installed -and $global:qt_installed -and $global:winget_installed
    $global:OptionsMet = $global:python_installed -and $global:vscode_installed -and $global:github_desktop
}

function RequiredInstall() {
    if (!$global:RequirementsMet) {
        Write-Output "WOULD YOU LIKE TO DOWNLOAD AND INSTALL ALL MISSING REQUIRED `nDEPENDENCIES FOR ATLAS`n"
        Write-Output "[Y] YES [N] NO" 
        
        while ($true) {
            $x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
            if ($x.Character -eq "Y" -or $x.Character -eq "y" -or $x.Character -eq "N" -or $x.Character -eq "n") {
                break
            }
        }
    
        #Check for yes
        if ($x.Character -eq "Y" -or $x.Character -eq "y") {
            #download missing packages
            
                        
            if ($global:winget_installed -eq $false) {
                Write-Output "Installing Winget`nPress ENTER after installation is complete"
                InstallProgram $winget_uri "Winget"
                cmd /c 'pause'
            }
            
            if ($global:7zip_installed -eq $false) {
                winget install -e --id 7zip.7zip
            }
            if ($global:git_installed -eq $false) {
                winget install --id Git.Git -e --source winget
            }
            if ($global:sqlite3_installed -eq $false) {
                InstallSqlite3
            }
            if ($global:cmake_installed -eq $false) {
                winget install -e --id Kitware.CMake
            }
            if ($global:mingw_installed -eq $false) {
                InstallMinGW
            }
            if ($global:qt_installed -eq $false) {
                InstallQt
            }

        }
    }
}

function OptionalInstall() {
    if (!$global:OptionsMet) {
        Write-Output "WOULD YOU LIKE TO DOWNLOAD AND INSTALL ALL MISSING OPTIONAL `nDEPENDENCIES FOR ATLAS`n"
        Write-Output "[Y] YES [N] NO" 
        
        while ($true) {
            $x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
            if ($x.Character -eq "Y" -or $x.Character -eq "y" -or $x.Character -eq "N" -or $x.Character -eq "n") {
                break
            }
        }

        if ($global:python_installed -eq $false) {
            winget install -e --id Python.Python.3.11
        }
        if ($global:github_desktop -eq $false) {
            winget install -e --id GitHub.GitHubDesktop
        }
        if ($global:vscode_installed -eq $false) {
            winget install -e --id Microsoft.VisualStudioCode
        }
    }
}
function ShowHeader() {
    #Clear-Host
    Write-Output "*******************************************************************"
    Write-Output "**                                                               **"
    Write-Output "**             ATLAS DEV SETUP | WINDOWS x86 | MinGW             **"
    Write-Output "**                                                               **"
    Write-Output "*******************************************************************"
}
function CreateDirectories() {
    New-Item -ItemType Directory -Force -Path "C:\sqlite3" | Out-Null
    New-Item -ItemType Directory -Force -Path "C:\sqlite3\include" | Out-Null
    New-Item -ItemType Directory -Force -Path "C:\sqlite3\lib" | Out-Null
}

function CheckInstallation($File, $Name) {
    if (Test-Path -Path $File -PathType Leaf) {
        if ($Name -ne "no-out") {
            Write-Host "INFO:" $Name "ALREADY INSTALLED" -ForegroundColor green
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

function InstallProgram($URL, $program_name) {
    $file = ([uri]$URL).Segments[-1]

    $Installer = $env:TEMP + "\" + $file + ".msixbundle"    
    Invoke-WebRequest -URI $URL -OutFile $Installer -UseBasicParsing

    &$Installer
}
function InstallProgramWithArgs($URL, $arguments, $program_name) {
    $file = ([uri]$URL).Segments[-1]
    Write-Output $file

    $Installer = $env:TEMP + "\" + $file

    Invoke-WebRequest -URI $URL -OutFile $Installer -UseBasicParsing
    Write-Output "$program_name Installtion Started"
    Start-Process -FilePath $Installer $arguments -Wait
    Write-Output "$program_name Installation Complete"
}
function InstallSqlite3() {
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
function InstallQt() {
    $URL = $qt_uri
    $file = ([uri]$URL).Segments[-1]

    #Download File
    $Installer = $env:TEMP + "\" + $file

    $fsize = [math]::Round(((Invoke-WebRequest -URI $URL -Method Head -UseBasicParsing).Headers.'Content-Length') / (1024 * 1024), 2)
    Write-Output "DOWNLOADING QT 6.4.3 SIZE: $fsize mb`nPLEASE WAIT......"
    Invoke-WebRequest -URI $URL -OutFile $Installer -UseBasicParsing

    Write-Output "
    Use the following install settings below:
    Qt
    ∟ Qt 6.4.3
      ∟ MinGW 11.x.x
      ∟ Additional Libraries
        ∟ Qt Image Formats
        ∟ Qt Multimedia
        ∟ Qt PDF
        ∟ Qt Virtual Keyboard
        ∟ Qt WebEngine
        ∟ Qt WebSockets
        ∟ Qt WebView	
   "

    Start-Process -FilePath $Installer -Wait


    #Set ENV
}
function InstallMinGW {
    $file = ([uri]$mingw_uri).Segments[-1]
    $Out = "C:\"

    #Download File
    $Installer = $env:TEMP + "\" + $file

    $fsize = [math]::Round(((Invoke-WebRequest -URI $MinGW_uri -Method Head -UseBasicParsing).Headers.'Content-Length') / (1024 * 1024), 2)
    Write-Output "DOWNLOADING MinGW SIZE: $fsize mb`nPLEASE WAIT......"
    Invoke-WebRequest -URI $MinGW_uri -OutFile $Installer -UseBasicParsing
    #Extract
    &$7zip_path x -y $Installer "-o$Out"
}
function CheckEnv($Name) {
    #$env:Path += ';C:\foo'   
    $tmp = $env:Path -split ';'
    Foreach ($path in $tmp) {
        #Write-Output $path        
        if ($path.Contains($Name)) {
            return $true
            break
        }
        else {
            return $false
        }
    }

}

function Unzip($path) {

}

#Program Entry Point
ShowHeader
CreateDirectories
Init
RequiredInstall
OptionalInstall
cmd /c 'pause'