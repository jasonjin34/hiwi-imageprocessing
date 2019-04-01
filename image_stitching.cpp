#include "image_stitching.h"
#include "ui_image_stitching.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <QFileDialog>
#include <QDebug>
#include <QTime>
#include <QString>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QMessageBox>
#include <QThread>
#include <QFuture>
#include <QTime>
#include <QtConcurrent>
#include "thread_loading.h"
#include "thread_stitching_transfer.h"

message status_message;

Image_stitching::Image_stitching(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Image_stitching)
{
    ui->setupUi(this);
    ui->stitching_result->setScene(&stitching_result);
    ui->inputView1->setScene(&inputView1);
    ui->inputView2->setScene(&inputView2);
    ui->inputView3->setScene(&inputView3);

    connect(ui->closeGUI, SIGNAL(clicked()), this, SLOT(on_closeGUI_clicked()));
    connect(this,&Image_stitching::notifyMessageProcessing,this,&Image_stitching::onMessageProcessing);
    //connect(ui->transf_button,SIGNAL(clicked()),this,SLOT(on_transf_button_clicked()));
}

Image_stitching::~Image_stitching()
{
    delete ui;
}

void Image_stitching::transfer_function()
{
     std::vector<cv::Mat> imageVector_pano;
     cv::Mat result;
     iaw::loadImagevector(files,imageVector_pano);
     iaw::imageStitching(imageVector_pano,result);
     this->result_save = result; //use to save the original stitched image

     //set image
     cv::Mat temp;
     cv::cvtColor(result,temp,CV_BGR2RGB);
     QImage dest(temp.data,temp.cols,temp.rows,static_cast<int>(temp.step),QImage::Format_RGB888);
     QImage destscaled = dest.scaled(1000,1000,Qt::KeepAspectRatio,Qt::SmoothTransformation);
     stitching_result.addPixmap(QPixmap::fromImage(destscaled));
     this->result = &destscaled;
     ui->status_label->setText("finish");
}

void Image_stitching::transfer_processing_function()
{
    on_transf_processing();
}


//TO DO Multithread
void Image_stitching::onMessageProcessing(message message_processing)
{
    on_transf_processing();
    if(message_processing.getProcessing() == true)
    {
       ui->status_label->setText("Image Processing ... ");
       transfer_function();

       /**
       Thread_loading Tfirst(this,"loading");
       Thread_loading Tsecond(this,"transfer");
       Tfirst.start();
       Tsecond.start();
       Tfirst.wait();
       Tsecond.wait();
       */
    }
}


void Image_stitching::on_transf_processing()
{
    bool b_test = false;
    if(b_test == true)
    {
        this->gif_anim = new QLabel();
        this->movie = new QMovie("C:/HIWI/hiwi_test_final/loading_image/loading_small.gif");
        this->gif_anim->setMovie(movie);
        this->movie->start();
        stitching_result.addWidget(gif_anim);
        ui->status_label->setText("proccesing ... ");
    }
    else {
        QImage loading_image;
        loading_image.load("C:/HIWI/hiwi_test_final/loading_image/loadingpng.png");
        stitching_result.addPixmap(QPixmap::fromImage(loading_image));
        inputView1.addPixmap(QPixmap::fromImage(loading_image));
        inputView2.addPixmap(QPixmap::fromImage(loading_image));
        inputView3.addPixmap(QPixmap::fromImage(loading_image));
        qDebug() << "loading";
        ui->status_label->setText("proccesing ... ");
        ui->status_label->setText("proccesing ... ");
    }
}

void Image_stitching::on_transf_button_clicked()
{
    transfer_function();
}

void Image_stitching::on_closeGUI_clicked()
{
    this->close();
}

void Image_stitching::on_searchImage_stitching_clicked()
{
    stitching_result.clear();
    inputView1.clear();
    inputView2.clear();
    inputView3.clear();

    on_transf_processing();
    QStringList tempFiles;
    if(tempFiles.isEmpty()){
       tempFiles = QFileDialog::getOpenFileNames(
                    this,
                    "Open Image",
                    "C:/HIWI/images/stitching",
                    "Images (*.png *.jpg *.bmp)"
                    );
       this->files = tempFiles;
    }
    else qDebug()<<"loading image error";

    //load at least 2 images
    if(tempFiles.length() >= 3)
    {
        QString output;
        for(int i = 0; i < tempFiles.size(); ++i){
            output = output + tempFiles[i];
        }
        ui->imageFile_stitching->setText(output);
        std::vector<cv::Mat> imageVector_pano;
        iaw::loadImagevector(tempFiles,imageVector_pano);
        QVector<QImage> qImageVector;
        for(int i = 0; i < static_cast<int>(imageVector_pano.size());++i)
        {
            cv::Mat temp;
            cv::cvtColor(imageVector_pano[static_cast<unsigned long long>(i)],temp,CV_BGR2RGB);
            QImage dest(temp.data,temp.cols,temp.rows,static_cast<int>(temp.step),QImage::Format_RGB888);
            QImage destscaled = dest.scaled(341,176,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            qImageVector.append(destscaled);
        }
        inputView1.addPixmap(QPixmap::fromImage(qImageVector[0]));
        inputView2.addPixmap(QPixmap::fromImage(qImageVector[1]));
        inputView3.addPixmap(QPixmap::fromImage(qImageVector[2]));
        transfer_function();
    }
    else {
        QMessageBox::critical(this,"Image Loading error","need more images");
    }
}


void Image_stitching::on_saveImage_button_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Image File"),
                                                    "C:/HIWI/images",
                                                    tr("Images(*.png *.xpm *.jpg)"));
    cv::imwrite(fileName.toStdString(),this->result_save);
}
