// #include <Python.h>
// #include "pybind11/embed.h"
// #include <pybind11/numpy.h>
// #include <pybind11/stl.h>
// #ifdef _WIN64
// #define MS_WIN64
// #include <windows.h>
// #endif

#include "menu.h"
#include "ui_menu.h"

#include <QCoreApplication>
#include <iostream>
//#include "MecaVision.h"
// #include "frm_irbgrab_v4.h"
// #include "Modbus.h"
// #include "vision.h"
#include "qcustomplot.h"
// #include "streamingbufferedai.h"
// #include "configuredialog.h"
// #include <QProcess>
#include <QString>
#include <QDebug>
// #include <QtConcurrent/QtConcurrent>
// #include <QDateTime>
#include <fstream>
#include <string>
#include <QTextStream>
#include <QElapsedTimer>
#include <algorithm>
#include <QDir>
#include <QRegularExpression>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

#include <QApplication>
// #include "mysql.h"


// namespace py = pybind11;

// QString Menu::timeValue = QString("0s");
// const QString pre_filePath = "predict.txt"; // 保存文件的路径(txt)

// static py::object* infrared_features = nullptr;
// static py::object* image_features = nullptr;
// static py::object* I_features = nullptr;
// static py::object* U_features = nullptr;
// static py::object* S_features = nullptr;
// static py::object* pre = nullptr;
// std::atomic<bool> infraredProcessing(false); // 全局或类成员
// QElapsedTimer lastSaveTimer;
// QString newfileName;
// QStringList weldingStatusText = {"正常", "未焊透", "焊偏", "气孔", "焊漏"};


