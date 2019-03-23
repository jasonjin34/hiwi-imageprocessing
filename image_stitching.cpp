#include "image_stitching.h"
#include "ui_image_stitching.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <QFileDialog>
#include <QDebug>

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
    connect(ui->transf_button,SIGNAL(clicked()),this,SLOT(on_transf_button_clicked()));
    //connect(ui->searchImage_stitching,SIGNAL(clicked()),this,SLOT(on_searchImage_stitching_clicked()));

}

Image_stitching::~Image_stitching()
{
    delete ui;
}

void Image_stitching::on_transf_button_clicked()
{
    std::vector<cv::Mat> imageVector_pano;
    cv::Mat result;
    iaw::loadImagevector(files,imageVector_pano);
    iaw::imageStitching(imageVector_pano,result);

    //set image
    cv::Mat temp;
    cv::cvtColor(result,temp,CV_BGR2RGB);
    QImage dest(temp.data,temp.cols,temp.rows,static_cast<int>(temp.step),QImage::Format_RGB888);
    QImage destscaled = dest.scaled(1000,1000,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    stitching_result.addPixmap(QPixmap::fromImage(destscaled));
}

void Image_stitching::on_closeGUI_clicked()
{
    this->close();
}

void Image_stitching::on_searchImage_stitching_clicked()
{
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
    else qDebug()<<"error";
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
}