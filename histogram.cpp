#include "histogram.h"

Histogram::Histogram()
{

}

Histogram::Histogram(std::string src_string)
{
    //load image
    src = cv::imread(src_string,-1);

    //check load error
    if(src.empty()){
        qDebug() << "the pic can not load";
    } else qDebug() << "load success";
}

cv::Mat Histogram::getEqualizedGray()
{
    return this->hisEqua_src_gray;
}

void Histogram::histoEqualizGray(){
   cv::Mat temp_image;
   cv::cvtColor(src,temp_image,cv::COLOR_BGR2GRAY);
   cv::Ptr<cv::CLAHE> clane = cv::createCLAHE(30,cv::Size(2,2));
   clane->apply(temp_image, hisEqua_src_gray);
}

void Histogram::histoEqualiz(){
    std::vector<cv::Mat> hsvChannel(3),hsvchannel_temp(3),output_hsv;
    cv::Mat temp_src,output,output_converted;
    std::vector<cv::Mat> channel;

    //use temp to get the color transfered image
    cv::cvtColor(src,temp_src,CV_BGR2HSV);
    cv::split(temp_src,hsvChannel);
    cv::split(temp_src,hsvchannel_temp);
    cv::equalizeHist(hsvChannel[2],hsvchannel_temp[2]);

    //push the converted and original channels and merge into the original image
    output_hsv.push_back(hsvChannel[0]);
    output_hsv.push_back(hsvChannel[1]);
    output_hsv.push_back(hsvchannel_temp[2]);

    cv::merge(output_hsv,output);
    cv::cvtColor(output,output_converted,CV_HSV2BGR);
    //cv::imshow("histogram equalized color image -- BGR format",output_converted); // testing result
    hisEqua_src_color = output_converted;
}

cv::Mat Histogram::getEqualizedColor(){
    return this->hisEqua_src_color;
}
