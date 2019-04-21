#pragma once

#include <QMainWindow>
#include <QGraphicsTextItem>
#include <QLineEdit>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <iostream>
#include <vector>

#include "histogram.h"
#include "imgfunctions.h"
#include "window_histmatching.h"
#include "image_stitching.h"
#include "curve.h"
#include "message.h"
#include "videostreaming.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow  *ui;
    window_histmatching *hismatchinggui;
    Curve *curvegui;
    Image_stitching *stitchinggui;
    Videostreaming *streaminggui;
    QGraphicsScene  m_scene_orig, m_scene_res;
    QGraphicsView   m_view;
    cv::Mat         m_image;
    cv::Mat         m_result;
    bool Equlizationdivide;


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_button();
    void on_search_file();

private slots:
    void onMessageSent(message);

    void onMessageSentCurve(message);

    void on_actionCurve_triggered();

    void on_resetbutton_clicked();

    void on_actionLanuch_triggered();

    void on_videoStreaming_triggered();

private:
    void transform();
};
