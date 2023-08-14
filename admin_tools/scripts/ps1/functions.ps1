
#Paths
$7zip_path = "C:\Program Files\7-Zip\7z.exe"
$git_path = "C:\Program Files\Git\git-bash.exe"
$sqlite_path = "C:\msys64\mingw64\bin\sqlite3.exe"
$cmake_path = "C:\Program Files\CMake\bin\cmake-gui.exe"
$mingw_path = ""
$vscode_path = "$($env:LOCALAPPDATA)\Programs\Microsoft VS Code\Code.exe"
$python_path = "C:\Python311\python.exe"
#C:\Users\engineer\AppData\Local\Programs\Python\Python311
$py_win_path = "$($env:LOCALAPPDATA)\Programs\Python\Python311\python.exe"
$github_desktop_path = "$($env:LOCALAPPDATA)\GitHubDesktop\GitHubDesktop.exe"
$mingw_path = "C:\mingw64\bin\gcc.exe"
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

function CheckRequiredDependencies() {
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
    if (CheckInstallation $mingw_path "cmake" -eq $true) {
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