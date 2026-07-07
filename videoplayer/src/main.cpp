#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "videoplayermainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) 
    {
        const QString baseName = "videoplayer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) 
        {
            a.installTranslator(&translator);
            break;
        }
    }

    VideoPlayerMainWindow w;
    w.show();
    return QApplication::exec();
}
