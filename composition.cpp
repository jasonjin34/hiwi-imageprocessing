#include "composition.h"
#include "ui_composition.h"
#include <QDebug>
// TO DO
// 1. more functions
// 2. scaled
// 3. moveable for both image

Composition::Composition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Composition)
{
    ui->setupUi(this);

    // set graphic scene
    ui->resultImage->setMouseTracking(true);
    ui->resultImage->xAxis->setVisible(true);
    ui->resultImage->yAxis->setVisible(true);
    ui->resultImage->setBackgroundScaled(true);
    ui->resultImage->setBackgroundScaledMode(Qt::KeepAspectRatio);
    this->destination_image_index = 0;

    // add combobox items
    ui->composition_type->addItem("SourceOver");
    ui->composition_type->addItem("DestinationOver");
    ui->composition_type->addItem("Xor");
    ui->composition_type->addItem("Darken");
    ui->composition_type->addItem("Lighten");
    ui->composition_type->addItem("ColorDodge");
    ui->composition_type->addItem("ColorBurn");
    ui->composition_type->addItem("HardLight");
    ui->composition_type->addItem("SoftLight");
    ui->composition_type->addItem("Difference");

    // slot signal
    connect(ui->loadImageButton, SIGNAL(click()),this, SLOT(on_loadImageButton_clicked()));
    connect(ui->resultImage,SIGNAL(mouseMove(QMouseEvent*)),this, SLOT(mouseMove(QMouseEvent*)));
    connect(ui->resultImage,SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
    connect(ui->composition_type,SIGNAL(activated(int)), this, SLOT(composeImage()));
    connect(ui->resultImage,SIGNAL(mouseRelease(QMouseEvent*)),this, SLOT(mouseRelease(QMouseEvent*)));
    connect(ui->scaleValue,SIGNAL(valueChanged(int)),this,SLOT(scaleFunction(int)));

    // add pixmap
    image_pixmap_origin = new QCPItemPixmap(ui->resultImage);
    image_pixmap_origin->setVisible(true);
    ui->resultImage->addLayer("image_origin");
    image_pixmap_origin->setLayer("image_origin");
    image_pixmap_origin->setScaled(true);

    image_pixmap_reference = new QCPItemPixmap(ui->resultImage);
    image_pixmap_reference->setVisible(true);
    ui->resultImage->addLayer("image_reference");
    image_pixmap_reference->setLayer("image_reference");
    image_pixmap_reference->setScaled(true);

    // set default tracking parameter
    tracking = false;
    scale_origin = 4.0;
    scale_reference = 4.0;

    xRectItem = new QCPItemRect(this->ui->resultImage);
    xRectItem->setVisible(false);
    xRectItem->setPen(QPen(Qt::red));
}

Composition::~Composition()
{
    delete ui;
}

void Composition::on_loadImageButton_clicked()
{
    if(this->sourceImageVector.isEmpty())
    {
        //load input file names
        QStringList inputFilenameList;
        inputFilenameList = QFileDialog::getOpenFileNames(
                    this,
                    "Open Image List",
                    "C:/HIWI/images/stitching",
                    "Images (*.png *.jpg *.bmp)"
                    );

        // load images
        QImage temp_image;
        int num_vector = inputFilenameList.length();

        // get the plot size
        int width, height;
        width = ui->resultImage->width();
        height = ui->resultImage->height();

        for(int it = 0; it < num_vector; it++)
        {
            temp_image.load(inputFilenameList[it]);
            QImage destscaled = temp_image.scaled(width,height,Qt::KeepAspectRatio,Qt::SmoothTransformation);
            sourceImageVector.append(destscaled);
        }
        start_image = QPixmap::fromImage(sourceImageVector[0]);
        reference_image = QPixmap::fromImage(sourceImageVector[1]);
        image_pixmap_origin->setPixmap(reference_image);
        image_pixmap_reference->setPixmap(start_image);

        ratio = static_cast<double>(start_image.size().width()) / static_cast<double>(start_image.size().height());
        double image_width, image_height;
        double scale = 4;
        image_width = 5.0 / scale;
        image_height = ratio * 5.0 / scale;


        image_pixmap_origin->topLeft->setCoords(2.0 - image_width, 3.0 + image_height);
        image_pixmap_origin->bottomRight->setCoords(2.0 + image_width, 3.0 - image_height);
        QPointF center = QPointF(2, 3);
        this->centerList.append(center);

        image_pixmap_reference->topLeft->setCoords(3.0 - image_width, 2.0 + image_height);
        image_pixmap_reference->bottomRight->setCoords(3.0 + image_width, 2.0 - image_height);
        center= QPoint(3, 2);
        this->centerList.append(center);

        //load source image for comparision
        this->source_image = this->sourceImageVector[0];
        this->destination_image = this->sourceImageVector[1];

        ui->resultImage->replot();
    }
    else {
        this->sourceImageVector.clear();
    }
    //set the origin source
    composeImage();

    scale_origin = 4.0;
    scale_reference = 4.0;
}

void Composition::mouseMove(QMouseEvent* event)
{
   double x_temp = ui->resultImage->xAxis->pixelToCoord(event->pos().x());
   double y_temp = ui->resultImage->yAxis->pixelToCoord(event->pos().y());
   this->mousePosition_new = QPointF(x_temp, y_temp);

   double mousePosition_x_diff = this->mousePosition_new.x() - this->mousePosition_old.x();
   double mousePosition_y_diff = this->mousePosition_new.y() - this->mousePosition_old.y();

   if(tracking)
   {
       bool pointInOrigin = false;
       bool pointInReference = false;
       bool intersection = false;
       if( x_temp > centerList[0].x() - 5.0 / scale_origin && x_temp < centerList[0].x() + 5.0 / scale_origin
               && y_temp > centerList[0].y() - ratio * 5.0 / scale_origin && y_temp < centerList[0].y() + ratio * 5.0 / scale_origin)
       {
           pointInOrigin = true;
       }
       if( x_temp > centerList[1].x() - 5.0 / scale_origin && x_temp < centerList[1].x() + 5.0 / scale_origin
               && y_temp > centerList[1].y() - ratio * 5.0 / scale_origin && y_temp < centerList[1].y() + ratio * 5.0 / scale_origin)
       {
           pointInReference = true;
       }

       if (pointInOrigin && pointInReference) intersection = true;

       if(pointInOrigin && !intersection)
       {
           showlayerString = "Destination";
           x_temp = centerList[0].x() + mousePosition_x_diff;
           y_temp = centerList[0].y() + mousePosition_y_diff;
           centerList[0] = QPointF(x_temp, y_temp);
           ui->ShowLayer->setText(showlayerString);
           imagerender();
       }
       else if(pointInReference && !intersection){
           showlayerString = "Source";
           x_temp = centerList[1].x() + mousePosition_x_diff;
           y_temp = centerList[1].y() + mousePosition_y_diff;
           centerList[1] = QPointF(x_temp, y_temp);
           ui->ShowLayer->setText(showlayerString);
           imagerender();
       }
       else if(intersection)
       {
           if(this->source_layer == "SourceOver")
           {
               showlayerString = "Source";
               x_temp = centerList[1].x() + mousePosition_x_diff;
               y_temp = centerList[1].y() + mousePosition_y_diff;
               centerList[1] = QPointF(x_temp, y_temp);
               ui->ShowLayer->setText(showlayerString);
               imagerender();
           }
           else {
               showlayerString = "Destination";
               x_temp = centerList[0].x() + mousePosition_x_diff;
               y_temp = centerList[0].y() + mousePosition_y_diff;
               centerList[0] = QPointF(x_temp, y_temp);
               ui->ShowLayer->setText(showlayerString);
               imagerender();
           }
       }
       composeImage();
   }
   this->mousePosition_old = this->mousePosition_new;
}

void Composition::mousePress(QMouseEvent * event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        this->tracking = true;
        // init mouse Position
        double x_temp = ui->resultImage->xAxis->pixelToCoord(event->pos().x());
        double y_temp = ui->resultImage->yAxis->pixelToCoord(event->pos().y());
        QPointF initPos = QPointF(x_temp, y_temp);

        this->mousePosition_old = initPos;
        this->mousePosition_new = initPos;
    }
}