Menu::Menu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Menu)
    , windowSize(5000)
    , samplingRate(40960)
    , updatePointsPerSecond(40960)  // 每秒处理40960个点，与采样率一致
    , pointsPerUpdate(2048)  // 每50ms更新2048个点 (40960/20)
    , currentDataIndex(0)
    , currentSpecFileIndex(0)
    , currentImageIndex(0)
    , currentIrImageIndex(0)
    , isPlayingSpec(false)
    , isPlayingImages(false)
    , isPlayingIrImages(false)
    , imageScene(nullptr)
    , imageItem(nullptr)
    , currentDefectPath("")
    , isPlotting(false)
{
    ui->setupUi(this);
    
    // 初始化定时器
    plotUpdateTimer = new QTimer(this);
    connect(plotUpdateTimer, &QTimer::timeout, this, &Menu::updatePlotData);
    
    // 初始化光谱播放定时器 (4Hz = 250ms间隔)
    specPlayTimer = new QTimer(this);
    connect(specPlayTimer, &QTimer::timeout, this, &Menu::updateSpecPlayback);
    
    // 初始化熔池图像播放定时器 (33ms间隔 = 30FPS，智能抽帧播放)
    imagePlayTimer = new QTimer(this);
    connect(imagePlayTimer, &QTimer::timeout, this, &Menu::updateImagePlayback);
    
    // 初始化红外图像播放定时器 (111ms间隔 = 9FPS，70秒内播放595张)
    irImagePlayTimer = new QTimer(this);
    connect(irImagePlayTimer, &QTimer::timeout, this, &Menu::updateIrImagePlayback);
    
    // 初始化x轴数据
    xAxisData.resize(windowSize);
    for (int i = 0; i < windowSize; ++i) {
        xAxisData[i] = i;
    }

    QTimer::singleShot(0, this, [this](){

    // 按键使能设置 - 只保留缺陷数据按钮
    // ui->startButton->setEnabled(true);
    // ui->stopButton->setEnabled(false);
    // ui->saveButton->setEnabled(false);

    // 左侧文本框
    // ui->wireSpeedShow->setReadOnly(true); // 设置为只读

    // 电流图表 - 只保留基本的绘图设置
    ui->currentPlot->setMinimumSize(300, 200);
    ui->currentPlot->setBackground(Qt::transparent); // 将图表背景设置为透明
    ui->currentPlot->setStyleSheet("background: transparent;"); // 将图表背景设置为透明
    ui->currentPlot->addGraph(); // 添加一个图表
    ui->currentPlot->graph(0)->setPen(QPen(Qt::red, 1)); // 设置图表颜色为红色
    ui->currentPlot->yAxis->setRange(0,500);
//  ui->currentPlot->xAxis->setLabel("时间");
//  ui->currentPlot->yAxis->setLabel("电流(A)");
    ui->currentPlot->xAxis->grid()->setVisible(true); // 开启网格线显示
    ui->currentPlot->yAxis->grid()->setVisible(true); // 开启网格线显示
    ui->currentPlot->xAxis2->setVisible(true);  // 显示上轴
    ui->currentPlot->yAxis2->setVisible(true);  // 显示右轴
    ui->currentPlot->xAxis->setBasePen(QPen(Qt::gray));  // 设置 x 轴颜色
    ui->currentPlot->yAxis->setBasePen(QPen(Qt::gray));  // 设置 y 轴颜色
    ui->currentPlot->xAxis2->setBasePen(QPen(Qt::gray)); // 设置上轴颜色
    ui->currentPlot->yAxis2->setBasePen(QPen(Qt::gray)); // 设置右轴颜色
    ui->currentPlot->xAxis->grid()->setPen(QPen(QColor(195, 195, 195), 0.5, Qt::SolidLine));
    ui->currentPlot->yAxis->grid()->setPen(QPen(QColor(195, 195, 195), 0.5, Qt::SolidLine));
    ui->currentPlot->xAxis->setTickPen(Qt::NoPen);  // 设置主刻度线为透明
    ui->currentPlot->yAxis->setTickPen(Qt::NoPen);  // 设置主刻度线为透明
    ui->currentPlot->xAxis->setSubTickPen(Qt::NoPen);  // 设置次刻度线为透明
    ui->currentPlot->yAxis->setSubTickPen(Qt::NoPen);  // 设置次刻度线为透明
    ui->currentPlot->xAxis2->setTickLabels(false);  // 隐藏上轴的刻度标签
    ui->currentPlot->yAxis2->setTickLabels(false);  // 隐藏右轴的刻度标签
    ui->currentPlot->xAxis2->setTicks(false);  // 隐藏上轴的刻度线
    ui->currentPlot->yAxis2->setTicks(false);  // 隐藏右轴的刻度线
    // ui->currentPlot->hide();  // 初始隐藏图表

    // 电压图表设置
    ui->voltagePlot->setMinimumSize(300, 200);
    ui->voltagePlot->setBackground(Qt::transparent);
    ui->voltagePlot->setStyleSheet("background: transparent;");
    ui->voltagePlot->addGraph();
    ui->voltagePlot->graph(0)->setPen(QPen(Qt::blue, 1)); // 设置图表颜色为蓝色
    ui->voltagePlot->yAxis->setRange(0,150);
    ui->voltagePlot->xAxis->grid()->setVisible(true);
    ui->voltagePlot->yAxis->grid()->setVisible(true);
    ui->voltagePlot->xAxis2->setVisible(true);
    ui->voltagePlot->yAxis2->setVisible(true);
    ui->voltagePlot->xAxis->setBasePen(QPen(Qt::gray));
    ui->voltagePlot->yAxis->setBasePen(QPen(Qt::gray));
    ui->voltagePlot->xAxis2->setBasePen(QPen(Qt::gray));
    ui->voltagePlot->yAxis2->setBasePen(QPen(Qt::gray));
    ui->voltagePlot->xAxis->grid()->setPen(QPen(QColor(195, 195, 195), 0.5, Qt::SolidLine));
    ui->voltagePlot->yAxis->grid()->setPen(QPen(QColor(195, 195, 195), 0.5, Qt::SolidLine));
    ui->voltagePlot->xAxis->setTickPen(Qt::NoPen);
    ui->voltagePlot->yAxis->setTickPen(Qt::NoPen);
    ui->voltagePlot->xAxis->setSubTickPen(Qt::NoPen);
    ui->voltagePlot->yAxis->setSubTickPen(Qt::NoPen);
    ui->voltagePlot->xAxis2->setTickLabels(false);
    ui->voltagePlot->yAxis2->setTickLabels(false);
    ui->voltagePlot->xAxis2->setTicks(false);
    ui->voltagePlot->yAxis2->setTicks(false);
    // ui->voltagePlot->hide();  // 初始隐藏图表

    // 声音图表设置
    ui->soundPlot->setMinimumSize(300, 200);
    ui->soundPlot->setBackground(Qt::transparent);
    ui->soundPlot->setStyleSheet("background: transparent;");
    ui->soundPlot->addGraph();
    ui->soundPlot->graph(0)->setPen(QPen(Qt::green, 1)); // 设置图表颜色为绿色
    ui->soundPlot->yAxis->setRange(-5,5);
    ui->soundPlot->xAxis->grid()->setVisible(true);
    ui->soundPlot->yAxis->grid()->setVisible(true);
    ui->soundPlot->xAxis2->setVisible(true);
    ui->soundPlot->yAxis2->setVisible(true);
    ui->soundPlot->xAxis->setBasePen(QPen(Qt::gray));
    ui->soundPlot->yAxis->setBasePen(QPen(Qt::gray));
    ui->soundPlot->xAxis2->setBasePen(QPen(Qt::gray));
    ui->soundPlot->yAxis2->setBasePen(QPen(Qt::gray));
    ui->soundPlot->xAxis->grid()->setPen(QPen(QColor(195, 195, 195), 0.5, Qt::SolidLine));
    ui->soundPlot->yAxis->grid()->setPen(QPen(QColor(195, 195, 195), 0.5, Qt::SolidLine));
    ui->soundPlot->xAxis->setTickPen(Qt::NoPen);
    ui->soundPlot->yAxis->setTickPen(Qt::NoPen);
    ui->soundPlot->xAxis->setSubTickPen(Qt::NoPen);
    ui->soundPlot->yAxis->setSubTickPen(Qt::NoPen);
    ui->soundPlot->xAxis2->setTickLabels(false);
    ui->soundPlot->yAxis2->setTickLabels(false);
    ui->soundPlot->xAxis2->setTicks(false);
    ui->soundPlot->yAxis2->setTicks(false);
    // ui->soundPlot->hide();  // 初始隐藏图表

    // 光谱图表设置
    ui->specPlot->setMinimumSize(300, 200);
    ui->specPlot->setBackground(Qt::transparent);
    ui->specPlot->setStyleSheet("background: transparent;");
    ui->specPlot->addGraph();
    ui->specPlot->graph(0)->setPen(QPen(QColor(255, 140, 0), 1)); // 设置图表颜色为深橙色
    ui->specPlot->yAxis->setRange(0,300);
    ui->specPlot->xAxis->grid()->setVisible(true);
    ui->specPlot->yAxis->grid()->setVisible(true);
    ui->specPlot->xAxis2->setVisible(true);
    ui->specPlot->yAxis2->setVisible(true);
    ui->specPlot->xAxis->setBasePen(QPen(Qt::gray));
    ui->specPlot->yAxis->setBasePen(QPen(Qt::gray));
    ui->specPlot->xAxis2->setBasePen(QPen(Qt::gray));
    ui->specPlot->yAxis2->setBasePen(QPen(Qt::gray));
    ui->specPlot->xAxis->grid()->setPen(QPen(QColor(195, 195, 195), 0.5, Qt::SolidLine));
    ui->specPlot->yAxis->grid()->setPen(QPen(QColor(195, 195, 195), 0.5, Qt::SolidLine));
    ui->specPlot->xAxis->setTickPen(Qt::NoPen);
    ui->specPlot->yAxis->setTickPen(Qt::NoPen);
    ui->specPlot->xAxis->setSubTickPen(Qt::NoPen);
    ui->specPlot->yAxis->setSubTickPen(Qt::NoPen);
    ui->specPlot->xAxis2->setTickLabels(false);
    ui->specPlot->yAxis2->setTickLabels(false);
    ui->specPlot->xAxis2->setTicks(false);
    ui->specPlot->yAxis2->setTicks(false);
    // ui->specPlot->hide();  // 初始隐藏图表

    // 初始化gvMain用于显示熔池图像
    imageScene = new QGraphicsScene(this);
    ui->gvMain->setScene(imageScene);
    ui->gvMain->setRenderHint(QPainter::Antialiasing);
    ui->gvMain->setDragMode(QGraphicsView::NoDrag);
    ui->gvMain->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->gvMain->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->gvMain->setFrameStyle(QFrame::NoFrame); // 去除边框
    ui->gvMain->setContentsMargins(0, 0, 0, 0); // 去除边距
    ui->gvMain->setAlignment(Qt::AlignCenter); // 居中对齐
    ui->gvMain->setResizeAnchor(QGraphicsView::AnchorViewCenter); // 缩放时以中心为锚点
    // 注意：setViewportMargins是protected方法，不能直接调用
    
    // 创建图像项
    imageItem = new QGraphicsPixmapItem();
    imageScene->addItem(imageItem);

    // 初始化lblImage用于显示红外图像
    ui->lblImage->setScaledContents(true); // 允许图像缩放
    ui->lblImage->setAlignment(Qt::AlignCenter); // 居中对齐
    ui->lblImage->setStyleSheet("color: black; border: 1px solid black; background-color: transparent;");

    // 初始化缺陷类型列表
    defectTypes << "A18内部气孔" << "A35焊漏" << "A37焊偏" << "A40未焊透" << "C28错边";

    // 其他图表初始化都已注释
    // 光谱图表等都已注释

    });

// 初始化焊接类型下拉框 - 已注释
// ui->comboBox_3->addItem("打底焊");
// ui->comboBox_3->addItem("填充焊");
// ui->comboBox_4->addItem("横焊"); 
// ui->comboBox_4->addItem("立焊");
// ui->comboBox_4->addItem("角焊");

    // Python模块初始化 - 已注释
    // infrared_features = new py::object(py::module_::import("infrared_features"));
    // image_features = new py::object(py::module_::import("image_featuresA"));
    // I_features = new py::object(py::module_::import("I_features"));
    // U_features = new py::object(py::module_::import("U_features"));
    // S_features = new py::object(py::module_::import("S_features"));
    // pre = new py::object(py::module_::import("predictA"));

    /// 红外测温 - 已注释 ///
    //if (!infTemp.cameraCheck()) qDebug() << "没有检测到红外测温设备";

    // openSubModule(); // 在打开软件时即打开子模块并隐藏子模块界面

    /// 机器人速度 - 已注释 ///
    // robotSpeed.ui = ui;

    // 所有其他模块的初始化都已注释
}

