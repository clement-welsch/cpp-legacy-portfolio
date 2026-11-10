#ifndef DRAWING_H
#define DRAWING_H

#include <QDockWidget>
#include <QWidget>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>

class MainWindow;

class Drawing : public QWidget
{
    Q_OBJECT
public:
    explicit Drawing(QString _name, int _width, int _height, int  _bgColor, QWidget *parent = 0);
    ~Drawing();
    void SaveMe();
    void SaveMeAs();
    void OpenMe();
private:
    QDockWidget* m_dockWidget;
    QVBoxLayout *m_layout;
    QGraphicsScene* m_drawScene;
    QGraphicsView* m_drawView;
    QPixmap* m_pixmap;

    QWidget *m_widget;
    QMdiSubWindow *m_subWindow;

    bool m_firstSave;
    int m_width;
    int m_height;
    QString m_savePath;
    QString m_name;
    int m_bgColor;
    void DrawScene(QPaintDevice * dev);
    void DrawMe ();
    
signals:
    
public slots:
    
};

#endif // DRAWING_H
