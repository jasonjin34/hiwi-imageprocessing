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
#include "message.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow  *ui;
    window_histmatching *hismatchinggui;
    QGraphicsScene  m_scene_orig, m_scene_res;
    QGraphicsView   m_view;
    cv::Mat         m_image;
    cv::Mat         m_result;
    cv::Mat         m_refer; //reference image for hisMatch


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_button();
    void on_search_file();

private slots:
    void onMessageSent(message);

private:
    void transform();
};