Menu::~Menu()
{
    if (plotUpdateTimer) {
        plotUpdateTimer->stop();
    }
    if (specPlayTimer) {
        specPlayTimer->stop();
    }
    if (imagePlayTimer) {
        imagePlayTimer->stop();
    }
    if (irImagePlayTimer) {
        irImagePlayTimer->stop();
    }
    delete ui;
}

// 焊接缺陷数据库 - 显示缺陷类型选择对话框
void Menu::on_defect_data_clicked()
{
    // 如果正在播放，停止所有播放
    if (isPlayingSpec || isPlayingImages || isPlayingIrImages || isPlotting) {
        if (isPlayingSpec) {
            specPlayTimer->stop();
            isPlayingSpec = false;
            qDebug() << "停止光谱播放";
        }
        if (isPlayingImages) {
            imagePlayTimer->stop();
            isPlayingImages = false;
            qDebug() << "停止熔池图像播放";
        }
        if (isPlayingIrImages) {
            irImagePlayTimer->stop();
            isPlayingIrImages = false;
            qDebug() << "停止红外图像播放";
        }
        if (isPlotting) {
            plotUpdateTimer->stop();
            isPlotting = false;
            qDebug() << "停止当前数据绘图";
        }
        return;
    }
    
    // 显示缺陷类型选择对话框
    showDefectSelectionDialog();
}

// 显示缺陷类型选择对话框
void Menu::showDefectSelectionDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("选择焊接缺陷类型");
    dialog.setFixedSize(400, 300);
    
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    
    // 添加说明标签
    QLabel* label = new QLabel("请选择要查看的焊接缺陷类型：", &dialog);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    
    // 创建缺陷类型列表
    QListWidget* listWidget = new QListWidget(&dialog);
    for (const QString& defectType : defectTypes) {
        listWidget->addItem(defectType);
    }
    layout->addWidget(listWidget);
    
    // 添加按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("确定", &dialog);
    QPushButton* cancelButton = new QPushButton("取消", &dialog);
    
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);
    
    // 连接信号
    connect(okButton, &QPushButton::clicked, [&dialog, listWidget, this]() {
        if (listWidget->currentItem()) {
            QString selectedDefect = listWidget->currentItem()->text();
            qDebug() << "选择的缺陷类型:" << selectedDefect;
            dialog.accept();
            loadDefectData(selectedDefect);
        }
    });
    
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    // 双击选择
    connect(listWidget, &QListWidget::itemDoubleClicked, [&dialog, this](QListWidgetItem* item) {
        QString selectedDefect = item->text();
        qDebug() << "选择的缺陷类型:" << selectedDefect;
        dialog.accept();
        loadDefectData(selectedDefect);
    });
    
    dialog.exec();
}

