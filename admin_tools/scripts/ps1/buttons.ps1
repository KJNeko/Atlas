
$var_btn_mingw.Add_Click( {
        Start-Process powershell -args '-noprofile', '-noexit', '-EncodedCommand',
        ([Convert]::ToBase64String(
            [Text.Encoding]::Unicode.GetBytes(
            (Get-Command -Type Function InstallMinGW).Definition
            )
        )) -Wait
        
        #start-process "C:\msys64\msys2.exe" "pacman --noconfirm -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-ninja mingw-w64-x86_64-cmake base-devel mingw-w64-x86_64-libwinpthread-git" -Wait
        CheckDependencies
    })

$var_btn_git.Add_Click( {
        start-process powershell "winget install -e --id Git.Git" -Wait
        CheckDependencies
    })

$var_btn_cmake.Add_Click( {
        start-process powershell "winget install -e --id Kitware.CMake" -Wait
        CheckDependencies
    })

$var_btn_7zip.Add_Click( {
        start-process powershell "winget install -e --id 7zip.7zip" -Wait
        CheckDependencies
    })

$var_btn_qt.Add_Click( {
        
        Start-Process powershell -args '-noprofile', '-noexit', '-EncodedCommand',
        ([Convert]::ToBase64String(
            [Text.Encoding]::Unicode.GetBytes(
            (Get-Command -Type Function InstallQt).Definition
            )
        )) -Wait

        CheckDependencies
    })
$var_btn_sqlite.Add_Click( {
        
        Start-Process powershell -args '-noprofile', '-noexit', '-EncodedCommand',
        ([Convert]::ToBase64String(
            [Text.Encoding]::Unicode.GetBytes(
            (Get-Command -Type Function InstallSqlite3).Definition
            )
        )) -Wait

        CheckDependencies
    })
    
    