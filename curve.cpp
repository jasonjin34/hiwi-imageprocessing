#include "curve.h"
#include "ui_curve.h"

Curve::Curve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Curve)
{
    ui->setupUi(this);
}

Curve::~Curve()
{
    delete ui;
}
