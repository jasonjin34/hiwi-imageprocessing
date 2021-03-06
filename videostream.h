#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include <QMainWindow>
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

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}

namespace Ui {
class videoStream;
}

class videoStream : public QMainWindow
{
    Q_OBJECT
    cv::Mat image;
    QGraphicsPixmapItem pixmap;
    QImage dest;

public:
    explicit videoStream(QWidget *parent = nullptr);
    ~videoStream();

private slots:   
    void on_actionScreenshot_triggered();

    void on_actionRecord_triggered();

    void on_actionPlay_triggered();

    void on_actionPause_triggered();

    void on_actionStop_Video_triggered();

    void on_actionFaster_triggered();

    void on_actionFaster_2_triggered();

    void on_actionNormal_Speed_triggered();

    void on_actionSlower_fine_triggered();

    void on_actionSlower_triggered();

    void on_actionLoad_Local_File_triggered();

    void on_actionClose_triggered();

private:
    void closeEvent(QCloseEvent *bar);

    Ui::videoStream *ui;
    bool videopause;
    bool videostop;
    bool createVideo;
    int fps;
    cv::VideoWriter video;
    cv::Mat screenshotimage;
    QString videoname;
};

#endif // VIDEOSTREAM_H
