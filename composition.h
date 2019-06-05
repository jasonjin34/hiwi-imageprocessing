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
    void resizeEvent(QResizeEvent *);
    void reloadImage();

private slots:
    void on_loadImageButton_clicked();
    void mouseMove(QMouseEvent*);
    void mousePress(QMouseEvent*);
    void mouseRelease(QMouseEvent*);
    void composeImage();
    void scaleFunction(int);
    // void alterImage();
    void imagerender();
    bool overlapfunction();
    bool checkPoint(QPointF);

private:
    Ui::Composition *ui;
    QVector<QImage> sourceImageVector;
    QCPItemRect* rect;
    QImage source_image, destination_image, temp_image;
    int destination_image_index;
    QPointF mousePosition_new, mousePosition_old, topRight_overlap_reference, bottomRight_overlap_reference,topleft_overlap_origin, bottomRight_overlap_origin;
    QCPItemPixmap *image_pixmap_origin, *image_pixmap_reference;
    QPixmap start_image, reference_image;
    QVector<QPointF> centerList;
    bool tracking;
    int trackPoint;
    double scale_origin;
    double scale_reference;
    QString source_layer;
    double ratio;
    QRect targetRect, sourceRect;
    QPainter::CompositionMode mode;
    QString showlayerString;
    QCPItemRect * xRectItem;
};

#endif // COMPOSITION_H