void Composition::mouseRelease(QMouseEvent* event)
{
    tracking = false;
    event->accept();
}

void Composition::imagerender()
{
    image_pixmap_origin->topLeft->setCoords(centerList[0].x() - 5.0 / scale_origin, centerList[0].y() + ratio * 5.0 / scale_origin);
    image_pixmap_origin->bottomRight->setCoords(centerList[0].x() + 5.0 / scale_origin, centerList[0].y() - ratio * 5.0 / scale_origin);
    image_pixmap_reference->topLeft->setCoords(centerList[1].x() - 5.0 / scale_reference, centerList[1].y() + ratio * 5.0 / scale_reference);
    image_pixmap_reference->bottomRight->setCoords(centerList[1].x() + 5.0 / scale_reference, centerList[1].y() - ratio * 5.0 / scale_reference);
    ui->resultImage->replot();
}

bool Composition::checkPoint(QPointF point)
{
   bool temp = false;
   double x_diff_left = point.x() - centerList[1].x() + 5.0 / scale_reference;
   double x_diff_right = point.x() - centerList[1].x() - 5.0 / scale_reference;
   double y_diff_left = point.y() - centerList[1].y() + ratio * 5.0 / scale_reference;
   double y_diff_right = point.y() - centerList[1].y() - ratio * 5.0 / scale_reference;

   if(x_diff_left > 0 && x_diff_right < 0)
   {
       if(y_diff_left > 0 && y_diff_right < 0)
       {
           temp = true;
       }
   }
   return temp;
}

