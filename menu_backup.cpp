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

    // 电压图表 - 已注释
    // ui->voltagePlot->setMinimumSize(300, 200);
    // ui->voltagePlot->setBackground(Qt::transparent); // 将图表背景设置为透明
    // ui->voltagePlot->setStyleSheet("background: transparent;"); // 将图表背景设置为透明
    // ui->voltagePlot->addGraph(); // 添加一个图表
    // ui->voltagePlot->graph(0)->setPen(QPen(Qt::red, 1)); // 设置图表颜色为红色
    // ui->voltagePlot->yAxis->setRange(0,150);
//  ui->voltagePlot->xAxis->setLabel("时间");
//  ui->voltagePlot->yAxis->setLabel(电压(V)");
    // ui->voltagePlot->xAxis->grid()->setVisible(true); // 开启网格线显示
    // ui->voltagePlot->yAxis->grid()->setVisible(true); // 开启网格线显示
    // ui->voltagePlot->xAxis2->setVisible(true);  // 显示上轴
    // ui->voltagePlot->yAxis2->setVisible(true);  // 显示右轴
    // ui->voltagePlot->xAxis->setBasePen(QPen(Qt::gray));  // 设置 x 轴颜色
    // ui->voltagePlot->yAxis->setBasePen(QPen(Qt::gray));  // 设置 y 轴颜色
    // ui->voltagePlot->xAxis2->setBasePen(QPen(Qt::gray)); // 设置上轴颜色
    // ui->voltagePlot->yAxis2->setBasePen(QPen(Qt::gray)); // 设置右轴颜色
    // ui->voltagePlot->xAxis->grid()->setPen(QPen(QColor(195, 195, 195), 0.5, Qt::SolidLine));
    // ui->voltagePlot->yAxis->grid()->setPen(QPen(QColor(195, 195, 195), 0.5, Qt::SolidLine));
    // ui->voltagePlot->xAxis->setTickPen(Qt::NoPen);  // 设置主刻度线为透明
    // ui->voltagePlot->yAxis->setTickPen(Qt::NoPen);  // 设置主刻度线为透明
    // ui->voltagePlot->xAxis->setSubTickPen(Qt::NoPen);  // 设置次刻度线为透明
    // ui->voltagePlot->yAxis->setSubTickPen(Qt::NoPen);  // 设置次刻度线为透明
    // ui->voltagePlot->xAxis2->setTickLabels(false);  // 隐藏上轴的刻度标签
    // ui->voltagePlot->yAxis2->setTickLabels(false);  // 隐藏右轴的刻度标签
    // ui->voltagePlot->xAxis2->setTicks(false);  // 隐藏上轴的刻度线
    // ui->voltagePlot->yAxis2->setTicks(false);  // 隐藏右轴的刻度线
    // ui->voltagePlot->hide();  // 初始隐藏图表


    // 声音图表 - 已注释
    // ui->soundPlot->setMinimumSize(300, 200);
    // ui->soundPlot->setBackground(Qt::transparent); // 将图表背景设置为透明
    // ui->soundPlot->setStyleSheet("background: transparent;"); // 将图表背景设置为透明
    // ui->soundPlot->addGraph(); // 添加一个图表
    // ui->soundPlot->graph(0)->setPen(QPen(Qt::red, 1)); // 设置图表颜色为红色
    // ui->soundPlot->yAxis->setRange(-5,5);
