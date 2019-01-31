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

void histMathing(cv::Mat& input, cv::Mat& output)
{
    int histogram[256];
    for(int i = 0; i < 255;i++){
       histogram[i] = 0;
    }
    // calculate the no of pixels for each intensity values
    for(int y = 0; y < input.rows; y++)
        for(int x = 0; x < input.cols; x++)
            histogram[(int)input.at<uchar>(y,x)]++;

    for(int i = 0; i < 256; i++)
        std::cout<<histogram[i]<<" ";

    // draw the histograms
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound((double) hist_w/256);

    cv::Mat histImage(hist_h, hist_w, CV_8UC1, cv::Scalar(255, 255, 255));

    // find the maximum intensity element from histogram
    int max = histogram[0];
    for(int i = 1; i < 256; i++){
        if(max < histogram[i]){
            max = histogram[i];
        }
    }

    for(int i = 0; i < 255; i++){
        histogram[i] = (((double)histogram[i]/max)*histImage.rows);
    }

    // draw the intensity line for histogram
    for(int i = 0; i < 255; i++)
    {
        line(histImage,
             cv::Point(bin_w*(i), hist_h),
             cv::Point(bin_w*(i), hist_h - histogram[i]),
             cv::Scalar(0,0,0), 1, 8, 0);
    }
    cv::imshow("test histogram", histImage);

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
