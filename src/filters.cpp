/**
 * Jake Van Meter
 * Fall 2023
 * CS 5330
*/
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <cmath>

#include "filters.hpp"


int alt_grayscale(const cv::Mat& src, cv::Mat& dst)
{
    if (src.empty()) {
        return -1;
    }

    dst = cv::Mat::zeros(src.size(), CV_8UC3);

    // convert to greyscale by setting all pixels to the green value
    for (int i = 0; i < src.rows; i++) {

        const cv::Vec3b* src_row = src.ptr<cv::Vec3b>(i);
        cv::Vec3b* dst_row = dst.ptr<cv::Vec3b>(i);

        for (int j=0; j < dst.cols; j++) {
            uchar green = src_row[j][1];
            dst_row[j][0] = green;
            dst_row[j][1] = green;
            dst_row[j][2] = green;
        }
    }

    return 0;
}

int blur_pixel(const cv::Mat& src,
               cv::Mat& dst,
               const int f[],
               const int radius,
               const int r,
               const int c,
               const bool horizontal,
               const bool normalize)
{
    if (src.empty()) {
        return -1;
    }
    
    int kernel_total = 0;
    int sum_b = 0, sum_g = 0, sum_r = 0;

    cv::Vec3b pixel;

    // apply filter horizontally
    if (horizontal) {
        for (int i=-radius; i <= radius; i++) {
            // ignore pixels beyond border
            if (c + i >= 0 && c + i < src.cols) {
                pixel = src.at<cv::Vec3b>(r, c + i);
                sum_b += pixel[0]*f[i + radius];
                sum_g += pixel[1]*f[i + radius];
                sum_r += pixel[2]*f[i + radius];
                kernel_total += f[i + radius];
            }
        }
    } else { // apply filter vertically
        for (int i=-radius; i <= radius; i++) {
            // ignore pixels beyond border
            if (r + i >= 0 && r + i < src.rows) {
                pixel = src.at<cv::Vec3b>(r + i, c);
                sum_b += pixel[0]*f[i + radius];
                sum_g += pixel[1]*f[i + radius];
                sum_r += pixel[2]*f[i + radius];
                kernel_total += f[i + radius];
            }
        }
    }

    // perform convolution for the BGR pixel and normalize if necessary
    if (normalize) {
        dst.at<cv::Vec3b>(r, c)[0] = sum_b/kernel_total;
        dst.at<cv::Vec3b>(r, c)[1] = sum_g/kernel_total;
        dst.at<cv::Vec3b>(r, c)[2] = sum_r/kernel_total;
    } else {
        dst.at<cv::Vec3b>(r, c)[0] = sum_b;
        dst.at<cv::Vec3b>(r, c)[1] = sum_g;
        dst.at<cv::Vec3b>(r, c)[2] = sum_r;
    }

    return 0;
}

int blur5x5(const cv::Mat& src, cv::Mat& dst)
{
    if (src.empty()) {
        return -1;
    }

    // seperable filter to be applied horizontally and vertically to each pixel
    const int filter[5] = {1, 2, 4, 2, 1};
    const int kernel_total = 10;

    cv::Mat tmp = cv::Mat::zeros(src.size(), CV_16SC3);

    // apply filter horizontally across image
    for (int i = 0; i < dst.rows; i++) {
        for (int j = 0; j < dst.cols; j++) {
            blur_pixel(src, tmp, filter, 2, i, j, true, true);
        }
    }
    // apply filter vertically across image
    for (int i = 0; i < dst.rows; i++) {
        for (int j = 0; j < dst.cols; j++) {
            blur_pixel(tmp, dst, filter, 2, i, j, false, true);
        }
    }

    return 0;
}

