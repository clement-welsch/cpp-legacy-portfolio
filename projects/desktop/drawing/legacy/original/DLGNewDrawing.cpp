#include "DLGNewDrawing.h"
#include "ui_DLGNewDrawing.h"

DLGNewDrawing::DLGNewDrawing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLGNewDrawing)
{
    ui->setupUi(this);
    //ui->lineEdit_Width->setValidator( new QValidator(0, 4000, this) );
    //ui->lineEdit_Height->setValidator( new QValidator(0, 4000, this) );
}

DLGNewDrawing::~DLGNewDrawing()
{
    delete ui;
}
void DLGNewDrawing::setName(QString _name)
{
    ui->lineEdit_Name->setText(_name);
}
QString DLGNewDrawing::getName()
{
    return ui->lineEdit_Name->text();
}
int DLGNewDrawing::getWidth()
{
    bool ok;
    return ui->lineEdit_Width->text().toInt(&ok);
}
int DLGNewDrawing::getHeight()
{
    bool ok;
    return ui->lineEdit_Height->text().toInt(&ok);
}
