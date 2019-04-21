#ifndef VIDEOSTREAMING_H
#define VIDEOSTREAMING_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/stitching.hpp>

namespace Ui {
class Videostreaming;
}

class Videostreaming : public QWidget
{
    Q_OBJECT
    cv::Mat image;
    QGraphicsPixmapItem pixmap;
    QImage dest;

public:
    explicit Videostreaming(QWidget *parent = nullptr);
    ~Videostreaming();

private slots:
    void on_pushButton_clicked();

    void on_stopButton_clicked();

    void on_pauseButton_clicked();

    void on_Speed_valueChanged(int value);

private:
    void closeEvent(QCloseEvent *bar);

    Ui::Videostreaming *ui;
    bool videopause;
    bool videostop;
    int fps;
};

#endif // VIDEOSTREAMING_H
