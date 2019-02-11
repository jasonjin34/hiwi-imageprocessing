#include "window_histmatching.h"
#include "ui_window_histmatching.h"
#include <QImage>
#include <QDebug>

message message_signal;

window_histmatching::window_histmatching(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::window_histmatching)
{
    ui->setupUi(this);
    ui->inputhistogram->setScene(&input_scene);
    ui->outputhistogram->setScene(&output_scene);

    connect(ui->verticalMax,SIGNAL(valueChanged(int)),ui->maxIntensity,SLOT(setValue(int)));
    connect(ui->verticalMin,SIGNAL(valueChanged(int)),ui->minIntensity,SLOT(setValue(int)));
}

void window_histmatching::setinput(cv::Mat &input)
{
    this->input = input;
    cv::Mat output;
    iaw::histDraw(input,output,message_signal.getMin(),message_signal.getMax());

    QImage dest(output.data,output.cols,output.rows,static_cast<int>(output.step),QImage::Format_Grayscale8);
    QImage dest_scaled = dest.scaled(400,300, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    input_scene.addPixmap(QPixmap::fromImage(dest_scaled));
}

void window_histmatching::setoutput(cv::Mat &output){
    this->output_src = output;
}

window_histmatching::~window_histmatching()
{
    delete ui;
}


void window_histmatching::on_maxIntensity_valueChanged(int arg1)
{
    message_signal.setMax(arg1);
    emit notifyMessageSent(message_signal);

    cv::Mat output; //histogram output
    iaw::histDraw(this->output_src,output,message_signal.getMin(),message_signal.getMax());

    QImage dest(output.data,output.cols,output.rows,static_cast<int>(output.step),QImage::Format_Grayscale8);
    QImage dest_scaled = dest.scaled(400,300, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    output_scene.addPixmap(QPixmap::fromImage(dest_scaled));
}

void window_histmatching::on_minIntensity_valueChanged(int arg1)
{
    message_signal.setMin(arg1);
    emit notifyMessageSent(message_signal);
    cv::Mat output; //histogram output
    iaw::histDraw(this->output_src,output,message_signal.getMin(),message_signal.getMax());

    QImage dest(output.data,output.cols,output.rows,static_cast<int>(output.step),QImage::Format_Grayscale8);
    QImage dest_scaled = dest.scaled(400,300, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    output_scene.addPixmap(QPixmap::fromImage(dest_scaled));
}
