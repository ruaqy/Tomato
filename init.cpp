#include <QLocale>
#include <QTranslator>
#include <QApplication>

#include "main.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString &locale : uiLanguages) {
        const QString baseName = "i18n/Tomato_" + QLocale(locale).name() + ".qm";

        if (translator.load(baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    Main w;
    w.show();
    return a.exec();
}
