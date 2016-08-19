del .\*.vcproj
del .\*.ncb
del .\*.suo
..\bin\premake4 %1 --os windows
..\build\MyEngine_Depends.sln