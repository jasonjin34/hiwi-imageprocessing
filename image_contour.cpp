#include "image_contour.h"
#include "ui_image_contour.h"

image_contour::image_contour(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::image_contour)
{
    ui->setupUi(this);
}

image_contour::~image_contour()
{
    delete ui;
}
