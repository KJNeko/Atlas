# .Net methods for GUI
Add-Type -AssemblyName PresentationCore, PresentationFramework

#All paths are relative to developer_toolkit.bat. NOT the init.ps1 file.
# keep that in mind when loading files
$functionScript = ($pwd).Path + "\scripts\ps1\functions.ps1"
$buttonScript = ($pwd).Path + "\scripts\ps1\buttons.ps1"
Import-Module -Name $functionScript -Verbose

#VARS
$xamlFile = ($pwd).Path + "\ui\MainWindow.xaml"

#Create window
$inputXML = Get-Content $xamlFile -Raw
$inputXML = $inputXML -replace 'mc:Ignorable="d"', '' -replace "x:N", 'N' -replace '^<Win.*', '<Window'
[XML]$XAML = $inputXML

#Read XAML
$reader = (New-Object System.Xml.XmlNodeReader $xaml)
try {
    $window = [Windows.Markup.XamlReader]::Load( $reader )
}
catch {
    Write-Warning $_.Exception
    throw
}

# Create variables based on form control names.
# Variable will be named as 'var_<control name>'

$xaml.SelectNodes("//*[@Name]") | ForEach-Object {
    #"trying item $($_.Name)"
    try {
        Set-Variable -Name "var_$($_.Name)" -Value $window.FindName($_.Name) -ErrorAction Stop
    }
    catch {
        throw
    }
}
Get-Variable var_*

#Set button functions
Import-Module -Name $buttonScript -Verbose

CheckDependencies
#CreateDirectories

$Null = $window.ShowDialog()


