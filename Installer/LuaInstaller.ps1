$sourcePSfile = (Get-Item -Path ".\").FullName + "\LuaInstaller.ps1"
if (!([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) 
{
	Start-Process powershell.exe "-NoProfile -ExecutionPolicy Bypass -File `"$sourcePSfile`"" -Verb RunAs; exit 
	#Start-Process powershell -verb runas -ArgumentList "-file "+$sourcePSfile
}

#Start-Process powershell -Verb runAs
powershell -Command Set-ExecutionPolicy RemoteSigned
$sourceRootLuaFile = (Get-Item -Path ".\").FullName + "\logic.lua"
$sourceRoot = (Get-Item -Path ".\").FullName + "\Lua"
$destinationRoot = "C:\Program Files\Lua"
"sourceRootLuaFile: " + $sourceRootLuaFile
"sourceRoot       : " + $sourceRoot
"destinationRoot  : " + $destinationRoot
Copy-Item -Path $sourceRoot -Recurse -Destination $destinationRoot -Container
Copy-Item -Path $sourceRootLuaFile -Destination "C:\"
[System.Environment]::SetEnvironmentVariable("LUA_DIR", "C:\Program Files\Lua\5.3", "Machine")
[System.Environment]::SetEnvironmentVariable("LUA_PATH", "?.lua;%LUA_DIR%\lua\?.lua", "Machine")
[System.Environment]::SetEnvironmentVariable("LUA_CPATH", "?.dll;%LUA_DIR%\?.dll", "Machine")
[System.Environment]::SetEnvironmentVariable("Path", $Env:Path + ";C:\Program Files\Lua\5.3", "Machine")
"Succesfull install"
Wait-Event -Timeout 100