//  ui->soundPlot->xAxis->setLabel("时间");
//  ui->soundPlot->yAxis->setLabel(声音(V)");
    // ui->soundPlot->xAxis->grid()->setVisible(true); // 开启网格线显示
    // ui->soundPlot->yAxis->grid()->setVisible(true); // 开启网格线显示
    // ui->soundPlot->xAxis2->setVisible(true);  // 显示上轴
    // ui->soundPlot->yAxis2->setVisible(true);  // 显示右轴
    // ui->soundPlot->xAxis->setBasePen(QPen(Qt::gray));  // 设置 x 轴颜色
    // ui->soundPlot->yAxis->setBasePen(QPen(Qt::gray));  // 设置 y 轴颜色
    // ui->soundPlot->xAxis2->setBasePen(QPen(Qt::gray)); // 设置上轴颜色
    // ui->soundPlot->yAxis2->setBasePen(QPen(Qt::gray)); // 设置右轴颜色
    // ui->soundPlot->xAxis->grid()->setPen(QPen(QColor(195, 195, 195), 0.5, Qt::SolidLine));
    // ui->soundPlot->yAxis->grid()->setPen(QPen(QColor(195, 195, 195), 0.5, Qt::SolidLine));
    // ui->soundPlot->xAxis->setTickPen(Qt::NoPen);  // 设置主刻度线为透明
    // ui->soundPlot->yAxis->setTickPen(Qt::NoPen);  // 设置主刻度线为透明
    // ui->soundPlot->xAxis->setSubTickPen(Qt::NoPen);  // 设置次刻度线为透明
    // ui->soundPlot->yAxis->setSubTickPen(Qt::NoPen);  // 设置次刻度线为透明
    // ui->soundPlot->xAxis2->setTickLabels(false);  // 隐藏上轴的刻度标签
    // ui->soundPlot->yAxis2->setTickLabels(false);  // 隐藏右轴的刻度标签
    // ui->soundPlot->xAxis2->setTicks(false);  // 隐藏上轴的刻度线
    // ui->soundPlot->yAxis2->setTicks(false);  // 隐藏右轴的刻度线
    // ui->soundPlot->hide();  // 初始隐藏图表


    // 光谱图表 - 已注释
    // ui->specPlot->setMinimumSize(300, 200);
    // ui->specPlot->setBackground(Qt::transparent); // 将图表背景设置为透明
    // ui->specPlot->setStyleSheet("background: transparent;"); // 将图表背景设置为透明
    // ui->specPlot->xAxis->setRange(100.0, 1000.0);
    // ui->specPlot->yAxis->setRange(0.0, 1000.0);
    // ui->specPlot->xAxis->grid()->setVisible(true); // 开启网格线显示
    // ui->specPlot->yAxis->grid()->setVisible(true); // 开启网格线显示
    // ui->specPlot->xAxis2->setVisible(true);  // 显示上轴
    // ui->specPlot->yAxis2->setVisible(true);  // 显示右轴
    // ui->specPlot->xAxis->setBasePen(QPen(Qt::gray));  // 设置 x 轴颜色
    // ui->specPlot->yAxis->setBasePen(QPen(Qt::gray));  // 设置 y 轴颜色
    // ui->specPlot->xAxis2->setBasePen(QPen(Qt::gray)); // 设置上轴颜色
    // ui->specPlot->yAxis2->setBasePen(QPen(Qt::gray)); // 设置右轴颜色
    // ui->specPlot->xAxis->grid()->setPen(QPen(QColor(195, 195, 195), 0.5, Qt::SolidLine));
    // ui->specPlot->yAxis->grid()->setPen(QPen(QColor(195, 195, 195), 0.5, Qt::SolidLine));
    // ui->specPlot->xAxis->setTickPen(Qt::NoPen);  // 设置主刻度线为透明
    // ui->specPlot->yAxis->setTickPen(Qt::NoPen);  // 设置主刻度线为透明
    // ui->specPlot->xAxis->setSubTickPen(Qt::NoPen);  // 设置次刻度线为透明
    // ui->specPlot->yAxis->setSubTickPen(Qt::NoPen);  // 设置次刻度线为透明
    // ui->specPlot->xAxis2->setTickLabels(false);  // 隐藏上轴的刻度标签
    // ui->specPlot->yAxis2->setTickLabels(false);  // 隐藏右轴的刻度标签
    // ui->specPlot->xAxis2->setTicks(false);  // 隐藏上轴的刻度线
    // ui->specPlot->yAxis2->setTicks(false);  // 隐藏右轴的刻度线
    // ui->specPlot->hide();  // 初始隐藏图表
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

    // lastSaveTimer.start();
    // 把最新的红外图像传递出来 - 已注释
    // connect(f, &irbgrab::newInfraImage, this, [this](const QImage &pix){
    //     if (ifcapture)
    //     {
    //         // 如果距离上次时间小于100ms，则直接返回
    //         // if (lastSaveTimer.elapsed() < 100) return;
    //         // lastSaveTimer.restart(); // 更新保存时间

    //         ui->lblImage->setPixmap(QPixmap::fromImage(pix));

    //         newfileName = "./data/" + timeStamp + "/InfraredImage_" +
    //                            QDateTime::fromMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch()).toString("yyyyMMdd_HHmmss_zzz") + ".png";

    //         pix.save(newfileName);
    //         // qDebug()<<"红外保存成功="<<fileName;

    //         // 如果已经在处理上一帧，就直接返回，不新开线程
    //         // if (infraredProcessing.load()) return;
    //         //  infraredProcessing.store(true);

