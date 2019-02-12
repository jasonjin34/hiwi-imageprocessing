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

private:
    Ui::Curve *ui;
};

#endif // CURVE_H
