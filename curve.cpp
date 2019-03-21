#include "curve.h"
#include "ui_curve.h"
#include <QCoreApplication>
#include <QFileDialog>
#include <QDebug>

message message_singal_curve;

Curve::Curve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Curve)
{
    ui->setupUi(this);
    ui->curve_plot->setMouseTracking(true);

    //display interpolation
    ui->curve_plot->addGraph();
    ui->curve_plot->xAxis->setRange(0,255);
    ui->curve_plot->yAxis->setRange(0,255);
    ui->curve_plot->xAxis->grid()->setSubGridVisible(false);
    ui->curve_plot->yAxis->grid()->setSubGridVisible(false);
    ui->curve_plot->xAxis->setLabel("Pixel Intensity");
    ui->curve_plot->yAxis->setLabel("Dark                                                                              Light");
    ui->curve_plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    QPen interPen = ui->curve_plot->graph(0)->pen();
    interPen.setWidth(2);
    ui->curve_plot->graph(0)->setPen(interPen);

    //reference line
    ui->curve_plot->addGraph();
    ui->curve_plot->graph(1)->setPen(QPen(Qt::black));
    QVector<double> x(10),y(10);
    for(int i = 0; i < 10; ++i){
        x[i] = 1 + (i+1)/10*254;
        y[i] = x[i];
    }
    ui->curve_plot->graph(1)->setData(x,y);

    //interpolation points
    ui->curve_plot->addGraph();
    ui->curve_plot->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->curve_plot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,8));
    ui->curve_plot->graph(2)->setPen(QPen(Qt::black));


    connect(ui->button_clear,SIGNAL(clicked()),this,SLOT(on_button_clear_clicked()));
    connect(ui->curve_plot,SIGNAL(mousePress(QMouseEvent*)),this,SLOT(clickedGraph(QMouseEvent*)));
    connect(ui->Reset_button,SIGNAL(doubleclicked()),this,SLOT(on_Reset_button_clicked()));
    connect(ui->curve_plot,SIGNAL(mouseMove(QMouseEvent*)),this, SLOT(mousePosition(QMouseEvent*)));
}

Curve::~Curve()
{
    delete ui;
}

bool Curve::addPoint(double x, double y)
{
    if(qv_x.size() <= ui->numInterpolation->text().toInt() + 1) //max interpolation point
    {
        int position = qv_x.size()-1;
        qv_x.insert(position,x);
        qv_y.insert(position,y);
        return true;
    }
    else return false;
}

void Curve::updatePoint(double x, double y)
{
    QVector<double>::iterator it_x, it_y;
    QVector<double> vector_distance;
    QVector<int> vector_position;
    int temp_position = 0;
    for(it_x = qv_x.begin(),it_y = qv_y.begin();it_x < qv_x.end();++it_x,++it_y)
    {
        //check if the cursor position with radius 35
        double distance = sqrt((*it_x-x)*(*it_x-x)+(*it_y-y)*(*it_y-y));
        if(distance < 35)
        {
           //get all the point around the cursor only update the nearest one
           vector_distance.append(distance);
           vector_position.append(temp_position);
        }
        ++temp_position;
    }
    double distance_min = vector_distance[0];
    int vector_distance_index = 0;
    for(int i = 0; i < vector_distance.size(); i++)
    {
        if(vector_distance[i] < distance_min)
        {
             distance_min = vector_distance[i];
             vector_distance_index = i;
        }
    }

    //update the nearest point
    qv_x[vector_position[vector_distance_index]] = x;
    qv_y[vector_position[vector_distance_index]] = y;

}

bool Curve::aroundPoint(double x, double y)
{
    QVector<double>::iterator it_x, it_y;
    for(it_x = qv_x.begin(),it_y = qv_y.begin();it_x < qv_x.end();++it_x,++it_y)
    {
        //check if the cursor position with radius 8
        if(sqrt((*it_x-x)*(*it_x-x)+(*it_y-y)*(*it_y-y)) < 8)
        {
            return true;
        }
    }
    return false;
}

