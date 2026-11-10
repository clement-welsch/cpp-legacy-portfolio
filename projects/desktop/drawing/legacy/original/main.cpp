#include "Application.h"
#include <qpixmap.h>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <MainWindow.h>

int main (int argc, char* argv[])
{
    Application app(argc, argv);

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&translator);

    MainWindow main(&app);
    main.showMaximized();
    //main.show();

    return app.exec() ;
}
