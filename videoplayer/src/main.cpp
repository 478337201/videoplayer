#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "videoplayermainwindow.h"
#include "videoplayercontroller.h"

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) 
    {
        const QString baseName = "videoplayer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) 
        {
            application.installTranslator(&translator);
            break;
        }
    }
    VideoPlayerMainWindow mainWindow(nullptr, new VideoPlayerController(&application));
    mainWindow.show();
    return QApplication::exec();
}
