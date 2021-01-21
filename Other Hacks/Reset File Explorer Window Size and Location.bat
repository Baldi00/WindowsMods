@echo off

:: Created by: Shawn Brink
:: http://www.sevenforums.com
:: Tutorial:  http://www.sevenforums.com/tutorials/15692-folder-view-settings-reset-all-default.html


:: To reset all folders to default folder views
Reg Delete "HKCU\Software\Microsoft\Windows\Shell\BagMRU" /F
Reg Delete "HKCU\Software\Microsoft\Windows\Shell\Bags" /F

Reg Delete "HKCU\Software\Microsoft\Windows\ShellNoRoam\Bags" /F
Reg Delete "HKCU\Software\Microsoft\Windows\ShellNoRoam\BagMRU" /F

Reg Delete "HKCU\Software\Classes\Local Settings\Software\Microsoft\Windows\Shell\BagMRU" /F
Reg Delete "HKCU\Software\Classes\Local Settings\Software\Microsoft\Windows\Shell\Bags" /F

Reg Delete "HKCU\Software\Classes\Wow6432Node\Local Settings\Software\Microsoft\Windows\Shell\Bags" /F
Reg Delete "HKCU\Software\Classes\Wow6432Node\Local Settings\Software\Microsoft\Windows\Shell\BagMRU" /F

:: To reset Windows Photo Viewer position
Reg Delete "HKCU\Software\Microsoft\Windows Photo Viewer\Viewer" /VA /F

:: To reset navigation pane width to default
Reg Delete "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Modules\GlobalSettings\Sizer" /F

:: To kill and restart explorer
taskkill /f /im explorer.exe
start explorer.exe