// 加载指定缺陷类型的数据
void Menu::loadDefectData(const QString& defectType)
{
    // 设置当前缺陷路径
    currentDefectPath = "D:/codebuddy/test/defect_data/" + defectType;
    qDebug() << "加载缺陷数据路径:" << currentDefectPath;
    qDebug() << "===== 开始加载缺陷类型:" << defectType << " =====";
    
    // 更新窗口标题显示当前缺陷类型
    this->setWindowTitle("焊接监测软件 - 当前缺陷类型: " + defectType);
    
    // 加载光谱文件列表
    QDir specDir(currentDefectPath + "/光谱");
    QStringList filters;
    filters << "*.txt";
    QStringList allFiles = specDir.entryList(filters, QDir::Files, QDir::Name);
    
    // 按数字排序
    specFileList.clear();
    for (const QString &fileName : allFiles) {
        specFileList.append(fileName);
    }
    
    // 自然排序（按数字大小）
    std::sort(specFileList.begin(), specFileList.end(), [](const QString &a, const QString &b) {
        QRegularExpression re("(\\d+)");
        QRegularExpressionMatch matchA = re.match(a);
        QRegularExpressionMatch matchB = re.match(b);
        if (matchA.hasMatch() && matchB.hasMatch()) {
            return matchA.captured(1).toInt() < matchB.captured(1).toInt();
        }
        return a < b;
    });
    
    if (specFileList.isEmpty()) {
        qDebug() << "spec文件夹中没有找到.txt文件";
        return;
    }

    qDebug() << "找到" << specFileList.size() << "个光谱文件，开始播放...";
    qDebug() << "光谱文件列表:" << specFileList;
    
    // 加载熔池图像文件列表
    QDir imageDir(currentDefectPath + "/熔池图像");
    QStringList imageFilters;
    imageFilters << "*.jpg" << "*.JPG" << "*.png" << "*.PNG";
    QStringList allImageFiles = imageDir.entryList(imageFilters, QDir::Files, QDir::Name);
    
    // 按数字排序
    imageFileList.clear();
    for (const QString &fileName : allImageFiles) {
        imageFileList.append(fileName);
    }
    
    // 自然排序（按数字大小）
    std::sort(imageFileList.begin(), imageFileList.end(), [](const QString &a, const QString &b) {
        QRegularExpression re("(\\d+)");
        QRegularExpressionMatch matchA = re.match(a);
        QRegularExpressionMatch matchB = re.match(b);
        if (matchA.hasMatch() && matchB.hasMatch()) {
            return matchA.captured(1).toInt() < matchB.captured(1).toInt();
        }
        return a < b;
    });
    
    if (imageFileList.isEmpty()) {
        qDebug() << "熔池图像文件夹中没有找到图像文件";
    } else {
        qDebug() << "找到" << imageFileList.size() << "张熔池图像，计划70秒内播放完成";
        
        // 计算抽帧策略
        int totalImages = imageFileList.size();
        double targetDuration = 70.0; // 70秒
        double frameRate = totalImages / targetDuration;
        qDebug() << "原始帧率需求:" << frameRate << "FPS";
        
        // 计算最优抽帧策略
        int actualFrameRate = 30; // 设置为30FPS，更稳定
        int skipFrames = qMax(1, qRound(frameRate / actualFrameRate));
        double actualDuration = (totalImages / skipFrames) / (double)actualFrameRate;
        qDebug() << "实际播放帧率:" << actualFrameRate << "FPS";
        qDebug() << "跳帧策略: 每" << skipFrames << "帧选1帧播放";
        qDebug() << "预计播放时长:" << QString::number(actualDuration, 'f', 1) << "秒";
    }
    
    // 加载红外图像文件列表
    QDir irImageDir(currentDefectPath + "/红外图像");
    QStringList irImageFilters;
    irImageFilters << "*.png" << "*.PNG" << "*.jpg" << "*.JPG";
    QStringList allIrImageFiles = irImageDir.entryList(irImageFilters, QDir::Files, QDir::Name);
    
    // 按数字排序
    irImageFileList.clear();
    for (const QString &fileName : allIrImageFiles) {
        irImageFileList.append(fileName);
    }
    
    // 按红外图像命名规则排序：irdata0xy_ij.png，先按组号xy排序，再按组内序号ij排序
    std::sort(irImageFileList.begin(), irImageFileList.end(), [](const QString &a, const QString &b) {
        // 解析文件名格式：irdata0xy_ij.png (xy=组号, ij=组内序号)
        QRegularExpression re("irdata(\\d{3})_(\\d{2})\\.png");
        QRegularExpressionMatch matchA = re.match(a);
        QRegularExpressionMatch matchB = re.match(b);
        
        if (matchA.hasMatch() && matchB.hasMatch()) {
            int groupA = matchA.captured(1).toInt(); // 组号xy (000, 001, 002, ...)
            int indexA = matchA.captured(2).toInt(); // 组内序号ij (01, 02, 03, ...)
            int groupB = matchB.captured(1).toInt();
            int indexB = matchB.captured(2).toInt();
            
            // 先比较组号，组号相同时比较组内序号
            if (groupA == groupB) {
                return indexA < indexB;
            }
            return groupA < groupB;
        }
        
        // 如果无法解析，使用字符串比较
        return a < b;
    });
    
    if (irImageFileList.isEmpty()) {
        qDebug() << "红外图像文件夹中没有找到图像文件";
    } else {
        qDebug() << "找到" << irImageFileList.size() << "张红外图像，计划70秒内播放完成";
        
        // 显示前几个文件名验证排序
        qDebug() << "红外图像排序验证（前10个）:";
        for (int i = 0; i < qMin(10, irImageFileList.size()); ++i) {
            qDebug() << "  " << (i+1) << ":" << irImageFileList[i];
        }
        if (irImageFileList.size() > 10) {
            qDebug() << "  ... 还有" << (irImageFileList.size() - 10) << "张图像";
        }
        
        // 计算播放策略
        int totalIrImages = irImageFileList.size();
        double irFrameRate = totalIrImages / 70.0; // 70秒
        qDebug() << "红外图像播放帧率:" << irFrameRate << "FPS";
        
        // 使用约9FPS播放（111ms间隔）
        double actualIrDuration = totalIrImages / 9.0;
        qDebug() << "红外图像预计播放时长:" << QString::number(actualIrDuration, 'f', 1) << "秒";
    }

    // 使用缺陷数据路径打开数据文件
    QString elecFilePath = currentDefectPath + "/其他数据/elec_Data.txt";
    QString voltFilePath = currentDefectPath + "/其他数据/volt_Data.txt";
    QString soundFilePath = currentDefectPath + "/其他数据/sound_Data.txt";
    
    // 清空之前的数据
    allElecData.clear();
    allVoltData.clear();
    allSoundData.clear();
    allSpecDataX.clear();
    allSpecDataY.clear();
    elecDisplayData.clear();
    voltDisplayData.clear();
    soundDisplayData.clear();
    
    // 读取电流数据
    QFile elecFile(elecFilePath);
    if (!elecFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开elec_Data.txt文件:" << elecFilePath;
        return;
    }
    
    QTextStream elecIn(&elecFile);
    while (!elecIn.atEnd()) {
        QString line = elecIn.readLine();
        bool ok;
        double value = line.toDouble(&ok);
        if (ok) {
            allElecData.append(value);
        }
    }
    elecFile.close();

    qDebug() << "读取完elec_Data.txt文件:";

    // 读取电压数据
    QFile voltFile(voltFilePath);
    if (!voltFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开volt_Data.txt文件:" << voltFilePath;
        return;
    }
    
    QTextStream voltIn(&voltFile);
    while (!voltIn.atEnd()) {
        QString line = voltIn.readLine();
        bool ok;
        double value = line.toDouble(&ok);
        if (ok) {
            allVoltData.append(value);
        }
    }
    voltFile.close();

    // 读取声音数据
    QFile soundFile(soundFilePath);
    if (!soundFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开sound_Data.txt文件:" << soundFilePath;
        return;
    }
    
    QTextStream soundIn(&soundFile);
    while (!soundIn.atEnd()) {
        QString line = soundIn.readLine();
        bool ok;
        double value = line.toDouble(&ok);
        if (ok) {
            allSoundData.append(value);
        }
    }
    soundFile.close();

    if (allElecData.isEmpty() || allVoltData.isEmpty() || allSoundData.isEmpty()) {
        qDebug() << "数据文件为空或没有有效数据";
        qDebug() << "电流数据点数:" << allElecData.size();
        qDebug() << "电压数据点数:" << allVoltData.size(); 
        qDebug() << "声音数据点数:" << allSoundData.size();
        return;
    }

    qDebug() << "成功加载数据:";
    qDebug() << "电流数据点数:" << allElecData.size();
    qDebug() << "电压数据点数:" << allVoltData.size();
    qDebug() << "声音数据点数:" << allSoundData.size();
    qDebug() << "采样率:" << samplingRate << "Hz";
    qDebug() << "窗口大小:" << windowSize << "个点";
    qDebug() << "更新间隔: 50ms，每次更新" << pointsPerUpdate << "个点，每秒显示" << updatePointsPerSecond << "个点";

    // 显示所有图表
    ui->currentPlot->show();
    ui->voltagePlot->show();
    ui->soundPlot->show();
    ui->specPlot->show();       // 显示光谱图表
    ui->currentground->hide();
    ui->voltageground->hide();  // 隐藏电压背景框架
    ui->soundground->hide();    // 隐藏声音背景框架
    ui->specground->hide();     // 隐藏光谱背景框架

    // 初始化显示数据
    currentDataIndex = 0;
    elecDisplayData.resize(windowSize);
    elecDisplayData.fill(0.0);
    voltDisplayData.resize(windowSize);
    voltDisplayData.fill(0.0);
    soundDisplayData.resize(windowSize);
    soundDisplayData.fill(0.0);

    // 填入初始数据
    int initialPoints = qMin(windowSize, qMin(allElecData.size(), qMin(allVoltData.size(), allSoundData.size())));
    for (int i = 0; i < initialPoints; ++i) {
        elecDisplayData[i] = allElecData[i];
        voltDisplayData[i] = allVoltData[i];
        soundDisplayData[i] = allSoundData[i];
        xAxisData[i] = i;  // 初始X轴从0开始
    }
    currentDataIndex = initialPoints;

    // 设置电流图表初始数据
    ui->currentPlot->graph(0)->data()->clear();
    ui->currentPlot->graph(0)->setData(xAxisData, elecDisplayData);
    ui->currentPlot->xAxis->setRange(0, initialPoints - 1);
    
    // 计算电流y轴范围
    double elecMinVal = *std::min_element(allElecData.begin(), allElecData.end());
    double elecMaxVal = *std::max_element(allElecData.begin(), allElecData.end());
    double elecRange = elecMaxVal - elecMinVal;
    ui->currentPlot->yAxis->setRange(elecMinVal - elecRange * 0.1, elecMaxVal + elecRange * 0.1);

    // 设置电压图表初始数据
    ui->voltagePlot->graph(0)->data()->clear();
    ui->voltagePlot->graph(0)->setData(xAxisData, voltDisplayData);
    ui->voltagePlot->xAxis->setRange(0, initialPoints - 1);
    
    // 计算电压y轴范围
    double voltMinVal = *std::min_element(allVoltData.begin(), allVoltData.end());
    double voltMaxVal = *std::max_element(allVoltData.begin(), allVoltData.end());
    double voltRange = voltMaxVal - voltMinVal;
    ui->voltagePlot->yAxis->setRange(voltMinVal - voltRange * 0.1, voltMaxVal + voltRange * 0.1);

    // 设置声音图表初始数据
    ui->soundPlot->graph(0)->data()->clear();
    ui->soundPlot->graph(0)->setData(xAxisData, soundDisplayData);
    ui->soundPlot->xAxis->setRange(0, initialPoints - 1);
    
    // 计算声音y轴范围
    double soundMinVal = *std::min_element(allSoundData.begin(), allSoundData.end());
    double soundMaxVal = *std::max_element(allSoundData.begin(), allSoundData.end());
    double soundRange = soundMaxVal - soundMinVal;
    ui->soundPlot->yAxis->setRange(soundMinVal - soundRange * 0.1, soundMaxVal + soundRange * 0.1);

    // 重新绘制图表
    ui->currentPlot->replot();
    ui->voltagePlot->replot();
    ui->soundPlot->replot();

    // 开始实时更新电流、电压、声音数据
    isPlotting = true;
    plotUpdateTimer->start(50); // 50ms间隔
    
    // 开始光谱播放
    currentSpecFileIndex = 0;
    isPlayingSpec = true;
    updateSpecPlayback(); // 立即显示第一个光谱文件
    specPlayTimer->start(250); // 4Hz = 250ms间隔
    
    // 开始熔池图像播放（如果有图像文件）
    if (!imageFileList.isEmpty()) {
        currentImageIndex = 0;
        isPlayingImages = true;
        
        // 立即显示第一张图像并确保占满控件
        updateImagePlayback();
        
        imagePlayTimer->start(33); // 30FPS = 33ms间隔
        qDebug() << "开始熔池图像播放，30FPS频率";
    }
    
    // 开始红外图像播放（如果有红外图像文件）
    if (!irImageFileList.isEmpty()) {
        currentIrImageIndex = 0;
        isPlayingIrImages = true;
        
        // 立即显示第一张红外图像
        updateIrImagePlayback();
        
        irImagePlayTimer->start(111); // 约9FPS = 111ms间隔
        qDebug() << "开始红外图像播放，9FPS频率";
    }
    
    qDebug() << "开始实时数据显示，50ms间隔更新";
    qDebug() << "开始光谱播放，4Hz频率";
}