int sobelX3x3(const cv::Mat& src, cv::Mat& dst)
{
    if (src.empty()) {
        return -1;
    }

    const int filter_x[3] = {-1, 0, 1};
    const int filter_y[3] = {1, 2, 1};

    cv::Mat tmp = cv::Mat::zeros(src.size(), CV_16SC3);
    dst = cv::Mat::zeros(src.size(), CV_16SC3);

    int sum_b, sum_g, sum_r;

    for (int i = 1; i < src.rows - 1; i++) {
        for (int j = 1; j < src.cols - 1; j++) {
            sum_b = 0;
            sum_g = 0;
            sum_r = 0;
            for (int k = -1; k <= 1; k += 2) {
                sum_b += (short) src.at<cv::Vec3b>(i + k, j)[0]*filter_x[k + 1];
                sum_g += (short) src.at<cv::Vec3b>(i + k, j)[1]*filter_x[k + 1];
                sum_r += (short) src.at<cv::Vec3b>(i + k, j)[2]*filter_x[k + 1];
            }
            tmp.at<cv::Vec3s>(i, j)[0] = sum_b;
            tmp.at<cv::Vec3s>(i, j)[1] = sum_g;
            tmp.at<cv::Vec3s>(i, j)[2] = sum_r;
        }
    }
    for (int i = 1; i < tmp.rows - 1; i++) {
        for (int j = 1; j < tmp.cols - 1; j++) {
            sum_b = 0;
            sum_g = 0;
            sum_r = 0;
            for (int k = -1; k <= 1; k++) {
                sum_b += tmp.at<cv::Vec3s>(i, j + k)[0]*filter_y[k + 1];
                sum_g += tmp.at<cv::Vec3s>(i, j + k)[1]*filter_y[k + 1];
                sum_r += tmp.at<cv::Vec3s>(i, j + k)[2]*filter_y[k + 1];
            }
            dst.at<cv::Vec3s>(i, j)[0] = sum_b/4;
            dst.at<cv::Vec3s>(i, j)[1] = sum_g/4;
            dst.at<cv::Vec3s>(i, j)[2] = sum_r/4;
        }
    }

    return 0;
}

int sobelY3x3(const cv::Mat& src, cv::Mat& dst)
{
    if (src.empty()) {
        return -1;
    }

    const int filter_x[3] = {1, 2, 1};
    const int filter_y[3] = {1, 0, -1};

    cv::Mat tmp = cv::Mat::zeros(src.size(), CV_16SC3);
    dst = cv::Mat::zeros(src.size(), CV_16SC3);

    int sum_b, sum_g, sum_r;

    for (int i = 1; i < src.rows - 1; i++) {
        for (int j = 1; j < src.cols - 1; j++) {
            sum_b = 0;
            sum_g = 0;
            sum_r = 0;
            for (int k = -1; k <= 1; k += 2) {
                sum_b += (short) src.at<cv::Vec3b>(i + k, j)[0]*filter_x[k + 1];
                sum_g += (short) src.at<cv::Vec3b>(i + k, j)[1]*filter_x[k + 1];
                sum_r += (short) src.at<cv::Vec3b>(i + k, j)[2]*filter_x[k + 1];
            }
            tmp.at<cv::Vec3s>(i, j)[0] = sum_b;
            tmp.at<cv::Vec3s>(i, j)[1] = sum_g;
            tmp.at<cv::Vec3s>(i, j)[2] = sum_r;
        }
    }
    for (int i = 1; i < tmp.rows - 1; i++) {
        for (int j = 1; j < tmp.cols - 1; j++) {
            sum_b = 0;
            sum_g = 0;
            sum_r = 0;
            for (int k = -1; k <= 1; k++) {
                sum_b += tmp.at<cv::Vec3s>(i, j + k)[0]*filter_y[k + 1];
                sum_g += tmp.at<cv::Vec3s>(i, j + k)[1]*filter_y[k + 1];
                sum_r += tmp.at<cv::Vec3s>(i, j + k)[2]*filter_y[k + 1];
            }
            dst.at<cv::Vec3s>(i, j)[0] = sum_b/4;
            dst.at<cv::Vec3s>(i, j)[1] = sum_g/4;
            dst.at<cv::Vec3s>(i, j)[2] = sum_r/4;
        }
    }

    return 0;
}

int magnitude(const cv::Mat& sx, const cv::Mat& sy, cv::Mat& dst)
{
    if (sx.empty() || sy.empty()) {
        return -1;
    }

    dst = cv::Mat::zeros(sx.size(), CV_16SC3);

    for (int i = 0; i < sx.rows; i++) {

        cv::Vec3s* dst_rptr = dst.ptr<cv::Vec3s>(i);
        const cv::Vec3s* sx_rptr = sx.ptr<cv::Vec3s>(i);
        const cv::Vec3s* sy_rptr = sy.ptr<cv::Vec3s>(i);

        for (int j = 0; j < sx.cols; j++) {
            cv::Vec3s sx_val = sx_rptr[j];
            cv::Vec3s sy_val = sy_rptr[j];
            dst_rptr[j][0] = std::sqrt(sx_val[0]*sx_val[0] + sy_val[0]*sy_val[0]);
            dst_rptr[j][1] = std::sqrt(sx_val[1]*sx_val[1] + sy_val[1]*sy_val[1]);
            dst_rptr[j][2] = std::sqrt(sx_val[2]*sx_val[2] + sy_val[2]*sy_val[2]);
        }
    }

    return 0;
}

