netsh interface ip set address name="Wi-Fi" static 192.168.1.30 255.255.255.0 192.168.1.1 1
netsh interface ip set dns name="Wi-Fi" static 208.67.222.222
netsh interface ip add dns name="Wi-Fi" 8.8.8.8 index=2