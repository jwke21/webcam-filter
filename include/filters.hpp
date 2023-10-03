/**
 * Jake Van Meter
 * Fall 2023
 * CS 5330
*/
#include <opencv2/core.hpp>


#ifndef FILTERS_HPP

int alt_grayscale(const cv::Mat& src, cv::Mat& dst);

int blur5x5(const cv::Mat& src, cv::Mat& dst);

int sobelX3x3(const cv::Mat& src, cv::Mat& dst);

int sobelY3x3(const cv::Mat& src, cv::Mat& dst);

int magnitude(const cv::Mat& sx, const cv::Mat& sy, cv::Mat& dst);

int blurQuantize(const cv::Mat& src, cv::Mat& dst, const int levels = 15);

int cartoon(const cv::Mat& src, cv::Mat& dst, const int magThreshold = 10);

/**
 * https://docs.opencv.org/4.x/d3/dc1/tutorial_basic_linear_transform.html. 
 * The brightness is associated with β in the equation: g(i, j) = α*f(i, j) + β.
 * Contrast is associated with the α.
*/
int brightness(const cv::Mat& src, cv::Mat& dst, const int alpha = 1.3, const int beta = 10);

int sepia(const cv::Mat& src, cv::Mat& dst);

enum class FilterType {
    DEFAULT, // no filter (color)
    GRAYSCALE, // greyscale using cv::cvtColor function
    ALT_GRAYSCALE, // greyscale using alt_greyscale function
    BLUR5x5, // gaussian blur with a 5x5 filter
    SOBEL_X, // horizontal sobel filter
    SOBEL_Y, // vertical sobel filter 
    GRAD_MAG, // gradient magnitude
    BLUR_QUANTIZE, // blur and quantize
    CARTOON, // cartoon filter
    BRIGHTNESS, // brightness filter
    SEPIA, // sepia filter
};

#endif // FILTERS_HPP