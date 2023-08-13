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