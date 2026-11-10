#include <QtGui>
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <DLGNewDrawing.h>
#include <QMdiSubWindow>
#define BGColor_WHITE 0;
#define BGColor_TRANSPARENT 1;


MainWindow::MainWindow(Application* _app,QWidget *parent) :
    m_app(_app),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(ui->mdiArea);
    m_vectorDrawing = new QVector <Drawing*>();
    m_untiltedCpt = 1;
    InitConnection();
}

MainWindow::~MainWindow()
{
    m_vectorDrawing->clear();
    delete m_vectorDrawing;
    m_vectorDrawing = NULL;
    delete ui;
    ui = NULL;
}
void MainWindow::InitConnection()
{
    InitConnectFichier();
}
void MainWindow::InitConnectFichier()
{
    //-------------
    //---Fichier---
    //-------------
    //New Drawing
    connect(ui->actionNouveau, SIGNAL(triggered()), this, SLOT(NewDrawing()));
    //Close Drawing
    connect(ui->actionFermer, SIGNAL(triggered()), this, SLOT(CloseDrawing()));
    //Save Drawing
    connect(ui->actionEnregistrer, SIGNAL(triggered()), this, SLOT(SaveDrawing()));
    //Save Drawing
    connect(ui->actionEnregistrer_sous, SIGNAL(triggered()), this, SLOT(SaveDrawingAs()));
    //Open Drawing
    connect(ui->actionOuvrir, SIGNAL(triggered()), this, SLOT(OpenDrawing()));
    //Quit
    connect(ui->actionQuitter, SIGNAL(triggered()), this, SLOT(Quit()));
}
void MainWindow::InitConnectAide()
{
    //-------------
    //-----Aide----
    //-------------
    //About Qt
    QObject::connect(ui->actionA_propos_de_Qt, SIGNAL(triggered()), m_app, SLOT(aboutQt()));
}
void MainWindow::NewDrawing()
{
    /*if(m_drawing)
        return;*/

    DLGNewDrawing* newDLGDrawing = new DLGNewDrawing (this);
    QString nameInit = "Sans Titre-"+QString::number(m_untiltedCpt);
    newDLGDrawing->setName(nameInit);

    if(newDLGDrawing->exec()==QDialog::Accepted)
    {
        //Drawing* newDrawing;

        QString name = newDLGDrawing->getName();
        int width = newDLGDrawing->getWidth();
        int height = newDLGDrawing->getHeight();

        if (name == nameInit)
            m_untiltedCpt++;

        m_vectorDrawing->push_back(new Drawing(name, width ,height, 0, ui->mdiArea));
        //delete newDrawing;
    }


    delete newDLGDrawing;
}
void MainWindow::SaveDrawing()
{
    /*ui->mdiArea->currentSubWindow()->findChild<SubWindow*>()->SaveMe();
    QMdiSubWindow* sub = static_cast<QMdiSubWindow*> (ui->mdiArea->currentSubWindow());

    QMdiSubWindow* sub2 = static_cast<QMdiSubWindow*> (ui->mdiArea->currentSubWindow()->findChild<SubWindow*>());*/
}
void MainWindow::SaveDrawingAs()
{
    /*if (m_drawing)
        m_drawing->SaveMeAs();*/
}
void MainWindow::OpenDrawing()
{
    /*if (m_drawing)
        m_drawing->OpenMe();*/
}
void MainWindow::CloseDrawing()
{
    /*if (!m_drawing)
        return;
    delete m_drawing;
    m_drawing = NULL;*/
}
void MainWindow::Quit()
{
    m_app->exit(0);
}
