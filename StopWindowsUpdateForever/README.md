# How to install
-Create a folder in %programfiles% (C:\ProgramFiles) and rename it "StopWindowsUpdateForever"<br>
-Copy "StopWindowsUpdateForever.exe" and "SWUFOpenInBackGround.vbs" inside the folder you previously created<br>
-Open pianification utility and import "StopWindowsUpdateForeverPianification.xml"<br>

# What does it do?
This program runs in background with administrator privileges and keeps stopping windows update services<br>

# How does it do this?
-Stop Windows Update service<br>
-Stop "bits" service (support service for Windows Update)<br>
-Delete "SoftwareDistribution" folder (which contains updates) to prevent update installation even if the service re-enable itself<br>
-Remove the icon that says you have updates (not necessary, just a graphic thing)<br>
-Repeat this every 5 seconds<br>
