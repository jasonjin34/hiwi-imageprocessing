#ifndef COMPOSITION_H
#define COMPOSITION_H

#include <QFileDialog>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include "qcustomplot.h"

namespace Ui {
class Composition;
}

class Composition : public QWidget
{
    Q_OBJECT

public:
    explicit Composition(QWidget *parent = nullptr);
    ~Composition();

private slots:
    void on_loadImageButton_clicked();
    void mouseMove(QMouseEvent*);
    void mousePress(QMouseEvent*);
    void composeImage();
    void alterImage();

private:
    Ui::Composition *ui;
    QVector<QImage> sourceImageVector;
    QCPItemRect* rect;
    QImage source_image, destination_image, temp_image;
    int destination_image_index;
    QPoint mousePosition;
};

#endif // COMPOSITION_H
