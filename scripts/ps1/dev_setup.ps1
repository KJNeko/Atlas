#towerwatchman 7.7.2023
#Windows installer for Atlas Development

Set-ExecutionPolicy "Unrestricted"

#Variables
#REQUIRED
$git_installed = $false
$sqlite3_installed = $false
$cmake_installed = $false
$mingw_installed = $false
$7zip_installed = $false
$qt_installed =  $false

#OPTIONAL
$vscode_installed = $false
$python_installed = $false
$github_desktop = $false

#Paths
$git_path = "C:\Program Files\Git\git-bash.exe"
$sqlit3_path = "C:\Sqlite3"
$cmake_path = "C:\Program Files\CMake\"
$vscode_path = "%LOCALAPPDATA%\Programs\Microsoft VS Code"
$python_path = "C:\Python311\python.exe"
$github_desktop_path = "%LOCALAPPDATA%\GitHubDesktop"
$mingw_path = "C:\MinGW"

# winget install --id Git.Git -e --source winget
# https://www.python.org/ftp/python/3.11.4/python-3.11.4-amd64.exe
# https://www.sqlite.org/2023/sqlite-amalgamation-3420000.zip
# https://www.sqlite.org/2023/sqlite-dll-win64-x64-3420000.zip
# https://www.sqlite.org/2023/sqlite-preprocessed-3420000.zip
# https://github.com/Kitware/CMake/releases/download/v3.26.4/cmake-3.26.4-windows-x86_64.msi
# https://github.com/brechtsanders/winlibs_mingw/releases/download/12.2.0-16.0.0-10.0.0-ucrt-r5/winlibs-x86_64-posix-seh-gcc-12.2.0-llvm-16.0.0-mingw-w64ucrt-10.0.0-r5.7z
# https://download.qt.io/official_releases/qt/6.4/6.4.3/single/qt-everywhere-src-6.4.3.zip

#Add cmake path var to ENV
function Init {
    Write-Output "INFO: STARTING DEPENDENCY CHECK"
    Write-Output "INFO: CHECKING REQUIRED DEPENDENCIES"

    $git_installed = CheckInstallation $git_path "Git"

    $python_installed = CheckInstallation $python_path 'Python'
    
    #InstallProgram 'https://www.python.org/ftp/python/3.11.4/python-3.11.4-amd64.exe' ' InstallAllUsers=1 PrependPath=1 TargetDir="C:\Python311"' 'Python'

}

function CreateDirectories()
{
    
}

function CheckInstallation($File, $Name) {
    if (Test-Path -Path $File -PathType Leaf) {
        Write-Host "INFO:" $Name "ALREADY INSTALLED" -ForegroundColor green
        return $true
    } else {
        Write-Host "ERROR:"  $Name "NOT INSTALLED" -ForegroundColor red
        return $false
    }
}

function InstallProgram($URL, $arguments, $program_name)
{
    $file = ([uri]$URL).Segments[-1]
    Write-Output $file

    $Installer = $env:TEMP + "\" + $file

    #Write-Output $arguments

    #Invoke-WebRequest -URI $URL -OutFile $Installer
    #$Installer = $env:TEMP + "\7z1900-x64.msi"; 
    #Invoke-WebRequest "https://www.7-zip.org/a/7z1900-x64.msi" -OutFile 
    #$Installer7Zip; 
    Write-Output "$program_name Installtion Started"
    Start-Process -FilePath $Installer $arguments -Wait
    Write-Output "$program_name Installation Complete"
    #msiexec /i $Installer /qb; 
    #Remove-Item $Installer;

}

function Unzip($path)
{

}
#Program Entry Point
Init

