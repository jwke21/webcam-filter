/**
 * Jake Van Meter
 * Fall 2023
 * CS 5330
*/
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

int main()
{
    std::string image_path = "cat.jpeg";
    cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);

    cv::imshow("Display window", img);

    std::cout << "Press 'q' to quit\n";
    while (int k = cv::waitKey(0) != 'q') {
    }

    return 0;
}