// 按钮：开始采集 - 清空画面回到原始界面状态
void Menu::on_startButton_clicked()
{
    qDebug() << "点击开始采集按钮 - 清空画面回到原始状态";
    
    // 停止所有正在进行的播放和绘图
    if (isPlayingSpec) {
        specPlayTimer->stop();
        isPlayingSpec = false;
        qDebug() << "停止光谱播放";
    }
    
    if (isPlayingImages) {
        imagePlayTimer->stop();
        isPlayingImages = false;
        qDebug() << "停止熔池图像播放";
    }
    
    if (isPlayingIrImages) {
        irImagePlayTimer->stop();
        isPlayingIrImages = false;
        qDebug() << "停止红外图像播放";
    }
    
    if (isPlotting) {
        plotUpdateTimer->stop();
        isPlotting = false;
        qDebug() << "停止当前数据绘图";
    }
    
    // 清空所有图表数据
    ui->currentPlot->graph(0)->data()->clear();
    ui->voltagePlot->graph(0)->data()->clear();
    ui->soundPlot->graph(0)->data()->clear();
    ui->specPlot->graph(0)->data()->clear();
    
    // 重置图表显示范围到初始状态
    ui->currentPlot->yAxis->setRange(0, 500);
    ui->voltagePlot->yAxis->setRange(0, 150);
    ui->soundPlot->yAxis->setRange(-5, 5);
    ui->specPlot->yAxis->setRange(0, 300);
    
    // 重新绘制清空的图表
    ui->currentPlot->replot();
    ui->voltagePlot->replot();
    ui->soundPlot->replot();
    ui->specPlot->replot();
    
    
    // 确保图像框架可见
    ui->gvMain->show(); // 熔池图像视图
    ui->lblImage->show(); // 红外图像标签
    
    // 清空图像显示
    if (imageScene) {
        imageScene->clear();
        // 重新创建图像项，因为clear()会删除所有项
        imageItem = new QGraphicsPixmapItem();
        imageScene->addItem(imageItem);
        qDebug() << "重新创建熔池图像项";
    }
    
    // 清空红外图像显示，恢复到初始状态
    ui->lblImage->clear();
    ui->lblImage->setText("Image"); // 恢复到初始文本
    ui->lblImage->setStyleSheet("color: black; border: 1px solid black; background-color: transparent;");
    
    // 清空所有数据容器
    allElecData.clear();
    allVoltData.clear();
    allSoundData.clear();
    allSpecDataX.clear();
    allSpecDataY.clear();
    elecDisplayData.clear();
    voltDisplayData.clear();
    soundDisplayData.clear();
    specFileList.clear();
    imageFileList.clear();
    irImageFileList.clear();
    
    // 重置索引
    currentDataIndex = 0;
    currentSpecFileIndex = 0;
    currentImageIndex = 0;
    currentIrImageIndex = 0;
    
    // 清空当前缺陷路径
    currentDefectPath = "";
    
    // 重新初始化显示数据数组，防止访问越界
    xAxisData.resize(windowSize);
    elecDisplayData.resize(windowSize);
    voltDisplayData.resize(windowSize);
    soundDisplayData.resize(windowSize);
    
    // 用初始值填充数组
    for (int i = 0; i < windowSize; ++i) {
        xAxisData[i] = i;
        elecDisplayData[i] = 0.0;
        voltDisplayData[i] = 0.0;
        soundDisplayData[i] = 0.0;
    }
    
    // 重置窗口标题
    this->setWindowTitle("焊接监测软件");
    
    qDebug() << "界面已清空，回到原始状态";
}