//         QtConcurrent::run([fileName](){


//             //提取红外特征
//             py::object result = infrared_features->attr("extract_infrared_features")(fileName.toStdString());
//             // qDebug()<<"红外特征提取成功";
//             if (!result.is_none()) {
//                 irbgrab::infrared_features = result.cast<std::vector<double>>();
//             }
//             // 打印特征
//             qDebug() << "Extracted infrared_Features:";
//             for (size_t i = 0; i < irbgrab::infrared_features.size(); i++) {
//                 qDebug() << "f" << i << ":" << irbgrab::infrared_features[i];

//             }
//              infraredProcessing.store(false); // 处理完成
//         });

    //      }
    //  }, Qt::QueuedConnection);

}

// void Menu::updateLineEditContent() // 更新左侧文本框内容 - 已注释
// {
//     QTimer::singleShot(0, this, [this](){
//     ui->elecShow->setText(AI_StreamingBufferedAi::elecValue); // 电流
//     ui->voltShow->setText(AI_StreamingBufferedAi::voltValue); // 电压
//     ui->soundShow->setText(AI_StreamingBufferedAi::soundValue); // 声音
//     ui->flowShow->setText(Modbus::flowValue); // 气流
//     ui->wireSpeedShow->setText(WireSpeed::speedValue); // 送丝速度
//     });

//     // 运行时间
//     double nowTime = static_cast<double>(QDateTime::currentDateTime().toMSecsSinceEpoch());
//     timeValue = QString::number((nowTime-startTime)/1000.0, 'f', 3) + "s";
// }

