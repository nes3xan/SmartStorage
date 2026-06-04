#include <QApplication>
#include "appstyle.h"
#include "splashscreen.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    AppStyle::setupApplication(app);


    auto *mainWindow = new MainWindow();


    auto *splash = new SplashScreen();

    QObject::connect(splash, &SplashScreen::finished, [mainWindow, splash]() {
        mainWindow->show();
        splash->hide();
        splash->deleteLater();
    });

    splash->show();
    splash->startProgress();

    return app.exec();
}