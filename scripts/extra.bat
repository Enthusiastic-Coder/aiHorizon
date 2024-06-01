del extra.obb.bak
copy extra.obb extra.obb.bak
c:\Qt\6.5.3\msvc2019_64\bin\rcc -binary ../extra.qrc -o extra.obb
copy extra.obb ..\android\extra\src\main\assets