// connect(m_thread, SIGNAL(captured(QImage)), this, SLOT(Image_process(QImage)), Qt::BlockingQueuedConnection);
// void Menu::captureFrame() // 保存熔池图像的函数 将m_scene图像显示及保存 - 已注释
{
    //m_scene熔池图像
    //ui显示熔池图像
    if (this->w->status) ui->gvMain->setScene(w->m_scene); // 如果焊接视觉模块已经打开，则在主界面显示熔池图像
    ui->gvMain->fitInView(ui->gvMain->sceneRect(), Qt::KeepAspectRatio); // 使m_scene适应menu中gvMain的尺寸

    //保存熔池图像，这边图像大小跟ui控件尺寸相同，不是原始图像大小
   // count++;
   // if (weldPoolMap) delete weldPoolMap;// 如果指针已指向一熔池图，则delete指针
   weldPoolMap = new QPixmap(w->ui->gvMain->size()); // 新建一与gvMain同尺寸的QPixmap


   // if (weldPoolMapPainter) delete weldPoolMapPainter;
   weldPoolMapPainter = new QPainter(weldPoolMap); //将QPainter与要绘图的对象关联起来

   w->m_scene->render(weldPoolMapPainter); // 使用render()方法将视图内容绘制到QPixmap上

   QString fileName = "./data/" + timeStamp + "/WeldPoolImage_" +
                      QDateTime::fromMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch()).toString("yyyyMMdd_HHmmss_zzz") + ".png";
   weldPoolMap->save(fileName); // 保存截图到文件


    //提取熔池特征
   // py::module_ image_features = py::module_::import("image_featuresC");
   py::object result = image_features->attr("extract_image_features")(fileName.toStdString());


       MecaVision::image_features = result.cast<std::vector<double>>();
       // 打印特征
       qDebug() << "Extracted image_Features:";
       for (size_t i = 0; i < MecaVision::image_features.size(); i++) {
           qDebug() << "f" << i << ":" << MecaVision::image_features[i];

   }


        //71维度 1-6电流，7-12电压，13-54声音，55气流，56送丝速度57-59红外，60-71熔池
        combined.clear();
        if (AI_StreamingBufferedAi::I_features.empty()) return;
        combined.insert(combined.end(), AI_StreamingBufferedAi::I_features.begin(), AI_StreamingBufferedAi::I_features.end());
        combined.insert(combined.end(), AI_StreamingBufferedAi::U_features.begin(), AI_StreamingBufferedAi::U_features.end());
        combined.insert(combined.end(), AI_StreamingBufferedAi::S_features.begin(), AI_StreamingBufferedAi::S_features.end());
        combined.push_back(Modbus::flowValue.toDouble());
//        combined.push_back(WireSpeed::speedValue.toDouble());

        // qDebug() << "flow="<<Modbus::flowValue.toDouble();
        // qDebug() << "speed="<<WireSpeed::speedValue.toDouble();
        //把红外特征去除
        // if (irbgrab::infrared_features.empty()) return;
       // combined.insert(combined.end(), irbgrab::infrared_features.begin(), irbgrab::infrared_features.end());
          if (MecaVision::image_features.empty()) return;
         combined.insert(combined.end(), MecaVision::image_features.begin(), MecaVision::image_features.end());

        // qDebug() << "I.size="<<AI_StreamingBufferedAi::I_features.size();
        // qDebug() << "U.size="<<AI_StreamingBufferedAi::U_features.size();
        // qDebug() << "S.size="<<AI_StreamingBufferedAi::S_features.size();
        // qDebug() << "infrared.size="<<Infrared::infrared_features.size();
        // qDebug() << "MecaVision.size="<<MecaVision::image_features.size();

        qDebug() << "predict.size="<<combined.size();
        for (size_t i = 0; i < combined.size(); i++) {
            qDebug() << "f" << i << ":" << combined[i];

        }

        //预测缺陷  0-4对应["正常", "未焊透", "焊偏", "气孔", "焊漏"]
        // 转为 numpy array
        py::array_t<double> arr(combined.size(), combined.data());

        result = pre->attr("predict_from_array")(arr);
        std::vector<int> value = result.cast<std::vector<int>>();


        qDebug() << "predict=" << value[0];

        QString text = (value[0] >= 0 && value[0] < weldingStatusText.size()) ? weldingStatusText[value[0]] : "未知状态";

        QFile file(pre_filePath); // 相对路径
        if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) { // txt中不清除，继续添加数据
            qWarning("Cannot open file for writing");
        }
        QTextStream textStream(&file);
        textStream << QDateTime::fromMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch()).toString("yyyyMMdd_HHmmss_zzz")  << " " << text << "\n";
        file.close();

        ui->weldingstatus->setText(text);

}

void Menu::captureInfrared()  //提取红外图像的特征
{

    //提取红外特征
    py::object result = infrared_features->attr("extract_infrared_features")(newfileName.toStdString());


    irbgrab::infrared_features = result.cast<std::vector<double>>();
    // 打印特征
    qDebug() << "Extracted infrared_Features:";
    for (size_t i = 0; i < irbgrab::infrared_features.size(); i++) {
        qDebug() << "f" << i << ":" << irbgrab::infrared_features[i];

    }

}



