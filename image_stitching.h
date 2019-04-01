#ifndef IMAGE_STITCHING_H
#define IMAGE_STITCHING_H
#include <QLabel>
#include <QWidget>
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>
#include "imgfunctions.h"
#include <QMovie>
#include "message.h"

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
    void transfer_function();
    void transfer_processing_function();

signals:
    void notifyMessageProcessing(const message&);

private slots:
    void onMessageProcessing(message);

    void on_transf_button_clicked();

    void on_closeGUI_clicked();

    void on_searchImage_stitching_clicked();

    void on_saveImage_button_clicked();

    void on_transf_processing();

private:
    Ui::Image_stitching *ui;
    QStringList files;
    QImage* result;
    cv::Mat result_save;
    QLabel* gif_anim;
    QMovie* movie;

};

#endif // IMAGE_STITCHING_H
