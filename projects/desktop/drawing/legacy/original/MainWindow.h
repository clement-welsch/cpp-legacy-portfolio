#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Application.h"
#include "Drawing.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(Application* _app, QWidget *parent = 0);
    ~MainWindow();
    Application* m_app;
    
private:
    int m_untiltedCpt;
    Ui::MainWindow *ui;
    QVector <Drawing*> *m_vectorDrawing;
    void InitConnection();
    void InitConnectFichier();
    void InitConnectAide();

private slots:
    void NewDrawing();
    void SaveDrawing();
    void SaveDrawingAs();
    void OpenDrawing();
    void CloseDrawing();
    void Quit();
};

#endif // MAINWINDOW_H