// 在打开软件时即打开子模块并隐藏子模块界面
void Menu::openSubModule()
{
    /// 熔池图 ///
   MecaVision* visionWindow = MecaVision::getInstance();
   w = visionWindow;

   /// 红外 ///
   irbgrab* infraWindow = irbgrab::getInstance();

   f = infraWindow;
   f->ConnectDeviceOneStep(0);

    /// 气体 ///
    //this->m.show();
    this->m.pToMenu = this; // 将本例指针赋给气体流量类的成员

    /// 采集卡 ///
    openConfiguredialog();

}

Menu::~Menu()
{
    delete ui;
}


// 打开configuredialog.ui，对应电弧、声音传感界面
void Menu::openConfiguredialog()
{
    int resultDialog = this->dialog.exec();
    if (resultDialog == QDialog::Rejected)
    {
        qApp->exit(0);
        return;
    }
    else if (resultDialog == QDialog::Accepted)
    {
        this->buffer.SetConfigureParameter(this->dialog.GetConfigureParameter());
        this->buffer.SetConfigureDialog(&this->dialog);
        this->buffer.Initialize();
        this->buffer.show();
    }
}

void Menu::on_craft_data_clicked()
{
    // // 1. (可选但推荐) 显式启用高DPI缩放
    // QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // // 2. (最关键的一行) 设置缩放因子取整策略为"PassThrough"（直接使用，不取整）
    // QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    // QApplication app(argc, argv);
    mysql *craft = new mysql(this);  // 每次点击新建一个窗口
    craft->show();
    qDebug()<<"显示数据库";

}

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

// 按钮：开始采集
void Menu::on_startButton_clicked()
{
    //this->q.show(); // TODO：q对应了draw.h和draw.cpp文件，是杨雨欣测试用的，可以删掉
    startTime = static_cast<double>(QDateTime::currentDateTime().toMSecsSinceEpoch()); // 获取当前毫秒时间戳

    QTimer::singleShot(0, this, [this](){
    // 按键使能设置
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->saveButton->setEnabled(true);

    ui->currentPlot->show();
    ui->voltagePlot->show();
    ui->soundPlot->show();
    ui->specPlot->show();
    ui->currentground->hide();
    ui->voltageground->hide();
    ui->soundground->hide();
    ui->specground->hide();
    });

    int type1 = ui->comboBox_3->currentIndex(); // 0=打底焊,1=填充焊
    int type2 = ui->comboBox_4->currentIndex(); // 0=横焊,1=立焊,2=角焊
    
    // 组合类型编码规则
    weldType = type1 * 3 + type2; // 0-5

    // 创建保存本次采集数据的文件夹
    QDir dir;
    timeStamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    dir.mkpath("./data/"+timeStamp);

    // 子线程停止采集信号
    //stopCollection = false;

    // 等效于电弧传感模块中点击"Start"；焊接气流模块/送丝速度模块中点击"开始采集"
    buffer.ButtonStartClicked();
    m.on_startflowButton_clicked();
    wireSpeed.on_btn_open_clicked();

    // 重置绘图和文件读取状态
    count = 0;
    lastReadFilePos = 0;
    QTimer::singleShot(0, this, [this](){
    ui->currentPlot->graph(0)->data()->clear();
    ui->voltagePlot->graph(0)->data()->clear();
    ui->soundPlot->graph(0)->data()->clear();
    ui->currentPlot->replot();
    ui->voltagePlot->replot();
    ui->soundPlot->replot();
    });

    // UI更新和定时保存
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(updateAcquiCardPlot())); // 数据采集卡绘图
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(updateLineEditContent())); // 左侧文本框更新
    dataTimer->start(1000); // 更新频率，单位毫秒（更新频率可以调整）

    /// 熔池图 ///
    /// \brief connect
    ///在主线程中被阻塞，采集帧率受限，必须改成异步
   connect(captureTimer, SIGNAL(timeout()), this, SLOT(captureFrame())); // 熔池保存图像
   captureTimer->start(100);

   /// 红外 ///
   ifcapture = true;
   connect(infraTimer, SIGNAL(timeout()), this, SLOT(captureInfrared()));// 红外保存图像
   // f->Device_Grab(0,1);
   infraTimer->start(100);



    /// 采集卡 ///
    ifDrawAcquiCard = true;
    //updatePlot->start(); // 启动线程
    //threads.append(updatePlot); // 将线程添加到列表中
    //dataTimer->start(1000); // 更新频率，单位毫秒（更新频率可以调整）

    /// 气流 ///
    m.ifMenuView = true;
    if (!ifPause) // 如果目前不处于暂停状态
    {
        //ui->airstreamPlot->graph(0)->data()->clear();
        m.startTime = static_cast<double>(QDateTime::currentDateTime().toMSecsSinceEpoch());
        //m.ui->airstreamPlot->graph(0)->data()->clear();
    }
    else ifPause = false;


    // 保存数据
