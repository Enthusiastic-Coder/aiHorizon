call :copyAssets main
call :copyAssets patch
call :copyAssets extra


goto :eof

:copyAssets
mkdir __obbs
c:\Qt\6.5.3\msvc2019_64\bin\rcc -binary ../%1.qrc -o __obbs/%1.obb

IF ERRORLEVEL 1 (
    cscript //nologo errorpopup.vbs
    EXIT /B 1
)

mkdir ..\android\%1
copy %1\build.gradle ..\android\%1
mkdir ..\android\%1\src\main\assets
del ..\android\%1\src\main\assets\*.* /q
copy __obbs\%1.obb ..\android\%1\src\main\assets

