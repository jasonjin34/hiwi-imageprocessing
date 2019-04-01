#include "imgfunctions.h"
#include <QDebug>

namespace iaw
{
void imageHist(cv::Mat input, int histogram[])
{
    for(int i = 0; i < 256; i++){
        histogram[i] = 0;
    }
    for(int i = 0; i < input.rows; i ++)
        for( int x = 0; x < input.cols; x++)
            histogram[static_cast<int>(input.at<uchar>(i,x))]++;
}

void cumhist(int hist[], int cumhist[])
{
    cumhist[0] = hist[0];
    for(int i = 1;i < 256; i++){
        cumhist[i] = cumhist[i-1] + hist[i];
    }
}


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

//histogram equalization for level adjustment gui
/**
 * @brief histEqual_leveladj
 * @param input
 * @param output
 * @param min minimal pixel intensity value
 * @param middle set up the two different sub-range, each range will be assigned 50% pixel intensity values
 * @param max maxiaml pixel intensity value
 * @param equalization
 */
void histEqual_leveladj(cv::Mat& input,cv::Mat& output, int min,double middle, int max, bool equalization, bool equldivide)
{
    int range = max - min;
    int histogram[256];

    int range_upper = static_cast<int>(range*middle);
    int range_under = range - range_upper;

    imageHist(input,histogram);

    int size = input.rows*input.cols;
    float alpha = static_cast<float>(range)/size;

    float alpha_upper = static_cast<float>(range_upper*2.0/size);
    float alpha_under = static_cast<float>(range_under*2.0/size);

    if(equalization)
    {
        if(equldivide == false)
        {
            int cumhistogram[256];
            cumhist(histogram,cumhistogram);

            //Scale the histogram with equalization
            int scale_his[256];
            for(int i= 0; i <256; i++) scale_his[i] = 0;
            for(int i = 0; i < 255; i++){
                scale_his[i] = cvRound(static_cast<float>(cumhistogram[i])*alpha + min);
            }

            cv::Mat temp = input.clone();
            for(int y = 0; y < input.rows ; y++)
                for(int x = 0; x < input.cols; x++)
                    temp.at<uchar>(y,x) = cv::saturate_cast<uchar>(scale_his[input.at<uchar>(y,x)]);
            output = temp;
        }
        else
        {
            int cumhistogram[256];
            cumhist(histogram,cumhistogram);

            //Scale the histogram with equalization
            int scale_his[256];
            for(int i= 0; i <256; i++) scale_his[i] = 0;
            for(int i = 0; i < 128; i++){
                scale_his[i] = cvRound(static_cast<float>(cumhistogram[i])*alpha_upper + min);
            }

            for(int i = 128; i < 255; i++){
               scale_his[i] = cvRound(static_cast<float>((cumhistogram[i])-cumhistogram[127])*alpha_under + min + range_upper);
            }

            cv::Mat temp = input.clone();
            for(int y = 0; y < input.rows ; y++)
                for(int x = 0; x < input.cols; x++)
                    temp.at<uchar>(y,x) = cv::saturate_cast<uchar>(scale_his[input.at<uchar>(y,x)]);
            output = temp;
        }

    }
    else
    {
        //Scale the histogram pixel value without equalization
        int scale_his[256];
        for(int i= 0; i <256; i++) scale_his[i] = 0; // Scale  vector init
        for(int i = 0; i < 127; i++){
            scale_his[i] = cvRound(static_cast<double>(2.0*i*range_upper/255.0)+min); // set up 50 pixel in the upper range
        }
        for(int i = 127; i < 255; i++){
            scale_his[i] = cvRound(static_cast<double>(2.0*(i-127)*range_under/255.0)+range_upper+min);
        }

        cv::Mat temp = input.clone();
        for(int y = 0; y < input.rows ; y++)
            for(int x = 0; x < input.cols; x++)
                temp.at<uchar>(y,x) = cv::saturate_cast<uchar>(scale_his[input.at<uchar>(y,x)]);
        output = temp;
    }
}


void histClahe(cv::Mat& input, cv::Mat& output)
{
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(30,cv::Size(2,2));
    clahe->apply(input, output);
}

void histDraw(cv::Mat& input, cv::Mat& output, int min, int max){
    cv::Mat hist;

    int histSize = 256;
    float range[] = {0, 256};
    const int channels[] = {0};
    const float* histRange = { range };
    cv::calcHist(&input,1,channels,cv::Mat(),hist,1,&histSize,&histRange,true,true);

    int bin_w = cvRound(static_cast<double>(600/histSize));

    cv::Mat histImage(500,600,CV_8UC1, cv::Scalar(255)); //para high, width, type
    cv::normalize(hist,hist,0,hist.rows,cv::NORM_MINMAX,-1,cv::Mat());

    for( int i = min; i < max; i++ )
    {
        cv::line( histImage,
                  cv::Point(bin_w*(i-1), 500 - cvRound(hist.at<float>(i-1))) ,
                  cv::Point(bin_w*(i-1), 500),
                  cv::Scalar(0), 2, 8, 0  );
    }
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

void contrastAdjustment(cv::Mat& input, cv::Mat& output, double alpha, double beta)
{
    output = input.clone();
    for(int x = 0; x < input.cols; x++)
        for(int y = 0; y < input.rows; y++)
            output.at<uchar>(y,x) = cv::saturate_cast<uchar>(alpha*static_cast<double>(input.at<uchar>(y,x)) + beta);
}



void contrastAdjInterpolation(cv::Mat &input, cv::Mat &output, QVector<double> alphavectorptr)
{
    output = input.clone();
    for(int x = 0; x < input.cols; x++)
    {
        for(int y = 0; y < input.rows; y++)
        {
            double temp = alphavectorptr.at(static_cast<int>(static_cast<double>(input.at<uchar>(y,x))));
            if(temp <= 255 || temp >= 0 )
            {
                output.at<uchar>(y,x) = cv::saturate_cast<uchar>(temp);
            }
        }
    }
}

bool imageStitching(std::vector<cv::Mat> &input, cv::Mat &output)
{
    cv::Stitcher::Mode mode = cv::Stitcher::PANORAMA;
    cv::Mat pano;
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(mode);
    cv::Stitcher::Status status = stitcher->stitch(input,pano);
    if(status != cv::Stitcher::OK){
        qDebug() << "Can't stitch images\n";
        return false;
    }
    output = pano;
    return true;
}

void loadImagevector(QStringList& inputfiles, std::vector<cv::Mat> &output)
{
    std::vector<cv::Mat> imageVector;
    for(int i = 0;i < inputfiles.size();++i){
        cv::Mat temp_image = cv::imread(inputfiles[i].toStdString(),1);
        if(!temp_image.empty()){
            imageVector.push_back(temp_image);
        }
        else qDebug() << "loading error";
    }
    output = imageVector;
}

void threshold_imagestitching(cv::Mat &input, cv::Mat &output)
{
    cv::cvtColor(input,input,CV_BGR2GRAY);
    cv::threshold(input,output,0,240,CV_THRESH_BINARY );
}


}