//    TaskThread* taskThread = new TaskThread(this, TaskThread::CollectWeldMap, timeStamp, this);
//    connect(taskThread, &TaskThread::finished, taskThread, &QObject::deleteLater);
//    taskThread->start(); // 启动线程
//    threads.append(taskThread); // 将线程添加到列表中

    /// 红外测温 ///
//    if (!infTemp.cameraCheck()) qDebug() << "没有检测到红外测温设备"; // 检测并连接设备
//    if (!infTemp.startCollection()) qDebug() << "无法开始采集"; // 开始采集

//    /// 光谱 ///
//    spec.setParams(); // 设置参数
//    HWND hwnd = (HWND)this->winId();
//    spec.startMeasure(hwnd); // 开始测量
//    connect(specTimer, SIGNAL(timeout()), this, SLOT(getSpecData()));
//    specTimer->start(1000); // 更新频率，单位毫秒（更新频率可以调整）
    /// 光谱 ///
    this->spec->pToMenu = this; // 将本例指针赋给光谱类的成员
    spec->startMeasure(); // 开始测量

    /// 机器人速度 ///
    robotSpeed.readTimer->start(1000); // 每1000ms刷新一次
    robotSpeed.positionTimer.start();  // 启动计时器
    robotSpeed.on_Connect_buttom_clicked();
}

// 按钮：停止采集
void Menu::on_stopButton_clicked()
{
    QTimer::singleShot(0, this, [this](){
    // 按键使能设置
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    });

    // 停止QTimer
    dataTimer->stop();
    captureTimer->stop();
    ifcapture = false;
    infraTimer->stop();

    // 等效于电弧传感模块中点击"Stop"；焊接气流模块/送丝速度模块中点击"停止采集"
    buffer.ButtonStopClicked();
    m.on_stopflowButton_clicked();
    wireSpeed.on_btn_close_clicked();

    // 子线程停止采集信号
    //stopCollection = true;

//    // 停止线程
//    for (TaskThread* thread : threads)
//    {
//        thread->stop(); // 停止所有线程
//        thread->wait(); // 等待每个线程结束
//    }
//    threads.clear(); // 清空线程列表

    /// 采集卡 ///
    //buffer.ifMenuView = false;
    ifDrawAcquiCard = false;
    count = 0;

    /// 气流 ///
    ifPause = false;
    m.ifMenuView = false;
    m.startTime = static_cast<double>(QDateTime::currentDateTime().toMSecsSinceEpoch());

    /// 熔池图 ///
   if (ui->gvMain->scene()) ui->gvMain->setScene(nullptr); // 如果视图中已有场景，则清除场景
    // if (ui->gvMain_2->scene()) ui->gvMain_2->setScene(nullptr);
    /// 红外测温 ///
//    if (!infTemp.stopCollection()) qDebug() << "无法停止采集"; // 停止采集

    /// 光谱 ///
    spec->stopMeasure(); // 停止测量

    /// 机器人速度 ///
    robotSpeed.on_Disconnect_buttom_clicked();
}

