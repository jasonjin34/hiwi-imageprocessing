#ifndef WINDOW_HISTMATCHING_H
#define WINDOW_HISTMATCHING_H

#include <QDialog>
#include <QString>

namespace Ui {
class window_histmatching;
}

class window_histmatching : public QDialog
{
    Q_OBJECT

public:
    explicit window_histmatching(QWidget *parent = nullptr);
    ~window_histmatching();

signals:
    void notifyMessageSent(const int&);

private slots:
    void on_maxIntensity_valueChanged(int arg1);

private:
    Ui::window_histmatching *ui;
};

#endif // WINDOW_HISTMATCHING_H
