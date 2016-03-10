del .\*.vcproj
del .\*.ncb
del .\*.suo
..\bin\premake --target %1 --os windows
..\build\MyEngine_Depends.sln