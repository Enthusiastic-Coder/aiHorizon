del main.obb.bak
copy main.obb main.obb.bak
c:\Qt\6.5.3\msvc2019_64\bin\rcc -binary ../main.qrc -o main.obb
copy main.obb ..\android\main\src\main\assets