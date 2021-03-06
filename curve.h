#ifndef CURVE_H
#define CURVE_H

#include <QDialog>
#include "message.h"

namespace Ui {
class Curve;
}

class Curve : public QDialog
{
    Q_OBJECT
public:
    explicit Curve(QWidget *parent = nullptr);
    ~Curve();

    bool addPoint(double x, double y);
    /**
     * @brief updatePoint update the last point
     * @param x coordinate position
     * @param y coordinate position
     */
    void updatePoint(double x, double y);
    /**
     * @brief aroundPoint check if the curror is around the current exsiting point
     * @param x
     * @param y
     */
    int aroundPoint(double x, double y);
    void deletePoint(double x, double y);
    void updateGraph();
    void clearData();
    void plot();
signals:
    void notifyMessageSentCurve(const message&);

private slots:
    void clickedGraph(QMouseEvent *event);
    void on_button_clear_clicked();
    double interpolation(double x);
    void on_x_point1_valueChanged(double arg1);
    void on_x_point2_valueChanged(double arg1);
    void on_Reset_button_clicked();
    void mouseMove(QMouseEvent *event);
    void mouseRelease(QMouseEvent* evt);

private:
    Ui::Curve *ui;

    QVector<double> qv_x,qv_y;
    bool tracking;
    int trackPoint;
};

#endif // CURVE_H
