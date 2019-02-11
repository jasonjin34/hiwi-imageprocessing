#ifndef WINDOW_HISTMATCHING_H
#define WINDOW_HISTMATCHING_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QString>
#include "message.h"
#include "imgfunctions.h"


namespace Ui {
class window_histmatching;
}

class window_histmatching : public QDialog
{
    Q_OBJECT
    QGraphicsScene  input_scene, output_scene;
    QGraphicsView   histogram_view;

public:
    explicit window_histmatching(QWidget *parent = nullptr);
    ~window_histmatching();
    void setinput(cv::Mat& input);
    void setoutput(cv::Mat& output);

signals:
    void notifyMessageSent(const message&);

private slots:
    void on_maxIntensity_valueChanged(int arg1);

    void on_minIntensity_valueChanged(int arg1);

private:
    Ui::window_histmatching *ui;
    cv::Mat input;
    cv::Mat output_src;
};

#endif // WINDOW_HISTMATCHING_H
