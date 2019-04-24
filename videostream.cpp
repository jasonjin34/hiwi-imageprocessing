#include "videostream.h"
#include "ui_videostream.h"
#include <QDebug>

videoStream::videoStream(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::videoStream)
{
    ui->setupUi(this);

    ui->StreamView->setScene(new QGraphicsScene(this));
    ui->StreamView->scene()->addItem(&pixmap);
    ui->StreamView->setMinimumWidth(600);
    ui->StreamView->setMinimumHeight(450);
    videostop = false;
    videopause = false;
    this->createVideo = false;
    this->fps = 10;
}

videoStream::~videoStream()
{
    delete ui;
}


void videoStream::on_Speed_valueChanged(int value)
{
    this->fps = value;
}

void videoStream::closeEvent(QCloseEvent *bar)
{
    this->videostop = true;
    video.release();
}

void videoStream::on_actionScreenshot_triggered()
{
    this->videopause = true;
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Screenshot Image"), "C:/", tr("Image (*.png *.xmp *jpg)"));
    cv::imwrite(filename.toStdString(), this->screenshotimage);
    this->videopause = false;
}

void videoStream::on_actionRecord_triggered()
{
    this->videopause = true;
    this->videoname = QFileDialog::getSaveFileName(this, tr("Recording Image"), "C:/", tr("Video (*.avi)"));
    this->videopause = false;
}

