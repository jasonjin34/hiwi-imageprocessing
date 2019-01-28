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
    cv::boxFilter(input,output,input.depth(),cv::Size(3,3));
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

void threshold_simple(cv::Mat& input, cv::Mat& output)
{
    // TODO
}

void threshold_adaptive_mean(cv::Mat& input, cv::Mat& output)
{
    // TODO
}

void threshold_adaptive_gauss(cv::Mat& input, cv::Mat& output)
{
    // TODO
}

void threshold_otsu(cv::Mat& input, cv::Mat& output)
{
    // TODO
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

void histMathing(cv::Mat& input, cv::Mat& output)
{
    //TODO
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
