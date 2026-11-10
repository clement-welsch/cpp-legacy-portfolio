#ifndef DLGNEWDRAWING_H
#define DLGNEWDRAWING_H

#include <QDialog>

namespace Ui {
class DLGNewDrawing;
}

class DLGNewDrawing : public QDialog
{
    Q_OBJECT
    
public:
    explicit DLGNewDrawing(QWidget *parent = 0);
    ~DLGNewDrawing();
    void setName(QString _name);
    QString getName();
    int getWidth();
    int getHeight();
private:
    Ui::DLGNewDrawing *ui;
};

#endif // DLGNEWDRAWING_H
