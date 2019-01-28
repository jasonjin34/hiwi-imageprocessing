#ifndef CONVFUNKTION_H
#define CONVFUNKTION_H

#include <QImage>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>

QImage Mat2QImage(cv::Mat& src){
    cv::Mat temp;
    temp = src;
    cv::cvtColor(src,temp,CV_BGR2RGB);  //might need it in the future
    QImage dest(temp.data,temp.cols,temp.rows,static_cast<int>(temp.step),QImage::Format_RGB888);
    //dest.bits(); //enforce deep copy of QImage
    return dest;
}

QImage Mat2QImageGrayscale(cv::Mat& src){
    cv::Mat temp;
    temp = src;
    QImage dest(temp.data,temp.cols,temp.rows,static_cast<int>(temp.step),QImage::Format_Grayscale8);
    //dest.bits(); //enforce deep copy of QImage
    return dest;
}



#endif // CONVFUNKTION_H
