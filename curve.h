#ifndef CURVE_H
#define CURVE_H

#include <QDialog>

namespace Ui {
class Curve;
}

class Curve : public QDialog
{
    Q_OBJECT
public:
    explicit Curve(QWidget *parent = nullptr);
    ~Curve();

    void addPoint(double x, double y);
    void clearData();
    void plot();

private slots:

    void on_addpoint1_clicked();
    void clickedGraph(QMouseEvent *event);
    void on_button_clear_clicked();


private:
    Ui::Curve *ui;

    QVector<double> qv_x,qv_y;
};

#endif // CURVE_H
