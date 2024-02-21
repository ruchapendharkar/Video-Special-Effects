//
//  filters.cpp
//  Project1
//
//  Created by Rucha Pendharkar on 1/31/24
//


#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;

//Alternative GreyScale Filter
int greyscale( cv::Mat &src, cv::Mat &dst) {
    dst.create(src.size(), CV_8UC1);
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            dst.at<uchar>(i, j) = (src.at<cv::Vec3b>(i, j)[0] + src.at<cv::Vec3b>(i, j)[1] + src.at<cv::Vec3b>(i, j)[2]) / 3;
        }
    }
    return 0;
}


//Sepia Filter
int Sepia(cv::Mat &src, cv::Mat &dst){
    cv::cvtColor(src, dst, cv::COLOR_BGR2RGB);
    cv::transform(dst,dst,cv::Matx33f(0.393,0.769,0.189,0.349,0.686,0.168,0.272,0.534,0.131));
    cv::cvtColor(dst,dst,cv::COLOR_RGB2BGR);
    return 0;
    
}

//5x5 Blur Filter - First Implementation
int blur5x5_1(cv::Mat &src, cv::Mat &dst) {
    if (src.empty()) {
        cout << "Input image is empty." << endl;
        return -1;
    }

    // Ensure the destination matrix has the same size as the source
    if (dst.size() != src.size() || dst.type() != src.type()) {
        dst.create(src.size(), src.type());
    }

    // Copy the source image to the destination image
    src.copyTo(dst);

    // Define the 5x5 Gaussian kernel
    int kernel[5][5] = {{1, 2, 4, 2, 1},
                        {2, 4, 8, 4, 2},
                        {4, 8, 16, 8, 4},
                        {2, 4, 8, 4, 2},
                        {1, 2, 4, 2, 1}};

    // Get the image dimensions
    int rows = src.rows;
    int cols = src.cols;

    // Apply the blur filter to each color channel separately
    for (int y = 2; y < rows - 2; ++y) {
        for (int x = 2; x < cols - 2; ++x) {
            for (int c = 0; c < src.channels(); ++c) {
                int sum = 0;

                // Convolution with the kernel
                for (int i = -2; i <= 2; ++i) {
                    for (int j = -2; j <= 2; ++j) {
                        sum += kernel[i + 2][j + 2] * src.at<cv::Vec3b>(y + i, x + j)[c];
                    }
                }

                // Normalize the result and update the destination image
                dst.at<cv::Vec3b>(y, x)[c] = static_cast<uchar>(sum / 76);
            }
        }
    }

    return 0;
    
}

//5x5 Blur Filter Second Implementation

int blur5x5_2(cv::Mat &src, cv::Mat &dst) {
    if (src.empty()) {
        cout << "Input image is empty." << endl;
        return -1;
    }

    cv::Mat temp;
    temp.create(src.size(), src.type());
    dst.create(src.size(), src.type());
    cv::Vec3i resultRow = {0, 0, 0};
    cv::Vec3i resultCol = {0, 0, 0};

    // 5x1 filter (vertical filter)
    const float kernelVertical[5] = {0.1, 0.2, 0.4, 0.2, 0.1};

    // Loop through the image rows (excluding the outer two rows)
    for (int i = 2; i < src.rows - 2; i++) {
        // Get pointers to the current row in source and destination images
        const cv::Vec3b* srcRow = src.ptr<cv::Vec3b>(i);
        cv::Vec3b* tempRow = temp.ptr<cv::Vec3b>(i);

        // Loop through the image columns (excluding the outer two columns)
        for (int j = 2; j < src.cols - 2; j++) {
            // Initialize the result vector for each color channel
            cv::Vec3i result = {0, 0, 0};

            // Loop through the filter coefficients
            for (int k = -2; k <= 2; k++) {
                // Get pointers to the pixel values in the current filter window
                const cv::Vec3b* pixel = srcRow + j;
                
                // Update the result vector for each color channel
                for (int c = 0; c < 3; c++) {
                    result[c] += (*pixel)[c] * kernelVertical[k + 2];
                    pixel += src.cols;  // Move to the next row
                }
            }

            // Store the result vector in the destination image
            tempRow[j] = cv::Vec3b(result);
        }
    }

    // 1x5 filter (horizontal filter)
    const float kernelHorizontal[5] = {0.1, 0.2, 0.4, 0.2, 0.1};

    // Loop through the image rows (excluding the outer two rows)
    for (int i = 2; i < src.rows - 2; i++) {
        // Get pointers to the current row in the temporary and destination images
        const cv::Vec3b* tempRow = temp.ptr<cv::Vec3b>(i);
        cv::Vec3b* dstRow = dst.ptr<cv::Vec3b>(i);

        // Loop through the image columns (excluding the outer two columns)
        for (int j = 2; j < src.cols - 2; j++) {
            // Initialize the result vector for each color channel
            cv::Vec3i result = {0, 0, 0};

            // Loop through the filter coefficients
            for (int k = -2; k <= 2; k++) {
                // Update the result vector for each color channel
                for (int c = 0; c < 3; c++) {
                    result[c] += tempRow[j + k][c] * kernelHorizontal[k + 2];
                }
            }

            // Store the result vector in the destination image
            dstRow[j] = cv::Vec3b(result);
        }
    }

    return 0;
}

//3x3 Sobel X Filter

