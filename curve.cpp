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
    qv_x.append(x);
    qv_y.append(y);

    if(qv_x.size() > 2)
    {
        qv_x.pop_front();
        qv_y.pop_front();
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
    double x1 = 0.0;
    double x2 = qv_x.back();
    double x3 = qv_x.front();
    double x4 = 255.0;

    double y2 = qv_y.back();
    double y3 = qv_y.front();
    double y4 = 255.0;

    double a_coeff, b_coeff, c_coeff, d_coeff;
    a_coeff = 0.0;
    b_coeff = (x - x1)*(x - x3)*(x - x4)*y2/((x2 - x1)*(x2 - x3)*(x2 - x4));
    c_coeff = (x - x1)*(x - x2)*(x - x4)*y3/((x3 - x1)*(x3 - x2)*(x3 - x4));
    d_coeff = (x - x1)*(x - x2)*(x - x3)*y4/((x4 - x1)*(x4 - x2)*(x4 - x3));

    return a_coeff + b_coeff + c_coeff + d_coeff;
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

    //spline cubic interpolator
    if(qv_x.size()>=2)
    {
       QVector<double> image_interoplation_y;
       QVector<double> image_interoplation_x;
       for(int i = 0; i < 255; i++)
       {
           image_interoplation_y.append(interpolation(i));
           image_interoplation_x.append(i);
       }
       ui->curve_plot->graph(0)->setData(image_interoplation_x,image_interoplation_y);
       ui->curve_plot->replot();
       ui->curve_plot->update();
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
