$var_btn_msys2.Add_Click( {
        start-process powershell "winget install -e --id MSYS2.MSYS2" -Wait
        CheckDependencies
    })

$var_btn_mingw.Add_Click( {
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-ninja mingw-w64-x86_64-cmake base-devel mingw-w64-x86_64-libwinpthread-git" -Wait
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
        
        #Start-Process powershell -args '-noprofile', '-noexit', '-EncodedCommand',
        #([Convert]::ToBase64String(
        #    [Text.Encoding]::Unicode.GetBytes(
        #    (Get-Command -Type Function InstallQt).Definition
        #    )
        #)) -Wait

        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-base-6.4.3-1-any.pkg.tar.zst" -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-3d-6.4.3-1-any.pkg.tar.zst"  -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-charts-6.4.3-1-any.pkg.tar.zst"  -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-imageformats-6.4.3-1-any.pkg.tar.zst"  -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-multimedia-6.4.3-1-any.pkg.tar.zst"  -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-svg-6.4.3-1-any.pkg.tar.zst"  -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-shadertools-6.4.3-1-any.pkg.tar.zst"  -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-webview-6.4.3-1-any.pkg.tar.zst" -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-webchannel-6.4.3-1-any.pkg.tar.zst" -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-base-debug-6.4.3-1-any.pkg.tar.zst" -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-3d-debug-6.4.3-1-any.pkg.tar.zst" -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-charts-debug-6.4.3-1-any.pkg.tar.zst" -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-imageformats-debug-6.4.3-1-any.pkg.tar.zst" -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-multimedia-debug-6.4.3-1-any.pkg.tar.zst" -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-svg-debug-6.4.3-1-any.pkg.tar.zst" -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-shadertools-debug-6.4.3-1-any.pkg.tar.zst" -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-webview-debug-6.4.3-1-any.pkg.tar.zst" -NoNewWindow -Wait
        start-process "C:\msys64\msys2.exe" "pacman --noconfirm -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt6-webchannel-debug-6.4.3-1-any.pkg.tar.zst" -NoNewWindow -Wait
        
        #mingw-w64-x86_64-qt6-declarative-
        #mingw-w64-x86_64-qt6-declarative-6.5.1-1
        #     mingw-w64-x86_64-qt6-declarative-debug-6.5.1-1
        #     mingw-w64-x86_64-qt6-webchannel-6.5.1-1  

        CheckDependencies
    })

    