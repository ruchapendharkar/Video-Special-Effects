//
//  imgDisplay.cpp
//  Project1
//
//  Created by Rucha Pendharkar on 1/31/24.
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace std;

int main(int argc, char *argv[]) {
    
    // read the image
    
    std::string img_path = cv::samples::findFile("TurtleBot3_Burger.png");
    cv:: Mat img = imread(img_path, cv::IMREAD_COLOR);
    
    
    // check if image exists
    
    if(img.empty())
    {
        cout << "NO IMAGE !!";
        return 1;
    }
    
    // name window for dispay
    
    cv::namedWindow("Image-1", cv::WINDOW_NORMAL);
    
    imshow("Image-1", img);
    
    // Adding Key Press functionality
    
    while (true) {
            char key = cv::waitKey(10);
            // quit the window
            if (key == 'q') {
                break;
            }
            // resize the window
            if(key == 's')
            {
                cv::resizeWindow("Image-1", 600, 600);
                imshow("Image Display", img);
            }
                }

        cv::destroyAllWindows();
    
    return 0;
    
    
    
}
    