// 所有其他函数都已注释掉
// void Menu::on_stopButton_clicked() { ... }
// void Menu::on_saveButton_clicked() { ... }
// void Menu::on_craft_data_clicked() { ... }
// void Menu::captureFrame() { ... }
// void Menu::captureInfrared() { ... }
// void Menu::openSubModule() { ... }
// void Menu::openConfiguredialog() { ... }

// 简化：直接通过成员变量控制重置

// 实时更新绘图数据 - 50ms间隔更新，保持与40160Hz采样率的时间同步
void Menu::updatePlotData()
{
    if (!isPlotting || allElecData.isEmpty() || allVoltData.isEmpty() || allSoundData.isEmpty()) {
        return;
    }
    
    // 安全检查：确保显示数据数组大小正确
    if (elecDisplayData.size() != windowSize || voltDisplayData.size() != windowSize || 
        soundDisplayData.size() != windowSize || xAxisData.size() != windowSize) {
        qDebug() << "显示数据数组大小不正确，重新初始化";
        elecDisplayData.resize(windowSize);
        voltDisplayData.resize(windowSize);
        soundDisplayData.resize(windowSize);
        xAxisData.resize(windowSize);
        for (int i = 0; i < windowSize; ++i) {
            elecDisplayData[i] = 0.0;
            voltDisplayData[i] = 0.0;
            soundDisplayData[i] = 0.0;
            xAxisData[i] = i;
        }
    }

    // 简化：每次开始绘图时重新初始化定时器
    static QElapsedTimer realTimeTimer;
    static int lastDataIndex = -1;
    
    // 如果是新的数据序列，重新开始计时
    if (lastDataIndex != currentDataIndex || currentDataIndex == 0) {
        realTimeTimer.start();
        lastDataIndex = currentDataIndex;
    }
    
    // 计算基于实际时间应该到达的数据索引
    qint64 elapsedMs = realTimeTimer.elapsed();
    int expectedDataIndex = currentDataIndex + (elapsedMs * samplingRate / 1000);
    
    // 检查是否还有数据需要显示（以最小数据量为准）
    int minDataSize = qMin(allElecData.size(), qMin(allVoltData.size(), allSoundData.size()));
    if (expectedDataIndex >= minDataSize) {
        plotUpdateTimer->stop();
        isPlotting = false;
        qDebug() << "所有数据已显示完毕，停止绘图";
        return;
    }

    // 如果当前数据索引落后于预期，需要跳跃到正确位置
    if (currentDataIndex < expectedDataIndex) {
        currentDataIndex = expectedDataIndex;
        qDebug() << "时间同步：跳转到数据索引" << currentDataIndex;
    }

    // 确保不超出数据范围
    if (currentDataIndex >= minDataSize) {
        plotUpdateTimer->stop();
        isPlotting = false;
        qDebug() << "数据已处理完毕";
        return;
    }

    // 滑动窗口显示：显示当前时间点前面的2000个数据点
    int displayStartIndex = qMax(0, currentDataIndex - windowSize);
    
    // 填充显示数据和X轴数据
    for (int i = 0; i < windowSize; ++i) {
        int dataIndex = displayStartIndex + i;
        if (dataIndex < minDataSize && dataIndex >= 0) {
            elecDisplayData[i] = allElecData[dataIndex];
            voltDisplayData[i] = allVoltData[dataIndex];
            soundDisplayData[i] = allSoundData[dataIndex];
            xAxisData[i] = dataIndex;
        } else {
            elecDisplayData[i] = 0.0;
            voltDisplayData[i] = 0.0;
            soundDisplayData[i] = 0.0;
            xAxisData[i] = dataIndex;
        }
    }

    // 更新X轴范围 - 显示当前窗口的真实索引范围
    double xMin = xAxisData[0];
    double xMax = xAxisData[windowSize - 1];
    
    // 更新电流图表
    ui->currentPlot->xAxis->setRange(xMin, xMax);
    ui->currentPlot->graph(0)->setData(xAxisData, elecDisplayData);
    ui->currentPlot->replot();
    
    // 更新电压图表
    ui->voltagePlot->xAxis->setRange(xMin, xMax);
    ui->voltagePlot->graph(0)->setData(xAxisData, voltDisplayData);
    ui->voltagePlot->replot();
    
    // 更新声音图表
    ui->soundPlot->xAxis->setRange(xMin, xMax);
    ui->soundPlot->graph(0)->setData(xAxisData, soundDisplayData);
    ui->soundPlot->replot();
    
}

