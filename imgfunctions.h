#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>

namespace iaw
{

void noise_blur(cv::Mat& input, cv::Mat& output, int w = 2, int h = 2);

void noise_average(cv::Mat& input, cv::Mat& output);

void noise_gauss(cv::Mat& input, cv::Mat& output);

void noise_median(cv::Mat& input, cv::Mat& output);

void noise_bilateral(cv::Mat& input, cv::Mat& output);

void threshold_simple(cv::Mat& input, cv::Mat& output);

void threshold_adaptive_mean(cv::Mat& input, cv::Mat& output);

void threshold_adaptive_gauss(cv::Mat& input, cv::Mat& output);

void threshold_otsu(cv::Mat& input, cv::Mat& output);

void histEqual(cv::Mat& input, cv::Mat& output);

void histMathing(cv::Mat& input, cv::Mat& output);

void histClahe(cv::Mat& input, cv::Mat& output);

void cannyEdge(cv::Mat& input, cv::Mat& output, int min, bool merge);

}