void Curve::deletePoint(double x, double y)
{
    if(aroundPoint(x,y))
    {
        QVector<double>::iterator it_x, it_y;
        int temp_position = 0;
        for(it_x = qv_x.begin(),it_y = qv_y.begin();it_x < qv_x.end();++it_x,++it_y)
        {
            //check if the cursor position with radius 5
            if(sqrt((*it_x-x)*(*it_x-x)+(*it_y-y)*(*it_y-y)) < 5)
            {
               qv_x.remove(temp_position);
               qv_y.remove(temp_position);
            }
            ++temp_position;
        }
    }
}

void Curve::updateGraph()
{
    ui->x_point1->setValue(qv_x.front());
    ui->y_point1->setValue(qv_y.front());
    ui->x_point2->setValue(qv_x.back());
    ui->y_point2->setValue(qv_y.back());

    QVector<double> image_interoplation_y;
    QVector<double> image_interoplation_x;
    for(int i = 1; i < 255; i++)
    {
        double current_value = interpolation(static_cast<double>(i));
        image_interoplation_y.append(current_value > 255 ? 255:(current_value < 0 ? 0:current_value));
        image_interoplation_x.append(i);
    }
    ui->curve_plot->graph(0)->setData(image_interoplation_x,image_interoplation_y);
    ui->curve_plot->graph(2)->setData(qv_x,qv_y);
    ui->curve_plot->replot();
    ui->curve_plot->update();
    message_singal_curve.setalphafactor(image_interoplation_y);
    emit notifyMessageSentCurve(message_singal_curve);
}

void Curve::clearData()
{
    qv_x.clear();
    qv_y.clear();
    qv_y.push_front(1.0);
    qv_y.append(255.0);
    qv_x.push_front(1.0);
    qv_x.append(255.0);
}

void Curve::plot()
{
    QVector<double> temp;
    temp.append(0.0);
    ui->curve_plot->graph(0)->setData(temp,temp);
    ui->curve_plot->graph(2)->setData(qv_x,qv_y);

    ui->curve_plot->replot();
    ui->curve_plot->update();
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
    if(qv_x[0] != 1.0){
        qv_y.push_front(1.0);
        qv_y.append(255.0);
        qv_x.push_front(1.0);
        qv_x.append(255.0);
    }
    if(event->buttons() & Qt::LeftButton)
    {
        if(!aroundPoint(x_value,y_value))
        {
            if(addPoint(x_value,y_value))
            {
                updateGraph();
            }
            else
            {
                QMessageBox::warning(this, "Interpolation Points Warning","reach to the max number of points");
            }
        }
    }
    else if(event->buttons() & Qt::RightButton)
    {
        deletePoint(x_value,y_value);
        updateGraph();
    }
}

void Curve::mousePosition(QMouseEvent *event)
{
    QPoint point = event->pos();
    //get the current cursor poistion
    double x_value = ui->curve_plot->xAxis->pixelToCoord((point.x()));
    double y_value = ui->curve_plot->yAxis->pixelToCoord((point.y()));
    ui->CurrentPosition->setText(QString("%1,%2").arg(static_cast<int>(x_value)).arg(static_cast<int>(y_value)));

    if(event->buttons() & Qt::LeftButton)
    {
        if(qv_x.size() <= ui->numInterpolation->text().toInt() + 2)
        {
            if(qv_x[0] != 1.0){
                qv_y.push_front(1.0);
                qv_y.append(255.0);
                qv_x.push_front(1.0);
                qv_x.append(255.0);
            }
            if(qv_x.size() <= 2) addPoint(x_value,y_value);
            else updatePoint(x_value,y_value);
            updateGraph();
        }
        else
        {
            QMessageBox::warning(this, "Interpolation Points Warning","reach to the max number of points");
        }

    }
    else if(event->button())
    {
        addPoint(x_value,y_value);
        qDebug() << qv_x;
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

void Curve::on_Reset_button_clicked()
{
    message_singal_curve.clearMessage();
    message_singal_curve.setReset(true);
    emit notifyMessageSentCurve(message_singal_curve);
}