// 更新光谱播放
void Menu::updateSpecPlayback()
{
    if (!isPlayingSpec || specFileList.isEmpty() || currentDefectPath.isEmpty()) {
        return;
    }
    
    // 检查索引是否超出范围，如果是则循环播放
    if (currentSpecFileIndex >= specFileList.size()) {
        currentSpecFileIndex = 0;
    }
    
    // 构建当前文件路径
    QString currentSpecFile = currentDefectPath + "/光谱/" + specFileList[currentSpecFileIndex];
    
    // 清空之前的光谱数据
    allSpecDataX.clear();
    allSpecDataY.clear();
    
    // 读取当前光谱文件
    QFile specFile(currentSpecFile);
    if (!specFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开光谱文件:" << currentSpecFile;
        currentSpecFileIndex++;
        return;
    }
    
    QTextStream specIn(&specFile);
    while (!specIn.atEnd()) {
        QString line = specIn.readLine();
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.size() >= 2) {
            bool okX, okY;
            double xValue = parts[0].toDouble(&okX);
            double yValue = parts[1].toDouble(&okY);
            if (okX && okY) {
                allSpecDataX.append(xValue);
                allSpecDataY.append(yValue);
            }
        }
    }
    specFile.close();
    
    if (!allSpecDataX.isEmpty() && !allSpecDataY.isEmpty()) {
        // 更新光谱图表数据
        ui->specPlot->graph(0)->data()->clear();
        ui->specPlot->graph(0)->setData(allSpecDataX, allSpecDataY);
        
        // 动态计算光谱x轴和y轴范围，每次都重新计算以确保显示所有数据
        double specXMin = *std::min_element(allSpecDataX.begin(), allSpecDataX.end());
        double specXMax = *std::max_element(allSpecDataX.begin(), allSpecDataX.end());
        double specYMin = *std::min_element(allSpecDataY.begin(), allSpecDataY.end());
        double specYMax = *std::max_element(allSpecDataY.begin(), allSpecDataY.end());
        double specYRange = specYMax - specYMin;
        
        // X轴范围保持固定（使用第一次的范围）
        static bool xRangeSet = false;
        static double fixedXMin, fixedXMax;
        if (!xRangeSet) {
            fixedXMin = specXMin;
            fixedXMax = specXMax;
            xRangeSet = true;
        }
        ui->specPlot->xAxis->setRange(fixedXMin, fixedXMax);
        
        // Y轴范围每次动态调整，并增加更大的边距以便看到所有数据点
        double yPadding = qMax(specYRange * 0.5, 50.0); // 增加50%的边距，最少50个单位
        double yMin = specYMin - yPadding;
        double yMax = specYMax + yPadding;
        ui->specPlot->yAxis->setRange(yMin, yMax);
        
        // 重新绘制光谱图表
        ui->specPlot->replot();
        
        qDebug() << "播放光谱文件:" << specFileList[currentSpecFileIndex] 
                 << "数据点数:" << allSpecDataX.size()
                 << "Y范围:" << QString("(%1, %2)").arg(yMin, 0, 'f', 1).arg(yMax, 0, 'f', 1)
                 << "数据Y范围:" << QString("(%1, %2)").arg(specYMin, 0, 'f', 1).arg(specYMax, 0, 'f', 1);
    }
    
    // 移动到下一个文件
    currentSpecFileIndex++;
}

