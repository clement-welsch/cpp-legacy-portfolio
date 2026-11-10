#include "Application.h"

Application::Application(int & argc, char ** argv): QApplication (argc, argv)
{
    //QObject::connect(this, SIGNAL(QCoreApplication::aboutToQuit ()), this, SLOT(ClearApplication()));
}
void Application::ClearApplication()
{
}
