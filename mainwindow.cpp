#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "convfunktion.h"
#include <QCoreApplication>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    hismatchinggui(nullptr)
{   
    ui->setupUi(this);
    ui->viewOriginal->setScene(&m_scene_orig);
    ui->viewResult->setScene(&m_scene_res);

    connect(ui->searchImage, SIGNAL(clicked()), this, SLOT(on_search_file()));
    connect(ui->btnTransform, SIGNAL(clicked()), this, SLOT(on_button()));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button()
{
    if(!m_image.empty()) {
        transform();
    } else {
        QMessageBox::critical(this, "Image Transformation Error", "image is not loaded");
    }
}

void MainWindow::on_search_file()
{
    m_image = cv::Mat();
    m_scene_orig.clear();
    m_scene_res.clear();

    QString filename = QFileDialog::getOpenFileName(this, "Open Image", "C:/HIWI/images/backscatter", "Images (*.png *.jpg *.bmp)");
    if(!filename.isEmpty()) {        
        cv::Mat in = cv::imread(filename.toStdString(), 1);
        cv::cvtColor(in, m_image,cv::COLOR_BGR2GRAY);
        if(!m_image.empty()) {
            ui->imageFile->setText(filename);
            m_scene_orig.addPixmap(QPixmap::fromImage(Mat2QImageGrayscale(m_image)));
        } else {
            QMessageBox::critical(this, "Image Loading Error", "cannot load the image " + filename);
        }
    }
}

void MainWindow::onMessageSent(message message_signal){
    m_scene_res.clear();
    m_result = m_image.clone();

    //histogram equalization: level adjustment GUI
    iaw::histEqual_leveladj(m_result,m_result, message_signal.getMin(), message_signal.getMax(),ui->optHisMatching->isChecked());
    hismatchinggui->setoutput(m_result);

    m_scene_res.addPixmap(QPixmap::fromImage(Mat2QImageGrayscale(m_result)));
}

void MainWindow::transform()
{
    m_scene_res.clear();
    m_result = m_image.clone();

    if(!ui->optNoise->isChecked()) {
        if(ui->optNoiseBlur->isChecked()) {
            iaw::noise_blur(m_result, m_result);
        } else if(ui->optNoiseAvarage->isChecked()) {
            iaw::noise_average(m_result, m_result);
        } else if(ui->optNoiseGauss->isChecked()) {
            iaw::noise_gauss(m_result, m_result);
        } else if(ui->optNoiseMedian->isChecked()) {
            iaw::noise_median(m_result, m_result);
        } else if(ui->optNoiseBilateral->isChecked()) {
            iaw::noise_bilateral(m_result, m_result);
        } else if(ui->optNoiseFastNImean->isChecked()){
            iaw::noise_fastNImeansDenoising(m_result,m_result);
        }
    }

    if(!ui->optThreshold->isChecked()) {
        if(ui->optThresSimple->isChecked()) {
            iaw::threshold_simple(m_result, m_result);
        } else if(ui->optThresAdaptiveMean->isChecked()) {
            iaw::threshold_adaptive_mean(m_result, m_result);
        } else if(ui->optThresAdaptiveGaus->isChecked()) {
            iaw::threshold_adaptive_gauss(m_result, m_result);
        } else if(ui->optThresOtsu->isChecked()) {
            iaw::threshold_otsu(m_result, m_result);
        }
    }

    if(!ui->optHistogram->isChecked()) {
        if(ui->optHisEqual->isChecked()) {
            iaw::histEqual(m_result, m_result);
        } else if(ui->optHisMatching->isChecked() || ui->optHisMatchingnoEqu->isChecked()) {
            if(!hismatchinggui)
            {
                hismatchinggui = new window_histmatching;
                hismatchinggui->setinput(m_result);

                connect(hismatchinggui, &window_histmatching::notifyMessageSent,this,&MainWindow::onMessageSent);
            }
            hismatchinggui->show();
        } else if(ui->optHisClahe->isChecked()) {
            iaw::histClahe(m_result, m_result);
        } else if(ui->optHistogramDraw->isChecked()){
            iaw::histDraw(m_result,m_result,0,255);
        }
    }

    if(!ui->optCanny->isChecked()) {
        if(ui->optCanny25->isChecked()) {
            iaw::cannyEdge(m_result, m_result, 25, ui->optCannyMerge->isChecked());
        } else if(ui->optCanny50->isChecked()) {
            iaw::cannyEdge(m_result, m_result, 50, ui->optCannyMerge->isChecked());
        } else if(ui->optCanny75->isChecked()) {
            iaw::cannyEdge(m_result, m_result, 75, ui->optCannyMerge->isChecked());
        } else if(ui->optCanny100->isChecked()) {
            iaw::cannyEdge(m_result, m_result, 100, ui->optCannyMerge->isChecked());
        }
    }

    m_scene_res.addPixmap(QPixmap::fromImage(Mat2QImageGrayscale(m_result)));
}

