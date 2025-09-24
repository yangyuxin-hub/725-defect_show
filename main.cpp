#include <QApplication>
#include "menu.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序信息
    app.setApplicationName("焊接多信息在线监测软件");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Welding Monitor");

    // 创建主窗口
    Menu window;
    window.show();

    return app.exec();
}