bool Composition::overlapfunction()
{
    bool output = false;
    if(!centerList.isEmpty())
    {
        QPointF temp_topright = QPointF(centerList[0].x() + 5.0 / scale_origin, centerList[0].y() + ratio * 5.0 / scale_origin);
        QPointF temp_topleft = QPointF(centerList[0].x() - 5.0 / scale_origin, centerList[0].y() + ratio * 5.0 / scale_origin);
        QPointF temp_bottomright = QPointF(centerList[0].x() + 5.0 / scale_origin, centerList[0].y() - ratio * 5.0 / scale_origin);
        QPointF temp_bottomleft = QPointF(centerList[0].x() - 5.0 / scale_origin, centerList[0].y() - ratio * 5.0 / scale_origin);

        if(checkPoint(temp_bottomright)|| checkPoint(temp_bottomleft) || checkPoint(temp_topright) ||checkPoint(temp_topleft))
        {
            output = true;
        }

        if(output)
        {
            double width_ratio = (centerList[0].x() + 10.0 / scale_origin - centerList[1].x()) / (10.0 / scale_origin);
            double height_ratio = (- centerList[0].y() + ratio * 10.0 / scale_origin + centerList[1].y()) / (ratio * 10.0 / scale_origin);
            double temp_width = sourceImageVector[0].size().width();
            double temp_height = sourceImageVector[0].size().height();
            targetRect = QRect(QPoint(static_cast<int>(temp_width *(1.0 - width_ratio)), static_cast<int>(temp_height*(1.0 -height_ratio))), QPoint(static_cast<int>(temp_width), static_cast<int>(temp_height)));
            sourceRect = QRect(QPoint(0, 0), QPoint(static_cast<int>(temp_width * width_ratio + 1), static_cast<int>(temp_height * height_ratio + 1)));
        }

        // image composition
        this->temp_image = sourceImageVector[0].copy();
        QPainter painter(&temp_image);
        painter.setCompositionMode(mode);
        painter.drawImage(sourceRect, this->destination_image, targetRect);
        painter.end();
        this->source_image = temp_image.copy();
        output =  true;
    }
    return output;
}

