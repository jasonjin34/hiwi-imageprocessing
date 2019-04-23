#ifndef VIDEOSTREAMING_H
#define VIDEOSTREAMING_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>

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

    void on_screenshotButton_clicked();

    void on_RecordingButton_clicked();

private:
    void closeEvent(QCloseEvent *bar);

    Ui::Videostreaming *ui;
    bool videopause;
    bool videostop;
    bool createVideo;
    int fps;
    cv::VideoWriter video;
    cv::Mat screenshotimage;
    QString videoname;
};

#endif // VIDEOSTREAMING_H
