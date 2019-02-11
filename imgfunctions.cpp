#include "imgfunctions.h"
#include <QDebug>

namespace iaw
{

void noise_blur(cv::Mat& input, cv::Mat& output, int w, int h)
{
    cv::blur(input, output, cv::Size(w, h));
}

void noise_average(cv::Mat& input, cv::Mat& output)
{
    cv::boxFilter(input,output,input.depth(),cv::Size(5,5));
}

void noise_gauss(cv::Mat& input, cv::Mat& output)
{
    cv::GaussianBlur(input,output,cv::Size(5,5),0,0,cv::BORDER_DEFAULT);
}

void noise_median(cv::Mat& input, cv::Mat& output)
{
    cv::medianBlur(input,output, 5);
}

void noise_bilateral(cv::Mat& input, cv::Mat& output)
{
    cv::Mat temp;
    cv::bilateralFilter(input,temp,15,80,80,cv::BORDER_DEFAULT);
    output = temp;
}

void noise_fastNImeansDenoising(cv::Mat& input, cv::Mat& output)
{
    cv::fastNlMeansDenoising(input,output,10,7,21);
}

void threshold_simple(cv::Mat& input, cv::Mat& output)
{
    cv::threshold(input,output,170,255,0);
}

void threshold_adaptive_mean(cv::Mat& input, cv::Mat& output)
{
    cv::adaptiveThreshold(input,output,255,cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY,13,2);
}

void threshold_adaptive_gauss(cv::Mat& input, cv::Mat& output)
{
    cv::adaptiveThreshold(input,output,255,cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::THRESH_BINARY,13,2);
}

void threshold_otsu(cv::Mat& input, cv::Mat& output)
{
    cv::threshold(input,output,170,255,cv::THRESH_OTSU);
}

void histEqual(cv::Mat& input, cv::Mat& output)
{
    cv::equalizeHist(input, output);
}

void histClahe(cv::Mat& input, cv::Mat& output)
{
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(30,cv::Size(2,2));
    clahe->apply(input, output);
}

void histMathing(cv::Mat& input, cv::Mat& output) // need color image!
{

}

void histDraw(cv::Mat& input, cv::Mat& output){
    cv::Mat hist;

    int histSize = 256;
    float range[] = {0, 256};
    const int channels[] = {0};
    const float* histRange = { range };
    cv::calcHist(&input,1,channels,cv::Mat(),hist,1,&histSize,&histRange,true,true);

    int bin_w = cvRound(static_cast<double>(600/histSize));

    cv::Mat histImage(500,600,CV_8UC1, cv::Scalar(255)); //para high, width, type
    cv::normalize(hist,hist,0,hist.rows,cv::NORM_MINMAX,-1,cv::Mat());

    for( int i = 1; i < histSize; i++ )
    {
        cv::line( histImage,
                  cv::Point(bin_w*(i-1), 500 - cvRound(hist.at<float>(i-1))) ,
                  cv::Point(bin_w*(i-1), 500),
                  cv::Scalar(0), 2, 8, 0  );
    }
    // draw frame
    cv::line(histImage, cv::Point(0,0), cv::Point(600,0),cv::Scalar(0),1,8,0);
    cv::line(histImage, cv::Point(0,0), cv::Point(0,500),cv::Scalar(0),1,8,0);
    cv::line(histImage, cv::Point(599,0), cv::Point(599,500),cv::Scalar(0),1,8,0);
    cv::line(histImage, cv::Point(0,499), cv::Point(599,499),cv::Scalar(0),1,8,0);

    output = histImage;
}

void cannyEdge(cv::Mat& input, cv::Mat &output, int min, bool merge)
{
    int max = 2 * min;
    int kernel = 3;

    if(merge) {
        cv::Mat toMerge = input.clone();
        cv::Canny(input, output, min, max, kernel);
        cv::addWeighted(output, 0.3, toMerge, 0.7, 0, output);
    } else {
        cv::Canny(input, output, min, max, kernel);
    }
}

}
