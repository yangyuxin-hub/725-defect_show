# 焊接监测软件编译说明

## 系统要求

- Qt 6.x (推荐Qt 6.2或更高版本)
- Python 3.11 (或其他版本，需要相应调整)
- pybind11库
- C++17支持的编译器 (MSVC 2019+, MinGW-w64, 或Clang)

## 准备工作

### 1. 安装Qt 6
从Qt官网下载并安装Qt 6，确保包含以下组件：
- Qt Core, Widgets, GUI, Concurrent, PrintSupport模块
- Qt Creator IDE (可选但推荐)

### 2. 安装Python和pybind11
```bash
# 安装Python (如果尚未安装)
# 下载并安装Python 3.11

# 安装pybind11
pip install pybind11
```

### 3. 配置环境变量
确保以下路径在系统PATH中：
- Qt安装目录的bin文件夹 (例如: C:\Qt\6.5.0\msvc2019_64\bin)
- Python安装目录 (例如: C:\Python311)

## 编译步骤

### 方法1: 使用Qt Creator (推荐)
1. 打开Qt Creator
2. 选择 "Open Project"
3. 选择 `welding_monitor.pro` 文件
4. 配置Kit (选择适当的Qt版本和编译器)
5. 点击 "Build" 按钮

### 方法2: 使用命令行
1. 打开命令提示符并导航到项目目录
2. 运行以下命令：

```bash
# 生成Makefile
qmake welding_monitor.pro

# 编译项目
nmake         # 如果使用MSVC
# 或
mingw32-make  # 如果使用MinGW
# 或
make          # 如果使用其他编译器
```

## 配置说明

### Python路径配置
如果Python安装在非标准位置，请修改 `welding_monitor.pro` 文件中的以下行：

```qmake
PYTHON_PATH = C:/Python311  # 修改为您的Python安装路径
```

### 库文件名称
根据您的Python版本，可能需要修改库文件名称：

```qmake
LIBS += -L$$PYTHON_PATH/libs -lpython311  # 修改为对应的版本号
```

例如：
- Python 3.10: `-lpython310`
- Python 3.12: `-lpython312`

## 依赖文件

项目需要以下Python模块文件 (应在运行时可用)：
- `infrared_features.py`
- `image_featuresA.py`
- `I_features.py`
- `U_features.py`
- `S_features.py`
- `predictA.py`

## 常见问题

### 1. Python库链接错误
如果遇到Python库链接错误，请检查：
- Python路径是否正确
- 库文件名称是否匹配Python版本
- 是否安装了Python开发包 (python-dev)

### 2. pybind11找不到
确保pybind11已正确安装：
```bash
pip show pybind11  # 检查安装状态
pip install --upgrade pybind11  # 升级到最新版本
```

### 3. QCustomPlot相关错误
QCustomPlot源文件已包含在项目中，无需额外安装。

### 4. 资源文件错误
如果资源文件缺失，请将相应的图像文件添加到 `resource.qrc` 中。

## 输出

编译成功后，可执行文件将生成在 `bin` 目录中：
- Debug版本: `welding_monitor_d.exe`
- Release版本: `welding_monitor.exe`

## 注意事项

1. 确保所有依赖的Python模块在运行时可以被找到
2. 电流数据文件 `elec_Data.txt` 应位于可执行文件的工作目录中
3. 运行前请确保所有传感器设备驱动程序已正确安装