int sobelX3x3(cv::Mat& src, cv::Mat& dst) {
    cv::Mat temp;
    temp.create(src.size(), CV_16SC3);
    dst.create(src.size(), CV_16SC3);
    //cv::Vec3s result = {0, 0, 0};
    int result = 0;
    
    // 3x1 filter
    for (int i = 1; i < src.rows - 1; i++) {
        for (int j = 1; j < src.cols - 1; j++) {
            for (int c = 0; c < 3; c++) {
                 result = src.at<cv::Vec3b>(i - 1, j - 1)[c] * -1 + src.at<cv::Vec3b>(i - 1, j)[c] * 0 + src.at<cv::Vec3b>(i - 1, j + 1)[c];
                 temp.at<cv::Vec3s>(i, j)[c] = result;
            }
        }
    }

    // 1x3 filter
    for (int i = 1; i < temp.rows - 1; i++) {
        for (int j = 1; j < temp.cols - 1; j++) {
            for (int c = 0; c < 3; c++) {
                result = temp.at<cv::Vec3s>(i - 1, j - 1)[c] + temp.at<cv::Vec3s>(i, j - 1)[c] * 2 + temp.at<cv::Vec3s>(i + 1, j - 1)[c];
                dst.at<cv::Vec3s>(i, j)[c] = result;
            }
        }
    }
    return 0;
}

//3x3 Sobel Y Filter


int sobelY3x3( cv::Mat &src, cv::Mat &dst ){
    //Multiply [1 2 1] by
    //[1
    // 0
    //-1]
    
    cv::Mat temp;
    temp.create(src.size(), CV_16SC3);
    dst.create(src.size(), CV_16SC3);
    cv::Vec3s result = {0, 0, 0};
 
    // 3x1 filter
    for (int i = 1; i < src.rows-1; i++) {
        for (int j = 1; j < src.cols-1; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = src.at<cv::Vec3b>(i-1, j-1)[c] + src.at<cv::Vec3b>(i-1, j)[c]*2
                + src.at<cv::Vec3b>(i-1, j+1)[c];
            }
            temp.at<cv::Vec3s>(i,j) = result;
        }
    }

    // 1x3 filter
    for (int i = 1; i < temp.rows-1; i++) {
        for (int j = 1; j < temp.cols-1; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = temp.at<cv::Vec3s>(i-1, j-1)[c] + temp.at<cv::Vec3s>(i, j-1)[c]*0
                + temp.at<cv::Vec3s>(i+1, j-1)[c]*-1 ;
            }
            dst.at<cv::Vec3s>(i,j) = result;
        }
    }
    return 0;
}

int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst ){

    dst.create(sx.size(), sx.type());
    cv::Vec3s result = {0, 0, 0};
    for (int i = 0; i < sx.rows; i++) {
        for (int j = 0; j < sx.cols; j++){
  
            for (int c = 0; c < 3; c++) {
   
                result[c] = sqrt(pow(sx.at<cv::Vec3s>(i, j)[c], 2) + pow(sy.at<cv::Vec3s>(i, j)[c], 2));
            }
            dst.at<cv::Vec3s>(i,j) = result;
        }
    }
    return 0;
}

//Blur Quantize Filter
int blurQuantize(cv::Mat &src, cv::Mat &dst, int levels) {
    cv::Mat temp;
    temp.create(src.size(), src.type());
    dst.create(src.size(), src.type());
    
    blur5x5_2(src, temp);
    
    int b = 255 / levels;
    cv::Vec3i result = {0, 0, 0};

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            for (int c = 0; c < 3; c++) {
                result[c] = temp.at<cv::Vec3b>(i, j)[c] / b;
                result[c] = result[c] * b;
            }
            dst.at<cv::Vec3b>(i, j) = cv::Vec3b(result[0], result[1], result[2]);
        }
    }
    return 0;
}


//Converts Image to Negative
int Negative(cv::Mat &src, cv::Mat &dst){
    dst = 255 -src;
    return 0;
    
}

int blurOutsideFaces(cv::Mat &src, std::vector<cv::Rect> &faces, cv::Mat &dst) {
    // Create a mask to represent the regions outside of faces
    cv::Mat mask(src.size(), CV_8U, cv::Scalar(255));

    for (const auto &face : faces) {
        cv::rectangle(mask, face, cv::Scalar(0), cv::FILLED);
    }

    // Apply the 5x5 blur to the original image
    cv::Mat blurredImage;
    blur5x5_2(src, blurredImage);

    // Create a copy of the original image
    dst = src.clone();

    // Set the pixels outside of faces to the corresponding pixels in the blurred image
    dst.setTo(0, mask);
    blurredImage.copyTo(dst, mask);

    return 0;
}

int addSparkles(cv::Mat &edges, cv::Mat &dst) {
    CV_Assert(edges.type() == CV_8UC1);

    // Define sparkle color 
    cv::Scalar sparkleColor(255, 255, 255); // White sparkles

    // Iterate through the image and add sparkles where there are strong edges
    for (int y = 0; y < edges.rows; ++y) {
        for (int x = 0; x < edges.cols; ++x) {
            if (edges.at<uchar>(y, x) > 0) {
                // Add sparkle at the position (x, y)
                cv::rectangle(dst, cv::Point(x, y), cv::Point(x + 2, y + 2), sparkleColor, -1);
            }
        }
    }
    return 0;
}

//Sketch  - Extension 

int Sketch(cv::Mat &src, cv::Mat &dst)
{
    greyscale(src, dst);
    blur5x5_2(dst, dst);
    Laplacian(dst, dst, -1, 5);
    dst = 255 - dst;
    threshold(dst, dst, 150, 255, cv::THRESH_BINARY);
    return 0;
}