void videoStream::on_actionPlay_triggered()
{
    this->videostop = false;
    this->videopause = false;
    av_register_all();
    avformat_network_init();
    int ret;

    //set the url link
    /**
    int option = 1; // 1, local video, 2, rtsp protocal
    const char* url;
    if(option == 1) url = "C:/HIWI/VideoStreaming/Videostreaming/testvideo.mp4";
    else url = "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov";
    */
    QString qurl = ui->inputUrl->text();
     qurl.replace("\\", "/");
    std::string temp = qurl.toStdString();
    const char* url = temp.c_str();

    // open input file context
    AVFormatContext* inctx = nullptr;
    ret = avformat_open_input(&inctx, url, nullptr, nullptr);
    if (ret < 0) {
        std::cerr << "fail to avforamt_open_input(\"" << url << "\"): ret=" << ret;
        QMessageBox::critical(this, "Error", "Invalid url, please try the valid one");
        return;
    }
    // retrive input stream information
    ret = avformat_find_stream_info(inctx, nullptr);
    if (ret < 0) {
        std::cerr << "fail to avformat_find_stream_info: ret=" << ret;
         qDebug() << "failed";
    }

    // find primary video stream
    AVCodec* vcodec = nullptr;
    ret = av_find_best_stream(inctx, AVMEDIA_TYPE_VIDEO, -1, -1, &vcodec, 0);
    if (ret < 0) {
        std::cerr << "fail to av_find_best_stream: ret=" << ret;
         qDebug() << "failed";
    }
    const int vstrm_idx = ret;
    AVStream* vstrm = inctx->streams[vstrm_idx];

    // open video decoder context
    ret = avcodec_open2(vstrm->codec, vcodec, nullptr);
    if (ret < 0) {
        std::cerr << "fail to avcodec_open2: ret=" << ret;
       qDebug() << "failed";
    }

    // print input video stream informataion
    std::cout
        << "infile: " << url << "\n"
        << "format: " << inctx->iformat->name << "\n"
        << "vcodec: " << vcodec->name << "\n"
        << "size:   " << vstrm->codec->width << 'x' << vstrm->codec->height << "\n"
        << "frame:  " << vstrm->nb_frames << "\n"
        << std::flush;

    // initialize sample scaler
    const int dst_width = vstrm->codec->width;
    const int dst_height = vstrm->codec->height;
    const AVPixelFormat dst_pix_fmt = AV_PIX_FMT_BGR24;
    SwsContext* swsctx = sws_getCachedContext(
        nullptr, vstrm->codec->width, vstrm->codec->height, vstrm->codec->pix_fmt,
        dst_width, dst_height, dst_pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
    if (!swsctx) {
        std::cerr << "fail to sws_getCachedContext";
        qDebug() << "failed";
    }

    std::cout << "output: " << dst_width << 'x' << dst_height << ',' << av_get_pix_fmt_name(dst_pix_fmt) << std::endl;

    // allocate frame buffer for output
    AVFrame* frame = av_frame_alloc();
    std::vector<uint8_t> framebuf(avpicture_get_size(dst_pix_fmt, dst_width, dst_height));
    avpicture_fill(reinterpret_cast<AVPicture*>(frame), framebuf.data(), dst_pix_fmt, dst_width, dst_height);

    // decoding loop
    AVFrame* decframe = av_frame_alloc();
    unsigned nb_frames = 0;
    bool end_of_stream = false;
    int got_pic = 0;
    AVPacket pkt;

    bool resize_status = false;
    do {
        if (!end_of_stream) {
            // read packet from input file
            ret = av_read_frame(inctx, &pkt);
            if (ret < 0 && ret != AVERROR_EOF) {
                std::cerr << "fail to av_read_frame: ret=" << ret;
                 qDebug() << "failed";
                 break;
            }
            if (ret == 0 && pkt.stream_index != vstrm_idx)
                goto next_packet;
            end_of_stream = (ret == AVERROR_EOF);
        }

        if (end_of_stream) {
            // null packet for bumping process
            av_init_packet(&pkt);
            pkt.data = nullptr;
            pkt.size = 0;
        }

        // decode video frame
        avcodec_decode_video2(vstrm->codec, decframe, &got_pic, &pkt);
        if (!got_pic)
            goto next_packet;
        // convert frame to OpenCV matrix
        sws_scale(swsctx, decframe->data, decframe->linesize, 0, decframe->height, frame->data, frame->linesize);
        {
            QImage dest = QImage(framebuf.data(), dst_width, dst_height,QImage::Format_RGB888);
            screenshotimage = cv::Mat(dst_height, dst_width, CV_8UC3, framebuf.data(), frame->linesize[0]);

            //using opencv to record the video
            if(videoname.toStdString().length() > 2)
            {
                if(!this->createVideo)
                {
                  video = cv::VideoWriter(videoname.toStdString(),cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(dst_width, dst_height));
                  this->createVideo = true;
                }
            }

            if(videoname.toStdString().length() > 2)
            {
                 video.write(screenshotimage);
                 qDebug() << "Recording .....";
            }

            ui->StreamView->setAlignment(Qt::AlignCenter);
            if(resize_status == false)
            {
               if(dst_width > 800 || dst_height > 600)
               {
                   int width = ui->StreamView->geometry().width();
                   int height = ui->StreamView->geometry().height();
                   dest = dest.scaled(width, height, Qt::KeepAspectRatio);
               }
            }
            pixmap.setPixmap(QPixmap::fromImage(dest));
            ui->StreamView->setAlignment(Qt::AlignCenter);
        }
        qDebug() << "current fps: " << this->fps;
        cv::waitKey(this->fps);
        //stop the stream
        if(videostop)
        {
            break;
        }

        if(this->videopause)
        {
            while(1)
            {
                cv::waitKey(200);
                if(!this->videopause) break;
            }
        }
        qDebug() << "frame number: " << nb_frames;
        ++nb_frames;
    next_packet:
        av_free_packet(&pkt);

    } while (!end_of_stream || got_pic);

    std::cout << nb_frames << " frames decoded" << std::endl;

    av_frame_free(&decframe);
    av_frame_free(&frame);
    avcodec_close(vstrm->codec);
    avformat_close_input(&inctx);

    qDebug() << "pass";
}

void videoStream::on_actionPause_triggered()
{
    if(this->videopause == false)
    {
        this->videopause = true;
        ui->actionPause->setText("Continue");
    }
    else
    {
        this->videopause = false;
        ui->actionPause->setText("Pause");
    }

}

void videoStream::on_actionStop_Video_triggered()
{
    this->videostop = true;
    video.release();
}

void videoStream::on_actionFaster_triggered()
{
    this->fps = 0;
}

void videoStream::on_actionFaster_2_triggered()
{
    this->fps = 5;
}

void videoStream::on_actionNormal_Speed_triggered()
{
    this->fps = 10;
}

void videoStream::on_actionSlower_fine_triggered()
{
    this->fps = 25;
}

void videoStream::on_actionSlower_triggered()
{
    this->fps = 50;
}
