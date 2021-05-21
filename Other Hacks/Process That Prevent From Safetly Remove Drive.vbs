'Run this command in powershell:'

Get-EventLog -LogName System -after (Get-Date).AddHours(-1) | Where-Object {$_.EventID -eq 225} | Sort-Object TimeGenerated | Format-Table -Wrap

'Than kill the process with the PID that is shown. You can now safetly remove your drive'
