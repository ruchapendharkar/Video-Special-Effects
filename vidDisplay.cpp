//
//  VidDisplay.cpp
//  Project1
//
//  Created by Rucha Pendharkar on 1/31/24
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "filters.cpp"
#include <opencv2/opencv.hpp>
#include "faceDetect.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    cv::VideoCapture *capdev;

    // open the video device
    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return (-1);
    }

    // get some properties of the image
    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
                  (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    cv::namedWindow("Video", 1); // identifies a window
    cv::Mat frame, filter, edges;
    int count = -1;

    // List of flags for toggling off different modes
    bool greyscaleMode = false;
    bool CustomgreyscaleMode = false;
    bool SepiaMode = false;
    bool BlurMode = false;
    bool SobelXMode = false; 
    bool SobelYMode = false;  
    bool MagnitudeMode = false;
    bool BlurQuantize = false;
    bool DetectFaces = false;
    bool NegativeMode = false;
    bool BlurFacesMode = false;
    bool SparklesMode = false;
    bool SketchMode = false;

    for (;;) {
        *capdev >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }

        // Reset filter for each iteration
        filter = frame.clone();

        if (greyscaleMode) {
            // Applying greyscale filter to each frame
            cv::cvtColor(frame, filter, cv::COLOR_BGR2GRAY);
        } else if (CustomgreyscaleMode) {
            // Applying custom greyscale filter to each frame
            greyscale(frame, filter);
        } else if (SepiaMode) {
            // Applying Sepia filter to each frame
            Sepia(frame, filter);
        } else if (BlurMode) {
            // Applying Blur5x5_2 filter to each frame
            blur5x5_2(frame, filter);
        } else if (SobelXMode) {
            // Applying SobelX filter to each frame
            sobelX3x3(frame, filter);
            cv::convertScaleAbs(filter, filter);
        } else if (SobelYMode) {
            // Applying SobelX filter to each frame
            sobelY3x3(frame, filter);
            cv::convertScaleAbs(filter, filter);
        } else if (MagnitudeMode) {
            // Applying Gradient Magnitude Mode
            cv::Mat sobx, soby;
            sobelX3x3(frame, sobx);
            sobelY3x3(frame, soby);
            magnitude(sobx, soby, filter);
            cv::convertScaleAbs(filter, filter);
        } else if (BlurQuantize) {
            // Applying Blur Quantize Mode
            blurQuantize(frame, filter, 10);
        } else if (DetectFaces) {
            // Call detectFaces to find faces in the frame
            cv::Mat greyFrame;
            std::vector<cv::Rect> faces;
            cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY);
            detectFaces(greyFrame, faces);

            // Call drawBoxes to draw rectangles around the detected faces
            int minWidth = 30;
            float scale = 1.0;
            drawBoxes(filter, faces, minWidth, scale);
        } else if (NegativeMode) {
            // Applying Negative Filter - single step pixel modification
            Negative(frame, filter);
        } else if (BlurFacesMode) {
            cv::Mat greyFrame;
            std::vector<cv::Rect> faces;
            cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY);
            detectFaces(greyFrame, faces);
            int minWidth = 30;
            float scale = 1.0;
            drawBoxes(frame, faces, minWidth, scale);
            blurOutsideFaces(frame, faces, filter);

        } else if (SparklesMode) {
            // Applying Canny edge detection to find strong edges
            cv::Canny(frame, edges, 50, 150);
            // Add sparkles to the strong edges
            addSparkles(edges, filter);

        } else if (SketchMode){
            Sketch(frame, filter);
        }

        // Display the filtered frame
        cv::imshow("Video", filter);

        // see if there is a waiting keystroke
        char key = cv::waitKey(10);
        if (key == 'q') {
            break;
        } else if (key == 's') {
            // Save the image frame
            ++count;
            cv::imwrite("/home/rucha/CS5330/Project1/captured_frame_" + to_string(count) + ".jpeg", filter);
            cout << "Image saved as captured_frame_" << count << ".jpeg" << endl;
        } else if (key == 'g') {
            // Toggle greyscale mode on/off
            greyscaleMode = !greyscaleMode;
        } else if (key == 'h') {
            // Toggle custom greyscale mode on/off
            CustomgreyscaleMode = !CustomgreyscaleMode;
        } else if (key == 'p') {
            // Toggle Sepia mode on/off
            SepiaMode = !SepiaMode;
        } else if (key == 'b') {
            // Toggle blur mode on/off
            BlurMode = !BlurMode;
        } else if (key == 'x') {
            // SobelX mode on/off
            SobelXMode = !SobelXMode;
        } else if (key == 'y') {
            // SobelY mode on/off
            SobelYMode = !SobelYMode;
        } else if (key == 'm') {
            // Magnitude mode on/off
            MagnitudeMode = !MagnitudeMode;
        } else if (key == 'l') {
            // Blur Quantize mode on/off
            BlurQuantize = !BlurQuantize;
        } else if (key == 'f') {
            // Face Detection mode on/off
            DetectFaces = !DetectFaces;
        } else if (key == 'n') {
            // Negative mode on/off
            NegativeMode = !NegativeMode;
        } else if (key == 'r') {
            // Blur Faces mode on/off
            BlurFacesMode = !BlurFacesMode;
        } else if (key == 'e'){
            //Add Sparkles to strong edges mode on/off
            SparklesMode = !SparklesMode;
        } else if (key == 'w'){
            //Extension - sketch mode on /off
            SketchMode = !SketchMode;
        }
    }

    delete capdev;
    return (0);
}
