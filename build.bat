@echo off
setlocal enabledelayedexpansion

echo ========================================
echo 焊接监测软件编译脚本
echo ========================================

:: 检查是否存在Qt
where qmake >nul 2>&1
if %errorlevel% neq 0 (
    echo 错误: 未找到qmake，请确保Qt已安装并添加到PATH中
    echo 请将Qt的bin目录添加到系统PATH中，例如:
    echo C:\Qt\6.5.0\msvc2019_64\bin
    pause
    exit /b 1
)

:: 显示Qt版本
echo 检测到的Qt版本:
qmake --version

:: 创建build目录
if not exist "build" mkdir build
if not exist "bin" mkdir bin

echo.
echo 选择构建方式:
echo 1) Debug版本
echo 2) Release版本
echo 3) 两个版本都构建
set /p choice=请输入选择 (1-3): 

if "%choice%"=="1" goto build_debug
if "%choice%"=="2" goto build_release
if "%choice%"=="3" goto build_both
goto invalid_choice

:build_debug
echo.
echo 构建Debug版本...
qmake welding_monitor.pro CONFIG+=debug
if %errorlevel% neq 0 goto error
nmake clean >nul 2>&1
nmake
if %errorlevel% neq 0 goto error
echo Debug版本构建完成!
goto success

:build_release
echo.
echo 构建Release版本...
qmake welding_monitor.pro CONFIG+=release
if %errorlevel% neq 0 goto error
nmake clean >nul 2>&1
nmake
if %errorlevel% neq 0 goto error
echo Release版本构建完成!
goto success

:build_both
echo.
echo 构建Debug版本...
qmake welding_monitor.pro CONFIG+=debug
if %errorlevel% neq 0 goto error
nmake clean >nul 2>&1
nmake
if %errorlevel% neq 0 goto error
echo Debug版本构建完成!

echo.
echo 构建Release版本...
qmake welding_monitor.pro CONFIG+=release
if %errorlevel% neq 0 goto error
nmake clean >nul 2>&1
nmake
if %errorlevel% neq 0 goto error
echo Release版本构建完成!
goto success

:invalid_choice
echo 无效选择，请重新运行脚本
pause
exit /b 1

:error
echo.
echo 构建失败! 请检查错误信息并修复问题。
echo.
echo 常见问题:
echo 1. 确保Python已安装并在PATH中
echo 2. 确保pybind11已安装 (pip install pybind11)
echo 3. 检查welding_monitor.pro中的Python路径配置
echo 4. 确保所有源文件都存在
pause
exit /b 1

:success
echo.
echo ========================================
echo 构建成功!
echo ========================================
echo.
echo 可执行文件位置: .\bin\
if exist ".\bin\welding_monitor_d.exe" echo - Debug版本: welding_monitor_d.exe
if exist ".\bin\welding_monitor.exe" echo - Release版本: welding_monitor.exe
echo.
echo 运行前请确保:
echo 1. elec_Data.txt文件在工作目录中
echo 2. Python模块文件可用
echo 3. 所有设备驱动已安装
echo.
pause
