#include "curve.h"
#include "ui_curve.h"
#include <QDebug>

message message_singal_curve;

Curve::Curve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Curve)
{
    ui->setupUi(this);

    ui->curve_plot->addGraph();
    ui->curve_plot->xAxis->setRange(0,255);
    ui->curve_plot->yAxis->setRange(0,255);
    ui->curve_plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->curve_plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->curve_plot->xAxis->setLabel("Pixel Intensity");
    ui->curve_plot->yAxis->setLabel("Dark                                                                            Light");


    connect(ui->addpoint1, SIGNAL(clicked()), this, SLOT(on_addpoint1_click()));
    connect(ui->button_clear,SIGNAL(clicked()),this,SLOT(on_button_clear_clicked()));
    connect(ui->curve_plot,SIGNAL(mousePress(QMouseEvent*)),this,SLOT(clickedGraph(QMouseEvent*)));
}

Curve::~Curve()
{
    delete ui;
}

void Curve::addPoint(double x, double y)
{
    if(qv_x.size() <= ui->numInterpolation->text().toInt() + 1) //max interpolation point
    {
        qv_x.append(x);
        qv_y.append(y);
    }
}

void Curve::clearData()
{
    qv_x.clear();
    qv_y.clear();
}

void Curve::plot()
{
    ui->curve_plot->graph(0)->setData(qv_x,qv_y);
    ui->curve_plot->replot();
    ui->curve_plot->update();
}

void Curve::on_addpoint1_clicked()
{
    addPoint(ui->x_point1->value(),ui->y_point1->value());
    plot();
}

void Curve::on_button_clear_clicked()
{
    clearData();
    plot();
}

double Curve::interpolation(double x)
{
    //largarne basis
    QVector<double> coeff_lagrange;
    for(int j = 0; j < qv_x.size();++j)
    {
        double temp_coeff = 1.0;
        for(int k = 0; k < qv_x.size();++k){
            if(k != j){
                temp_coeff = temp_coeff* (x - qv_x.at(k))/(qv_x.at(j) - qv_x.at(k));
            }
        }
        coeff_lagrange.append(temp_coeff);
    }
    double output_temp = 0.0;
    for(int i = 0; i < qv_x.size();i++){
        output_temp =  output_temp + coeff_lagrange.at(i) * qv_y.at(i);
    }

    return output_temp;
}


void Curve::clickedGraph(QMouseEvent *event)
{
    QPoint point = event->pos();
    double x_value = ui->curve_plot->xAxis->pixelToCoord((point.x()));
    double y_value = ui->curve_plot->yAxis->pixelToCoord((point.y()));

    addPoint(x_value,y_value);

    ui->x_point1->setValue(qv_x.front());
    ui->y_point1->setValue(qv_y.front());
    ui->x_point2->setValue(qv_x.back());
    ui->y_point2->setValue(qv_y.back());

    if(qv_x.size() == ui->numInterpolation->text().toInt())
    {
        QVector<double> image_interoplation_y;
        QVector<double> image_interoplation_x;
        for(int i = 1; i < 255; i++)
        {
            image_interoplation_y.append(interpolation(static_cast<double>(i)));
            image_interoplation_x.append(i);
        }
        ui->curve_plot->graph(0)->setData(image_interoplation_x,image_interoplation_y);
        ui->curve_plot->replot();
        ui->curve_plot->update();
        message_singal_curve.setalphafactor(image_interoplation_y);
        emit notifyMessageSentCurve(message_singal_curve);
    }
    else{
        plot();
    }
}

void Curve::on_x_point1_valueChanged(double arg1)
{
    message_singal_curve.setAlphaMin(1.0 + 2.0*arg1/255.0);
    emit notifyMessageSentCurve(message_singal_curve);
}

void Curve::on_x_point2_valueChanged(double arg1)
{
    message_singal_curve.setAlphaMax(100.0*arg1/255.0);
    emit notifyMessageSentCurve(message_singal_curve);
}