void Composition::reloadImage()
{
    start_image = QPixmap::fromImage(source_image);
    reference_image = QPixmap::fromImage(destination_image);
    image_pixmap_origin->setPixmap(reference_image);
    image_pixmap_reference->setPixmap(start_image);
}

void Composition::composeImage()
{
    if (!this->sourceImageVector.isEmpty())
    {
        source_layer = ui->composition_type->currentText();
        if(source_layer == "SourceOver")
        {
            ui->resultImage->moveLayer(ui->resultImage->layer("image_reference"),ui->resultImage->layer("image_origin"));
            source_image = this->sourceImageVector[0].copy();
            destination_image = this->sourceImageVector[1].copy();
            reloadImage();
            ui->resultImage->replot();
            ui->LayerLabel->setText("Move Source Layer");
        }
        else if (source_layer == "DestinationOver")
        {
            ui->resultImage->moveLayer(ui->resultImage->layer("image_origin"),ui->resultImage->layer("image_reference"));
            source_image = this->sourceImageVector[0].copy();
            destination_image = this->sourceImageVector[1].copy();
            reloadImage();
            ui->resultImage->replot();
            ui->LayerLabel->setText("Move Destination Layer");
        }
        else
        {
            ui->resultImage->moveLayer(ui->resultImage->layer("image_reference"),ui->resultImage->layer("image_origin"));
            ui->LayerLabel->setText("Move Destination Layer");
            if(source_layer == "Xor") mode = QPainter::CompositionMode_Xor;
            if(source_layer == "Darken") mode = QPainter::CompositionMode_Darken;
            if(source_layer == "Lighten") mode = QPainter::CompositionMode_Lighten;
            if(source_layer == "ColorDodge") mode = QPainter::CompositionMode_ColorDodge;
            if(source_layer == "ColorBurn") mode = QPainter::CompositionMode_ColorBurn;
            if(source_layer == "HardLight") mode = QPainter::CompositionMode_HardLight;
            if(source_layer == "SoftLight") mode = QPainter::CompositionMode_SoftLight;
            if(source_layer == "Difference") mode = QPainter::CompositionMode_Difference;

            if(overlapfunction())
            {
                qDebug() << "overlap";
                reloadImage();
                topleft_overlap_origin = QPointF(centerList[0].x() - 1.1 * 5.0 / scale_origin,  centerList[0].y() + 1.1 * ratio * 5.0 / scale_origin);
                xRectItem->setVisible(true);
                bottomRight_overlap_origin = QPointF(centerList[0].x() + 1.1 * 5.0 / scale_origin,  centerList[0].y() -  0.8 * ratio * 5.0 / scale_origin);
                xRectItem->topLeft->setCoords(topleft_overlap_origin);
                xRectItem->bottomRight->setCoords(bottomRight_overlap_origin);
                ui->resultImage->replot();

            }
            else
            {
                qDebug() << "not overlap";
            }

        }
    }
}

void Composition::scaleFunction(int arg)
{

    QString selectImageStirng = ui->ShowLayer->text();
    if(selectImageStirng == "Source")
    {
        this->scale_reference = arg / 40.0 * 10.0;
        double resize_ratio = scale_reference / 4.0;
        int temp_width = static_cast<int>(this->source_image.size().width() * resize_ratio);
        int temp_height = static_cast<int>(this->source_image.size().height() * resize_ratio);
        this->source_image.scaled(temp_width, temp_height,Qt::KeepAspectRatio);
        imagerender();
    }
    else if(selectImageStirng == "Destination")
    {
        this->scale_origin = arg / 40.0 * 10.0;
        double resize_ratio = scale_reference / 4.0;
        int temp_width = static_cast<int>(this->destination_image.size().width() * resize_ratio);
        int temp_height = static_cast<int>(this->destination_image.size().height() * resize_ratio);
        this->destination_image.scaled(temp_width, temp_height,Qt::KeepAspectRatio);
        imagerender();
    }
}

void Composition::resizeEvent(QResizeEvent *event)
{
    qDebug() << event->size() << ratio;
    imagerender();
}
