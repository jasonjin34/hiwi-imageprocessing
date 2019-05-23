#include "composition.h"
#include "ui_composition.h"
#include <QDebug>

Composition::Composition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Composition)
{
    ui->setupUi(this);

    // set graphic scene
    ui->resultImage->setMouseTracking(true);
    ui->resultImage->xAxis->setVisible(false);
    ui->resultImage->yAxis->setVisible(false);
    ui->resultImage->setBackgroundScaled(true);
    ui->resultImage->setBackgroundScaledMode(Qt::KeepAspectRatio);

    // set the mouse tracting rectangle
    // this->rect = new QCPItemRect(ui->resultImage);
    // rect->setPen(QPen(Qt::blue));

    // add combobox items
    ui->composition_type->addItem("Layer1");
    ui->composition_type->addItem("Layer2");
    ui->composition_type->addItem("Layer3");

    // slot signal
    connect(ui->loadImageButton, SIGNAL(click()),this, SLOT(on_loadImageButton_clicked()));
    connect(ui->resultImage,SIGNAL(mouseMove(QMouseEvent*)),this, SLOT(mouseMove(QMouseEvent*)));
    connect(ui->composition_type,SIGNAL(activated(int)), this, SLOT(composeImage()));
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
        ui->resultImage->setBackground(start_image);
        ui->resultImage->resize(width, height);
        this->resize(static_cast<int>(width*6.0/5.0),height - 40);
    }
    else {
        this->sourceImageVector.clear();
    }

    //set the origin source
    composeImage();
}

void Composition::mouseMove(QMouseEvent* event)
{
    // origin point x = 0, y = 0 is ont the top left corner
    QPoint point = event->pos();
    this->mousePosition = point;
    /*
    double x_pos = ui->resultImage->xAxis->pixelToCoord(point.x());
    double y_pos = ui->resultImage->yAxis->pixelToCoord(point.y());
    rect->topLeft->setCoords(x_pos - 0.5, y_pos + 0.5);
    rect->bottomRight->setCoords(x_pos + 0.5, y_pos - 0.5);
    */
    this->ui->resultImage->replot();
    on_TestButton_clicked();
}

void Composition::composeImage()
{
    if (!this->sourceImageVector.isEmpty())
    {
        QString source_layer = ui->composition_type->currentText();
        if(source_layer == "Layer3")
        {
            this->source_image = sourceImageVector[2].copy();
        }
        else if (source_layer == "Layer2")
        {
            this->source_image = sourceImageVector[1].copy();
        }
        else if (source_layer == "Layer1")
        {
            this->source_image = sourceImageVector[0].copy();
        }
    }
}


void Composition::on_TestButton_clicked()
{
    if(!this->sourceImageVector.isEmpty())
    {
        int mouseX = this->mousePosition.x();
        int mouseY = this->mousePosition.y();
        int image_width = sourceImageVector[0].width();
        int image_height = sourceImageVector[0].height();
        static const QSize imagesize(image_width, image_height);
        //this->temp_image = QImage(imagesize, QImage::Format_ARGB32_Premultiplied);
        this->temp_image = source_image.copy();
        QPainter painter(&temp_image);
        int delta_width, delta_height;
        delta_width = static_cast<int>(image_width * 0.5 / 5.0);
        delta_height = static_cast<int>(image_height * 0.5 / 5.0);
        //qDebug() << delta_width << delta_height << mouseX << mouseY;

        QRect target(QPoint(mouseX - delta_width, mouseY - delta_height), QPoint(mouseX + delta_width, mouseY + delta_height));
        QRect source(QPoint(mouseX - delta_width, mouseY - delta_height), QPoint(mouseX + delta_width, mouseY + delta_height));
        qDebug() << target << source;

        painter.drawImage(target, sourceImageVector[0], source);
        painter.end();
        temp_image.scaled(ui->resultImage->width(), ui->resultImage->height(),Qt::KeepAspectRatio);
        QPixmap result = QPixmap::fromImage(temp_image);
        ui->resultImage->setBackground(result);
    }
}