// 更新熔池图像播放
void Menu::updateImagePlayback()
{
    if (!isPlayingImages || imageFileList.isEmpty() || currentDefectPath.isEmpty()) {
        return;
    }
    
    // 简化：直接使用成员变量，不需要静态变量
    static int frameSkip = 2;
    
    // 检查是否播放完所有图像（使用跳帧策略）
    int actualImageIndex = currentImageIndex * frameSkip;
    if (actualImageIndex >= imageFileList.size()) {
        imagePlayTimer->stop();
        isPlayingImages = false;
        qDebug() << "熔池图像播放完成";
        return;
    }
    
    // 构建当前图像文件路径
    QString currentImageFile = currentDefectPath + "/熔池图像/" + imageFileList[actualImageIndex];
    
    // 加载图像
    QPixmap pixmap(currentImageFile);
    if (!pixmap.isNull()) {
        // 更新图像显示
        imageItem->setPixmap(pixmap);
        
        // 获取控件尺寸
        QSize viewSize = ui->gvMain->size();
        
        // 缩放图像以占满整个控件（可能会拉伸）
        QPixmap scaledPixmap = pixmap.scaled(viewSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        imageItem->setPixmap(scaledPixmap);
        
        // 重置缩放
        imageItem->setScale(1.0);
        
        // 设置场景矩形为控件大小
        imageScene->setSceneRect(0, 0, viewSize.width(), viewSize.height());
        
        // 居中显示图像项
        imageItem->setPos(0, 0);
        
        // 让视图完全适应场景
        ui->gvMain->fitInView(imageScene->sceneRect(), Qt::IgnoreAspectRatio);
        
        qDebug() << "显示熔池图像:" << imageFileList[actualImageIndex] 
                 << "(" << (currentImageIndex + 1) << "/" << (imageFileList.size()/frameSkip) << ")"
                 << "跳帧:" << frameSkip;
    } else {
        qDebug() << "无法加载图像:" << currentImageFile;
    }
    
    // 移动到下一张图像
    currentImageIndex++;
}

// 更新红外图像播放
void Menu::updateIrImagePlayback()
{
    if (!isPlayingIrImages || irImageFileList.isEmpty() || currentDefectPath.isEmpty()) {
        return;
    }
    
    // 检查是否播放完所有红外图像
    if (currentIrImageIndex >= irImageFileList.size()) {
        irImagePlayTimer->stop();
        isPlayingIrImages = false;
        qDebug() << "红外图像播放完成";
        return;
    }
    
    // 构建当前红外图像文件路径
    QString currentIrImageFile = currentDefectPath + "/红外图像/" + irImageFileList[currentIrImageIndex];
    
    // 加载红外图像
    QPixmap irPixmap(currentIrImageFile);
    if (!irPixmap.isNull()) {
        // 获取lblImage控件尺寸
        QSize labelSize = ui->lblImage->size();
        
        // 缩放图像以占满整个lblImage控件
        QPixmap scaledIrPixmap = irPixmap.scaled(labelSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        
        // 设置到lblImage控件
        ui->lblImage->setPixmap(scaledIrPixmap);
        
        qDebug() << "显示红外图像:" << irImageFileList[currentIrImageIndex] 
                 << "(" << (currentIrImageIndex + 1) << "/" << irImageFileList.size() << ")";
    } else {
        qDebug() << "无法加载红外图像:" << currentIrImageFile;
    }
    
    // 移动到下一张红外图像
    currentIrImageIndex++;
}

// void Menu::updateAcquiCardPlot() { ... }
// void Menu::updateLineEditContent() { ... }