// 按钮：保存数据
void Menu::on_saveButton_clicked()
{
//    // 创建保存本次采集数据的文件夹
//    QDir dir;
//    QString timeStamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
//    dir.mkpath("./data/"+timeStamp);

    /// 采集卡 ///
//    QFile::copy(AI_StreamingBufferedAi::filePath, "./data/"+timeStamp+"/DAQ.txt");
//    QFile file(AI_StreamingBufferedAi::filePath); file.remove();
    QFile::copy(AI_StreamingBufferedAi::elecFilePath, "./data/"+timeStamp+"/elec_Data.txt");
    QFile file(AI_StreamingBufferedAi::elecFilePath); file.remove();
    QFile::copy(AI_StreamingBufferedAi::voltFilePath, "./data/"+timeStamp+"/volt_Data.txt");
    QFile file1(AI_StreamingBufferedAi::voltFilePath); file1.remove();
    QFile::copy(AI_StreamingBufferedAi::soundFilePath, "./data/"+timeStamp+"/sound_Data.txt");
    QFile file2(AI_StreamingBufferedAi::soundFilePath); file2.remove();


    /// 红外测温 ///
//    if (!infTemp.saveIRDX()) qDebug() << "无法保存为IRDX文件"; // 保存为IRDX文件
//    if (!infTemp.savePixelData()) qDebug() << "无法保存为txt文件"; // 保存为txt文件
//    QFile::copy("./"+QString(InfraredTemp::filePath_irdx), "./data/"+timeStamp+"/InfTemp_IRDX.irdx");
//    QFile::copy("./"+InfraredTemp::filePath_txt, "./data/"+timeStamp+"/InfTemp_txt.txt");
//    QFile file1("./"+QString(InfraredTemp::filePath_irdx)); file1.remove();
//    QFile file2("./"+InfraredTemp::filePath_txt); file2.remove();

    /// 光谱 ///
    //spec.getData(); // 获取测量数据
    for (int i = 0; i < spec->m_NrOfDevices; i++)
    {
        QFile::copy("./"+spectrum::filePath+QString::number(i)+".txt", "./data/"+timeStamp+"/Spec_"+QString::number(i)+".txt");
        QFile file4("./"+spectrum::filePath+QString::number(i)+".txt"); file4.remove();
    }


    /// 气体 ///
    QFile::copy("./"+Modbus::filePath, "./data/"+timeStamp+"/Flow.txt");
    QFile file5("./"+QString(Modbus::filePath)); file5.remove();

    /// 送丝速度 ///
    QFile::copy("./"+WireSpeed::filePath, "./data/"+timeStamp+"/WireSpeed.txt");
    QFile file6("./"+QString(WireSpeed::filePath)); file6.remove();

    /// 机器人速度 ///
    QFile::copy("./"+RobotSpeed::filePath, "./data/"+timeStamp+"/RobotSpeed.txt");
    QFile file7("./"+QString(RobotSpeed::filePath)); file7.remove();

    /// 预测结果 ///
    QFile::copy("./"+pre_filePath, "./data/"+timeStamp+"/predict.txt");
    QFile file8("./"+QString(pre_filePath)); file8.remove();
}

// 光谱-test
//void Menu::getSpecData()
//{
////    spec.setParams(); // 设置参数
////    HWND hwnd = (HWND)this->winId();
////    spec.startMeasure(hwnd); // 开始测量
////    Sleep(1000);
////    spec.stopMeasure(); // 停止测量
//    //spec.getData(); // 获取测量数据
//}


