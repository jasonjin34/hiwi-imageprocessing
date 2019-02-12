#include "curve.h"
#include "ui_curve.h"
#include <QDebug>

Curve::Curve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Curve)
{
    ui->setupUi(this);

    ui->curve_plot->addGraph();
    ui->curve_plot->xAxis->setRange(0,255);
    ui->curve_plot->yAxis->setRange(0,255);
    ui->curve_plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->curve_plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));


    connect(ui->addpoint1, SIGNAL(clicked()), this, SLOT(on_addpoint1_click()));
    connect(ui->button_clear,SIGNAL(clicked()),this,SLOT(on_button_clear_clicked()));
    connect(ui->curve_plot,SIGNAL(mousePress(QMouseEvent*)),this,SLOT(clickedGraph(QMouseEvent*)));
    qDebug() << "pass";
    int size = 10;
    QVector<double> x(size),y(size);

    /**
    for(int i = 0; i < size; ++i){
        x[i] = i;
        y[i] = i;
    }
    **/
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

void Curve::clickedGraph(QMouseEvent *event)
{
    QPoint point = event->pos();
    double x_value = ui->curve_plot->xAxis->pixelToCoord((point.x()));
    double y_value = ui->curve_plot->yAxis->pixelToCoord((point.y()));

    ui->x_point1->setValue(qv_x.front());
    ui->y_point1->setValue(qv_y.front());
    ui->x_point2->setValue(qv_x.back());
    ui->y_point2->setValue(qv_y.back());

    addPoint(x_value,y_value);
    plot();
}
