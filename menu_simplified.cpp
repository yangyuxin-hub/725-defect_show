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
// #include <QElapsedTimer>
#include <algorithm>

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
{
    ui->setupUi(this);

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
    ui->currentPlot->hide();  // 初始隐藏图表

    // 其他图表初始化都已注释
    // 电压图表、声音图表、光谱图表等都已注释

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
    delete ui;
}

// 只保留缺陷数据显示功能
void Menu::on_defect_data_clicked()
{
    // 读取elec_Data.txt文件并绘制到currentPlot
    QFile file("elec_Data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开elec_Data.txt文件";
        return;
    }

    QTextStream in(&file);
    QVector<double> elecData;
    QVector<double> xData;
    int index = 0;

    // 读取电流数据
    while (!in.atEnd()) {
        QString line = in.readLine();
        bool ok;
        double value = line.toDouble(&ok);
        if (ok) {
            elecData.append(value);
            xData.append(index++);
        }
    }
    file.close();

    if (elecData.isEmpty()) {
        qDebug() << "elec_Data.txt文件为空或没有有效数据";
        return;
    }

    // 显示currentPlot图表
    ui->currentPlot->show();
    ui->currentground->hide();

    // 清除之前的数据
    ui->currentPlot->graph(0)->data()->clear();

    // 设置新数据
    ui->currentPlot->graph(0)->setData(xData, elecData);

    // 设置坐标轴范围
    ui->currentPlot->xAxis->setRange(0, elecData.size());
    
    // 计算y轴范围，使用数据的最小值和最大值
    double minVal = *std::min_element(elecData.begin(), elecData.end());
    double maxVal = *std::max_element(elecData.begin(), elecData.end());
    double range = maxVal - minVal;
    ui->currentPlot->yAxis->setRange(minVal - range * 0.1, maxVal + range * 0.1);

    // 重新绘制图表
    ui->currentPlot->replot();

    qDebug() << "成功绘制了" << elecData.size() << "个电流数据点到currentPlot";
}

// 所有其他函数都已注释掉
// void Menu::on_startButton_clicked() { ... }
// void Menu::on_stopButton_clicked() { ... }
// void Menu::on_saveButton_clicked() { ... }
// void Menu::on_craft_data_clicked() { ... }
// void Menu::captureFrame() { ... }
// void Menu::captureInfrared() { ... }
// void Menu::openSubModule() { ... }
// void Menu::openConfiguredialog() { ... }
// void Menu::updateAcquiCardPlot() { ... }
// void Menu::updateLineEditContent() { ... }