int blurQuantize(const cv::Mat& src, cv::Mat& dst, const int levels)
{
    if (src.empty()) {
        return -1;
    }

    cv::Mat tmp = cv::Mat::zeros(src.size(), CV_8UC3);

    // blur image
    blur5x5(src, tmp);

    // quantize image
    const int b = 255/levels; // bucket size

    for (int i = 0; i < tmp.rows; i++) {

        cv::Vec3b* tmp_rptr = tmp.ptr<cv::Vec3b>(i);
        cv::Vec3b* dst_rptr = dst.ptr<cv::Vec3b>(i);

        for (int j = 0; j < tmp.cols; j++) {
            // xt = x / b; xf = xt * b;
            dst_rptr[j][0] = (tmp_rptr[j][0]/b)*b;
            dst_rptr[j][1] = (tmp_rptr[j][1]/b)*b;
            dst_rptr[j][2] = (tmp_rptr[j][2]/b)*b;
        }
    }

    return 0;
}

int cartoon(const cv::Mat& src, cv::Mat& dst, const int magThreshold)
{
    if (src.empty()) {
        return -1;
    }

    cv::Mat sx;
    cv::Mat sy;
    cv::Mat grad = cv::Mat::zeros(src.size(), CV_8UC3);

    // calculate gradient magnitude
    sobelX3x3(src, sx);
    sobelY3x3(src, sy);
    magnitude(sx, sy, grad);

    // blur and quantize image
    blurQuantize(src, dst, 10);

    for (int i = 0; i < src.rows; i++) {

        const cv::Vec3b* src_rptr = src.ptr<cv::Vec3b>(i);
        const cv::Vec3s* grad_rptr = grad.ptr<cv::Vec3s>(i);
        cv::Vec3b* dst_rptr = dst.ptr<cv::Vec3b>(i);

        for (int j = 0; j < src.cols; j++) {
            // if gradient magnitude is greater than threshold, set pixel to black
            if (grad_rptr[j][0] > magThreshold || grad_rptr[j][1] > magThreshold || grad_rptr[j][2] > magThreshold) {
                dst_rptr[j][0] = 0;
                dst_rptr[j][1] = 0;
                dst_rptr[j][2] = 0;
            }
        }

    }

    return 0;
}

int brightness(const cv::Mat& src, cv::Mat& dst, const int alpha, const int beta)
{
    if (src.empty()) {
        return -1;
    }

    dst = cv::Mat::zeros(src.size(), CV_8UC3);

    for (int i = 0; i < src.rows; i++) {

        const cv::Vec3b* src_rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3b* dst_rptr = dst.ptr<cv::Vec3b>(i);

        for (int j = 0; j < src.cols; j++) {
            // saturate_cast<uchar>() is used to enforce validity (i.e. <= 255 and of type int)
            // https://docs.opencv.org/4.x/db/de0/group__core__utils.html#gab93126370b85fda2c8bfaf8c811faeaf
            dst_rptr[j][0] = cv::saturate_cast<uchar>(alpha*src_rptr[j][0] + beta);
            dst_rptr[j][1] = cv::saturate_cast<uchar>(alpha*src_rptr[j][1] + beta);
            dst_rptr[j][2] = cv::saturate_cast<uchar>(alpha*src_rptr[j][2] + beta);
        }
    }

    return 0;
}

int sepia(const cv::Mat& src, cv::Mat& dst)
{
    if (src.empty()) {
        return -1;
    }

    dst = cv::Mat::zeros(src.size(), CV_8UC3);

    // Sepia filter is a 1x1 filter
    // It is a transformation of the original pixel (a 1x3 vector) by
    // multiplying it by a 3x3 color vector to obtain a new rgb vector

    // filter (RGB):
    // R*0.393 0.769 0.189 R 
    // G*0.349 0.686 0.168 G
    // B*0.272 0.534 0.131 B

    // filter (BGR):
    float filter[3][3] = {
        {0.131, 0.534, 0.272}, // blue weights
        {0.168, 0.686, 0.349}, // green weights
        {0.189, 0.769, 0.393}  // red weights
    };

    for (int i = 0; i < src.rows; i++) {
        
        const cv::Vec3b* src_rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3b* dst_rptr = dst.ptr<cv::Vec3b>(i);

        for (int j = 0; j < src.cols; j++) {

            float sum_b = 0.0f, sum_g = 0.0f, sum_r = 0.0f;

            for (int k = 0; k <= 2; k++) {
                sum_b += src_rptr[j][0]*filter[0][k];
                sum_g += src_rptr[j][1]*filter[1][k];
                sum_r += src_rptr[j][2]*filter[2][k];
            }

            dst_rptr[j][0] = cv::saturate_cast<uchar>(sum_b);
            dst_rptr[j][1] = cv::saturate_cast<uchar>(sum_g);
            dst_rptr[j][2] = cv::saturate_cast<uchar>(sum_r);
        }

    }

    return 0;
}
