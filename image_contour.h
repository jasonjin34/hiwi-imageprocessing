#ifndef IMAGE_CONTOUR_H
#define IMAGE_CONTOUR_H

#include <QMainWindow>

namespace Ui {
class image_contour;
}

class image_contour : public QMainWindow
{
    Q_OBJECT

public:
    explicit image_contour(QWidget *parent = nullptr);
    ~image_contour();

private:
    Ui::image_contour *ui;
};

#endif // IMAGE_CONTOUR_H
