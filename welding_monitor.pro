QT += core widgets gui concurrent printsupport

CONFIG += c++17

TARGET = welding_monitor
TEMPLATE = app

# 源文件
SOURCES += \
    main.cpp \
    menu.cpp \
    qcustomplot.cpp

# 头文件
HEADERS += \
    menu.h \
    qcustomplot.h

# UI文件
FORMS += \
    menu.ui

# 资源文件
RESOURCES += \
    resource.qrc



# Windows特定配置
win32 {
    DEFINES += MS_WIN64
    DEFINES += WIN32_LEAN_AND_MEAN
    
    # 如果使用MSVC编译器
    msvc {
        QMAKE_CXXFLAGS += /bigobj
        DEFINES += _CRT_SECURE_NO_WARNINGS
    }
    
    # 如果使用MinGW编译器
    mingw {
        QMAKE_CXXFLAGS += -Wa,-mbig-obj
    }
}

# 调试配置
CONFIG(debug, debug|release) {
    DEFINES += QT_DEBUG
    TARGET = $$TARGET"_d"
}

# 发布配置
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

# 输出目录配置
DESTDIR = $$PWD/bin
OBJECTS_DIR = $$PWD/build/obj
MOC_DIR = $$PWD/build/moc
RCC_DIR = $$PWD/build/rcc
UI_DIR = $$PWD/build/ui

# 创建必要的目录
win32 {
    !exists($$DESTDIR) {
        system(mkdir $$quote($$shell_path($$DESTDIR)))
    }
    !exists($$OBJECTS_DIR) {
        system(mkdir $$quote($$shell_path($$OBJECTS_DIR)))
    }
    !exists($$MOC_DIR) {
        system(mkdir $$quote($$shell_path($$MOC_DIR)))
    }
    !exists($$RCC_DIR) {
        system(mkdir $$quote($$shell_path($$RCC_DIR)))
    }
    !exists($$UI_DIR) {
        system(mkdir $$quote($$shell_path($$UI_DIR)))
    }
}

# 版本信息
VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "Welding Monitor"
QMAKE_TARGET_PRODUCT = "焊接多信息在线监测软件"
QMAKE_TARGET_DESCRIPTION = "Welding Multi-Information Online Monitoring Software"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2024"

# 额外的编译器标志
QMAKE_CXXFLAGS += -std=c++17

# 如果需要OpenGL支持（QCustomPlot可选）
# CONFIG += opengl
# QT += opengl

message("Building welding monitor application...")
message("Qt version: $$QT_VERSION")
message("Target: $$TARGET")
message("Destination: $$DESTDIR")
