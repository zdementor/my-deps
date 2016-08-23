del .\*.vcproj
del .\*.ncb
del .\*.suo
..\bin\premake4 %1 --os windows
..\build\x32\MyEngine_Depends_x32.sln