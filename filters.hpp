//
//  filters.hpp
//  Project1
//
//  Created by Rucha Pendharkar on 1/31/24
//

#ifndef FILTERS_H
#define FILTERS_H

#include <opencv2/core.hpp>

int greyscale(cv::Mat& src, cv::Mat& dst);
int Sepia(cv::Mat& src, cv::Mat& dst);
int blur5x5_1(cv::Mat& src, cv::Mat& dst);
int blur5x5_2(cv::Mat& src, cv::Mat& dst);
int sobelX3x3(cv::Mat& src, cv::Mat& dst);
int sobelY3x3(cv::Mat& src, cv::Mat& dst);
int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst);
int blurQuantize(cv::Mat& src, cv::Mat& dst, int levels);
int Negative(cv::Mat& src, cv::Mat& dst);
int blurOutsideFaces(cv::Mat &src, std::vector<cv::Rect> &faces, cv::Mat &dst);
int addSparkles(cv::Mat &edges, cv::Mat &dst);

//Extensions

int Sketch(cv::Mat& src, cv::Mat& dst);

#endif // FILTERS_H

