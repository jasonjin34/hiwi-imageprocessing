#include "window_histmatching.h"
#include "ui_window_histmatching.h"

window_histmatching::window_histmatching(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::window_histmatching)
{
    ui->setupUi(this);
}

window_histmatching::~window_histmatching()
{
    delete ui;
}


void window_histmatching::on_maxIntensity_valueChanged(int arg1)
{
    int message = arg1;
    emit notifyMessageSent(message);
}
