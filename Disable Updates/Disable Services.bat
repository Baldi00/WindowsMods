net stop wuauserv
sc config "wuauserv" start= disabled

net stop bits
sc config "bits" start= disabled