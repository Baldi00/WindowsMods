taskkill /f /im explorer.exe
del /f %LocalAppData%\Microsoft\Windows\Explorer\iconcache*
attrib -h %LocalAppData%\IconCache.db
del /f %LocalAppData%\IconCache.db
start explorer