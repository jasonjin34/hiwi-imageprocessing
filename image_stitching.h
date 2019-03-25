#ifndef IMAGE_STITCHING_H
#define IMAGE_STITCHING_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>
#include "imgfunctions.h"

namespace Ui {
class Image_stitching;
}

class Image_stitching : public QDialog
{
    Q_OBJECT
    QGraphicsScene stitching_result,inputView1,inputView2,inputView3;

public:
    explicit Image_stitching(QWidget *parent = nullptr);
    ~Image_stitching();

private slots:

    void on_transf_button_clicked();

    void on_closeGUI_clicked();

    void on_searchImage_stitching_clicked();

    void on_saveImage_button_clicked();

private:
    Ui::Image_stitching *ui;
    QStringList files;
    QImage* result;
    cv::Mat result_save;
};

#endif // IMAGE_STITCHING_H
