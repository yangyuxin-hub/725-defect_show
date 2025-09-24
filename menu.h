#ifndef MENU_H
#define MENU_H


#include <QMainWindow>
// #include "configuredialog.h"
// #include "MecaVision.h"
// #include "frm_irbgrab_v4.h"
// #include "Modbus.h"
// #include "ui_Modbus.h" // 为了确保能够在主控界面的类中调用子界面的控件
// #include "infraredtemp.h" // 红外测温
// #include "spectrum.h" // 光谱

// 在引用采集卡的h文件前先对Menu类进行前向声明，以确保两个h文件中的类可以互相引用且不发生编译报错
class Menu;

// #include "streamingbufferedai.h"
// #include "taskthread.h"
// #include "wirespeed.h" // 送丝速度

#include <QPixmap> // 图片
#include <QPainter> // 绘图
#include <QDateTime> // 获取日期时间
#include <QTimer> // 定时器
#include <QVector> // 向量容器
#include <string>
#include <QList>
#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

// 前向声明
class QGraphicsScene;
class QGraphicsPixmapItem;

// #include "application.h"

// #include "robotspeed.h" // 机器人速度

namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

private slots:
//    void on_configButton_clicked();

    // void on_startButton_clicked();

    // void on_stopButton_clicked();

    // void on_saveButton_clicked();

    // void on_craft_data_clicked();

    void on_defect_data_clicked();
    void updatePlotData(); // 定时更新绘图数据的槽函数
    void updateSpecPlayback(); // 更新光谱播放的槽函数
    void updateImagePlayback(); // 更新熔池图像播放的槽函数
    void updateIrImagePlayback(); // 更新红外图像播放的槽函数
    void showDefectSelectionDialog(); // 显示缺陷类型选择对话框
    void loadDefectData(const QString& defectType); // 加载指定缺陷类型的数据
public:
    Ui::Menu *ui;

    // void openSubModule(); // 在打开软件时即打开子模块并隐藏子模块界面
    // void openConfiguredialog(); // 打开configuredialog.ui，对应电弧、声音传感界面
    // bool ConnectInfraDevice(int connectionIndex); //连接红外设备
    // bool Device_Grab(int aStreamIndex, bool aGrabbing);


//private:
public:
    //Ui::Menu *ui; // 已改为公有成员
    // AI_StreamingBufferedAi buffer;
    // ConfigureDialog dialog;
    // //MecaVision w; // 已改为指针
    // MecaVision *w;
    // irbgrab *f;
    // Modbus m; // 改为指针得到话会crash，原因暂未知
    // //Modbus *m;

    // bool stopCollection = true; // 停止采集信号
    // QList<TaskThread*> threads; // 储存所有子线程的列表
    // TaskThread* updatePlot; // 数据采集卡绘图的线程
    // QString timeStamp = "null"; // 时间戳

    // /// 气体 ///
    // bool ifPause = false; // 用于判别目前是否处于暂停状态

    // /// 熔池图 ///
    // QTimer *captureTimer = new QTimer(this); // 熔池保存图像的QTimer

   // QPixmap *weldPoolMap = nullptr; // 熔池瞬时图像
   // QGraphicsPixmapItem* pixmapItem; // 图形项的容器，在QGraphicsView中显示QPixmap所必须的包装
   // QPainter *weldPoolMapPainter = nullptr; // 绘制熔池瞬时图像的对象
   // QGraphicsScene* singleShot; // 用于显示瞬时图像的scene

   // QTimer *infraTimer = new QTimer(this); // 红外保存图像的QTimer
   // bool ifcapture = false;

    // /// 采集卡 ///
    // bool ifDrawAcquiCard = false; // 用于确认是否采集数据绘制采集卡图像的成员
    // QTimer* dataTimer = new QTimer(this); // 计时器
    // int count = 0; // x轴坐标
    // double scaledData = 0; // y轴数值
    // std::vector<double> combined;

//    /// 红外测温 ///
//    InfraredTemp infTemp;
//    bool getInfTempData(); // 进行红外测温并获取数据-test

    // /// 光谱 ///
    // application *app;
    // spectrum *spec;
    // QTimer *specTimer = new QTimer(this); // 光谱保存的QTimer

    // /// 送丝速度 ///
    // WireSpeed wireSpeed;

    // /// 机器人速度 ///
    // RobotSpeed robotSpeed;

    // 实时数据显示相关成员变量
    QVector<double> allElecData;        // 存储所有电流数据
    QVector<double> allVoltData;        // 存储所有电压数据
    QVector<double> allSoundData;       // 存储所有声音数据
    QVector<double> allSpecDataX;       // 存储所有光谱X轴数据(波长)
    QVector<double> allSpecDataY;       // 存储所有光谱Y轴数据(强度)
    QVector<double> elecDisplayData;    // 电流显示数据窗口(2000个点)
    QVector<double> voltDisplayData;    // 电压显示数据窗口
    QVector<double> soundDisplayData;   // 声音显示数据窗口
    QVector<double> xAxisData;          // x轴数据
    QTimer* plotUpdateTimer;            // 50ms定时器
    QTimer* specPlayTimer;              // 光谱播放定时器(4Hz)
    QTimer* imagePlayTimer;             // 熔池图像播放定时器
    QTimer* irImagePlayTimer;           // 红外图像播放定时器
    int currentDataIndex;               // 当前数据读取索引
    int currentSpecFileIndex;           // 当前光谱文件索引
    int currentImageIndex;              // 当前熔池图像索引
    int currentIrImageIndex;            // 当前红外图像索引
    QStringList specFileList;           // 光谱文件列表
    QStringList imageFileList;          // 熔池图像文件列表
    QStringList irImageFileList;        // 红外图像文件列表
    bool isPlayingSpec;                 // 是否正在播放光谱
    bool isPlayingImages;               // 是否正在播放熔池图像
    bool isPlayingIrImages;             // 是否正在播放红外图像
    QGraphicsScene* imageScene;         // 熔池图像场景
    QGraphicsPixmapItem* imageItem;     // 熔池图像项
    QString currentDefectPath;          // 当前选择的缺陷类型路径
    QStringList defectTypes;            // 缺陷类型列表
    int windowSize;                     // 窗口大小(2000个点)
    int samplingRate;                   // 采样率(40960Hz)
    int updatePointsPerSecond;          // 每秒处理的点数(40960个，与采样率一致)
    int pointsPerUpdate;                // 每次更新的点数(2048个点)
    bool isPlotting;                    // 是否正在绘图

    // static QString timeValue; // 实时运行时间
    // double startTime; // 开始采集时间
    // std::streampos lastReadFilePos = 0; // 上次文件读取位置


     // int weldType = 0; // 0-5 对应不同焊接类型组合


public slots:
    // void updateAcquiCardPlot(); // 更新采集卡图像的函数
    // void captureFrame(); // 采集熔池图像的函数
    // void captureInfrared(); // 采集红外图像的函数
    // void updateLineEditContent(); // 更新左侧文本框内容
    //void getSpecData(); // 获取光谱数据-测试
};

#endif // MENU_H