//每次只更新500个数据点
void Menu::updateAcquiCardPlot()
{
    QTimer::singleShot(0, this, [this](){
    if (!AI_StreamingBufferedAi::elecList.isEmpty())
        AI_StreamingBufferedAi::elecValue = QString::number(AI_StreamingBufferedAi::elecList.last(), 'f', 3) + "A";
    else
        AI_StreamingBufferedAi::elecValue = "0.000A";
    if (!AI_StreamingBufferedAi::voltList.isEmpty())
        AI_StreamingBufferedAi::voltValue = QString::number(AI_StreamingBufferedAi::voltList.last(), 'f', 3) + "V";
    else
        AI_StreamingBufferedAi::voltValue = "0.000V";
    if (!AI_StreamingBufferedAi::soundList.isEmpty())
        AI_StreamingBufferedAi::soundValue = QString::number(AI_StreamingBufferedAi::soundList.last(), 'f', 3) + "V";
    else
        AI_StreamingBufferedAi::soundValue = "0.000";

    QVector<double> xData(500);
    for (int j = 0; j < 500; ++j) {
        xData[j] = count+j;
    }
    count=count+500;

    // 将 QList 转换为 QVector（QCustomPlot 接受 QVector）
    QVector<double> elecData = QVector<double>::fromList(AI_StreamingBufferedAi::elecList);
    QVector<double> voltData = QVector<double>::fromList(AI_StreamingBufferedAi::voltList);
    QVector<double> soundData = QVector<double>::fromList(AI_StreamingBufferedAi::soundList);

    // 将 QList 转 std::vector<double>
    std::vector<double> elecVec(AI_StreamingBufferedAi::elecList.begin(), AI_StreamingBufferedAi::elecList.end());
    std::vector<double> voltVec(AI_StreamingBufferedAi::voltList.begin(), AI_StreamingBufferedAi::voltList.end());
    std::vector<double> soundVec(AI_StreamingBufferedAi::soundList.begin(), AI_StreamingBufferedAi::soundList.end());


    if (!elecVec.empty()) {
    py::object result = I_features->attr("extract_current_time_features")(elecVec);
    AI_StreamingBufferedAi::I_features = result.cast<std::vector<double>>();
    // 打印特征
    qDebug() << "Extracted I_Features:";
    for (size_t i = 0; i < AI_StreamingBufferedAi::I_features.size(); i++) {
         qDebug() << "f" << i << ":" << AI_StreamingBufferedAi::I_features[i];
    }
    }

    if (!voltVec.empty()) {
        py::object result = U_features->attr("extract_voltage_time_features")(voltVec);
        AI_StreamingBufferedAi::U_features = result.cast<std::vector<double>>();
        // 打印特征
        qDebug() << "Extracted U_Features:";
        for (size_t i = 0; i < AI_StreamingBufferedAi::U_features.size(); i++) {
            qDebug() << "f" << i << ":" << AI_StreamingBufferedAi::U_features[i];
        }
    }

    if (!soundVec.empty()) {
        py::object result = S_features->attr("extract_all_sound_features")(soundVec);
        AI_StreamingBufferedAi::S_features = result.cast<std::vector<double>>();
        // 打印特征
        qDebug() << "Extracted S_Features:";
        for (size_t i = 0; i < AI_StreamingBufferedAi::S_features.size(); i++) {
            qDebug() << "f" << i << ":" << AI_StreamingBufferedAi::S_features[i];
        }
    }

    // 设置数据
    ui->currentPlot->graph(0)->setData(xData, elecData);
    ui->currentPlot->xAxis->setRange(qMax(0, count-500),  count);
    ui->currentPlot->replot(QCustomPlot::rpQueuedReplot);

    ui->voltagePlot->graph(0)->setData(xData, voltData);
//    ui->voltagePlot->xAxis->setRange(qMax(0, (count + 2) / 3-500),  (count + 2) / 3);
    ui->voltagePlot->xAxis->setRange(qMax(0, count-500),  count);
    ui->voltagePlot->replot(QCustomPlot::rpQueuedReplot);

    ui->soundPlot->graph(0)->setData(xData, soundData);
    ui->soundPlot->xAxis->setRange(qMax(0, count-500),  count);
    ui->soundPlot->replot(QCustomPlot::rpQueuedReplot);
    });
}
