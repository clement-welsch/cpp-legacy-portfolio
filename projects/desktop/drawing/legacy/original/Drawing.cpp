#include <QApplication>
#include <QFileDialog>
#include <QtGui>
#include "Drawing.h"
#include <qpainter.h>
#include <MainWindow.h>
#include <QMdiSubWindow>

#define BGColor_WHITE 0;
#define BGColor_TRANSPARENT 1;

Drawing::Drawing(QString _name, int _width, int _height, int _bgColor, QWidget *parent) :
    m_name(_name),
    m_width(_width),
    m_height(_height),
    m_bgColor (_bgColor),
    QWidget(parent)
{
    m_firstSave = false;
    m_savePath = "";
    /*m_drawScene = new QGraphicsScene;
    m_drawScene->setSceneRect(0,0,m_width,m_height);
    m_drawView = new QGraphicsView(m_drawScene, this);
    m_drawView->resize(m_width,m_height);

    m_pixmap = new QPixmap("W.JPG");
    m_drawScene->addPixmap(*m_pixmap);

    //DrawMe();
    if (m_bgColor == 1)//transparent
    {
        m_drawScene->setBackgroundBrush(Qt::transparent);
        m_pixmap->fill(Qt::transparent);
    }
    //else

    QMdiArea* mdiArea = static_cast<QMdiArea*>(parent);

    m_dockWidget = new QDockWidget( mdiArea );
    mdiArea->addSubWindow( m_dockWidget );
    m_dockWidget->setMinimumHeight(m_width+10);
    m_dockWidget->setMinimumHeight(m_height+10);
    m_dockWidget->layout()->addWidget(m_drawView);
    m_dockWidget->setWindowTitle(m_name);
    m_dockWidget->show();*/
    //m_dockWidget->showMaximized();

    QMdiArea* mdiArea = static_cast<QMdiArea*>(parent);

    QWidget *widget = new QWidget;
    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsView *vue = new QGraphicsView(scene);
    vue->setBackgroundBrush(Qt::Dense7Pattern);
    vue->setSceneRect(QRectF(0, 0, 200, 200));
    vue->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(vue, Qt::AlignCenter);
    widget->setLayout(layout);
    QMdiSubWindow *fen = mdiArea->addSubWindow(widget);
    fen->setWindowTitle(m_name);
    fen->showMaximized();
    fen->show();
}
Drawing::~Drawing()
{
    if(m_drawView)
    {
        delete m_drawView;
        m_drawView = NULL;
    }
    if(m_drawScene)
    {
        delete m_drawScene;
        m_drawScene = NULL;
    }
    if(m_layout)
    {
        delete m_layout;
        m_layout = NULL;
    }
    if(m_dockWidget)
    {
        delete m_dockWidget;
        m_dockWidget = NULL;
    }
    delete m_widget;
    delete m_subWindow;
}
void Drawing::DrawMe()
{
    //Image
    QPixmap monPixmap("W.JPG");
    m_drawScene->addPixmap(monPixmap);
    m_drawScene->setSceneRect(0, 0, monPixmap.width(), monPixmap.height());
    //m_drawView->setSceneRect(0, 0, monPixmap.width(), monPixmap.height());
    //Point
    QPolygon point;
    point << QPoint(100, 100);
    m_drawScene->addPolygon(point);
    //Lignes
    QPen pen(Qt::blue, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QLine ligne(50, 50, 200, 200);
    m_drawScene->addLine(ligne, pen);

    QPolygon polygon;
    pen.setColor(Qt::red);
    polygon << QPoint(100, 100) << QPoint(100, 200);
    m_drawScene->addPolygon(polygon,pen);
    //rectangle
    m_drawScene->addRect(10, 10, 100, 50);
    //polygon
    QPolygon polygon2;
    pen.setColor(Qt::white);
    polygon2 << QPoint(100, 100) << QPoint(100, 200) << QPoint(200, 200) << QPoint(200, 100);
    m_drawScene->addPolygon(polygon2,pen);
}
void Drawing::SaveMe()
{
    if(!m_firstSave || m_savePath =="")
    {
        m_savePath = QFileDialog::getSaveFileName(0, "Enregistrer","untitled.png","Images (*.png *.xpm *.jpg)");
        m_firstSave = true;
    }

    QPixmap pixmap(this->m_drawView->width(), this->m_drawView->height());

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    this->m_drawView->render(&painter);

    pixmap.save(m_savePath);
    painter.end();
}
void Drawing::SaveMeAs()
{
    m_savePath = QFileDialog::getSaveFileName(0, "Enregistrer Sous","untitled.png","Images (*.png *.xpm *.jpg)");

    QPixmap pixmap(this->m_drawView->width(), this->m_drawView->height());

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    this->m_drawView->render(&painter);

    pixmap.save(m_savePath);
    painter.end();
}
void Drawing::OpenMe ()
{
    QString openPath = QFileDialog::getOpenFileName(this, "Localiser le fichier ","D:/MonFirefox/","*.png");
    QPixmap monPixmap(openPath);
    this->m_drawScene->addPixmap(monPixmap);
}
