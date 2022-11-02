rmdir /s /q "ipch"
rmdir /s /q "Debug"
rmdir /s /q "Release"

rmdir /s /q .\Client_JH\Include\Debug
rmdir /s /q .\Client_JH\Include\ipch
rmdir /s /q .\Client_JH\Include\Release
rmdir /s /q .\Client_JH\Include\x64

rmdir /s /q .\Engine_JH\System\Include\Debug
rmdir /s /q .\Engine_JH\Utility\Include\Debug

del /f /q .\Client_JH\Bin\Client.exe
del /f /q .\Client_JH\Bin\Client.ilk
del /f /q .\Client_JH\Bin\Client.pdb
del /f /q Frame131C.VC.db
