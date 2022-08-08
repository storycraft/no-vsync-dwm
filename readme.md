# NoVsyncDwm
Disable forced vsync rendering on dwm by patching `WaitForVsync` function of dwmcore.dll

Enjoy high cpu usage. 

## Notes
This does not disable vsync all the time. VSync will be still used when there are no user input or screen update.
