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
        QPixmap start_image = QPixmap::fromImage(sourceImageVector[0]);
        QPixmap reference_image = QPixmap::fromImage(sourceImageVector[1]);

        ui->resultImage->resize(width, height);
        this->resize(static_cast<int>(width*6.0/5.0),height - 40);
        this->destination_image = sourceImageVector[destination_image_index].copy();
        //image width and height
        double image_width, image_height;
        double scale = 4;
        image_width = 5.0 / scale;
        image_height = 5.0 / scale;


        image_pixmap_origin->topLeft->setCoords(2.0 - image_width, 3.0 + image_height);
        image_pixmap_origin->bottomRight->setCoords(2.0 + image_width, 3.0 - image_height);
        image_pixmap_origin->setPixmap(reference_image);
        QPointF center = QPointF(2, 3);
        this->centerList.append(center);

        image_pixmap_reference->topLeft->setCoords(3.0 - image_width, 2.0 + image_height);
        image_pixmap_reference->bottomRight->setCoords(3.0 + image_width, 2.0 - image_height);
        qDebug() << 3.0 + image_width << 2.0 - image_height;
        image_pixmap_reference->setPixmap(start_image);
        center= QPoint(3, 2);
        this->centerList.append(center);

        ui->resultImage->replot();
    }
    else {
        this->sourceImageVector.clear();
    }
    //set the origin source
    composeImage();
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
               && y_temp > centerList[0].y() - 5.0 / scale_origin && y_temp < centerList[0].y() + 5.0 / scale_origin)
       {
           pointInOrigin = true;
       }
       if( x_temp > centerList[1].x() - 5.0 / scale_origin && x_temp < centerList[1].x() + 5.0 / scale_origin
               && y_temp > centerList[1].y() - 5.0 / scale_origin && y_temp < centerList[1].y() + 5.0 / scale_origin)
       {
           pointInReference = true;
       }

       if (pointInOrigin && pointInReference) intersection = true;

       if(pointInOrigin && !intersection)
       {
           ui->ShowLayer->setText("Destination");
           x_temp = centerList[0].x() + mousePosition_x_diff;
           y_temp = centerList[0].y() + mousePosition_y_diff;
           centerList[0] = QPointF(x_temp, y_temp);
           imagerender();
       }
       else if(pointInReference && !intersection){
           ui->ShowLayer->setText("Source");
           x_temp = centerList[1].x() + mousePosition_x_diff;
           y_temp = centerList[1].y() + mousePosition_y_diff;
           centerList[1] = QPointF(x_temp, y_temp);
           imagerender();
       }
       else if(intersection)
       {
           if(this->source_layer == "SourceOver")
           {
               ui->ShowLayer->setText("Source");
               x_temp = centerList[1].x() + mousePosition_x_diff;
               y_temp = centerList[1].y() + mousePosition_y_diff;
               centerList[1] = QPointF(x_temp, y_temp);
               imagerender();
           }
           else {
               ui->ShowLayer->setText("Destination");
               x_temp = centerList[0].x() + mousePosition_x_diff;
               y_temp = centerList[0].y() + mousePosition_y_diff;
               centerList[0] = QPointF(x_temp, y_temp);
                imagerender();
           }
       }
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
    image_pixmap_origin->topLeft->setCoords(centerList[0].x() - 5 / scale_origin, centerList[0].y() + 5 / scale_origin);
    image_pixmap_origin->bottomRight->setCoords(centerList[0].x() + 5 / scale_origin, centerList[0].y() - 5 / scale_origin);
    image_pixmap_reference->topLeft->setCoords(centerList[1].x() - 5 / scale_reference, centerList[1].y() + 5 / scale_reference);
    image_pixmap_reference->bottomRight->setCoords(centerList[1].x() + 5 / scale_reference, centerList[1].y() - 5 / scale_reference);
    ui->resultImage->replot();
}

void Composition::composeImage()
{
    if (!this->sourceImageVector.isEmpty())
    {
        source_layer = ui->composition_type->currentText();
        if(source_layer == "SourceOver")
        {
            ui->resultImage->moveLayer(ui->resultImage->layer("image_reference"),ui->resultImage->layer("image_origin"));
            ui->resultImage->replot();
        }
        else if (source_layer == "DestinationOver")
        {
            ui->resultImage->moveLayer(ui->resultImage->layer("image_origin"),ui->resultImage->layer("image_reference"));
            ui->resultImage->replot();
        }
    }
}

void Composition::scaleFunction(int arg)
{
    QString selectImageStirng = ui->ShowLayer->text();
    qDebug() << selectImageStirng;
    if(selectImageStirng == "Source")
    {
        this->scale_reference = arg;
        imagerender();
    }
    else if(selectImageStirng == "Destination")
    {
        this->scale_origin = arg;
        imagerender();
    }
}

/*
void Composition::alterImage()
{
    if(!this->sourceImageVector.isEmpty())
    {
        int mouseX = this->mousePosition.x();
        int mouseY = this->mousePosition.y();
        int image_width = sourceImageVector[0].width();
        int image_height = sourceImageVector[0].height();
        int delta_width, delta_height;
        delta_width = static_cast<int>(image_width * 0.5 / 5.0);
        delta_height = static_cast<int>(image_height * 0.5 / 5.0);

        static const QSize imagesize(image_width, image_height);

        this->temp_image = destination_image.copy();
        QPainter painter(&temp_image);

        QRect target(QPoint(mouseX - delta_width, mouseY - delta_height), QPoint(mouseX + delta_width, mouseY + delta_height));
        QRect source(QPoint(mouseX - delta_width, mouseY - delta_height), QPoint(mouseX + delta_width, mouseY + delta_height));

        painter.drawImage(target, source_image, source);
        painter.end();
        temp_image.scaled(ui->resultImage->width(), ui->resultImage->height(),Qt::KeepAspectRatio);
        QPixmap result = QPixmap::fromImage(temp_image);
        //set and get the image position
        double topleft_x, topleft_y, bottomRight_x, bottomRight_y;
        topleft_x = static_cast<double>(ui->resultImage->xAxis->pixelToCoord(mouseX - delta_width));
        topleft_y = static_cast<double>(ui->resultImage->yAxis->pixelToCoord(mouseY - delta_height));
        bottomRight_x = static_cast<double>(ui->resultImage->xAxis->pixelToCoord(mouseX + delta_width));
        bottomRight_y = static_cast<double>(ui->resultImage->yAxis->pixelToCoord(mouseY + delta_height));
        qDebug() << topleft_x << topleft_y << bottomRight_x << bottomRight_y << mouseX << mouseY;

        image_pixmap_origin->setPixmap(result);
        image_pixmap_origin->topLeft->setCoords(topleft_x, topleft_y);
        image_pixmap_origin->bottomRight->setCoords(bottomRight_x, bottomRight_y);
        ui->resultImage->replot();
    }
}

*/
