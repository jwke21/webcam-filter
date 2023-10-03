/**
 * Jake Van Meter
 * Fall 2023
 * CS 5330
*/
#include <iostream> // for standard I/O

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include "filters.hpp"


int main(int argc, char* argv[])
{
	// open the video device
	cv::VideoCapture* capdev = new cv::VideoCapture(0);

	if (!capdev->isOpened()) {
		printf("Unable to open video device\n");
		return -1;
	}

	// get some properties of the image
	const cv::Size refS((int) capdev->get(cv::CAP_PROP_FRAME_WIDTH),
				  		(int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
	printf("Expected size: %d %d\n", refS.width, refS.height);

	cv::namedWindow("Video", 1); // identifies a window
	cv::Mat src_frame(refS, CV_8UC3);
	cv::Mat dst_frame;

	// matrices for sobel filters
	cv::Mat sx;
	cv::Mat sy;

	// video writer used for saving videos as .mp4
	// https://docs.opencv.org/4.x/dd/d9e/classcv_1_1VideoWriter.html
	cv::VideoWriter vid_writer;
	// fourcc code for mpv4
	// https://docs.opencv.org/4.x/dd/d9e/classcv_1_1VideoWriter.html#afec93f94dc6c0b3e28f4dd153bc5a7f0
	const int fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');

	// brightness level that will be adjusted whenever user presses brightness key
	int brightness_level = 20;

	// variable tracking the enabled filter type
	FilterType fil = FilterType::DEFAULT;

	for (;;) {
		*capdev >> src_frame; // get a new frame from the camera, treat as a stream
		if (src_frame.empty()) {
			printf("frame is empty\n");
			break;
		}

		// check for which filter is enabled
		switch(fil) {
			case FilterType::GRAYSCALE:
				cv::cvtColor(src_frame, dst_frame, cv::COLOR_BGR2GRAY);
				break;
			case FilterType::ALT_GRAYSCALE:
				alt_grayscale(src_frame, dst_frame);
				break;
			case FilterType::BLUR5x5:
				blur5x5(src_frame, dst_frame);
				break;
			case FilterType::SOBEL_X:
				sobelX3x3(src_frame, dst_frame);
    			cv::convertScaleAbs(dst_frame, dst_frame);
				break;
			case FilterType::SOBEL_Y:
				sobelY3x3(src_frame, dst_frame);
    			cv::convertScaleAbs(dst_frame, dst_frame);
				break;
			case FilterType::GRAD_MAG:
				sobelX3x3(src_frame, sx);
				sobelY3x3(src_frame, sy);
				magnitude(sx, sy, dst_frame);
    			cv::convertScaleAbs(dst_frame, dst_frame);
				break;
			case FilterType::BLUR_QUANTIZE:
				blurQuantize(src_frame, dst_frame);
				break;
			case FilterType::CARTOON:
				cartoon(src_frame, dst_frame);
				break;
			case FilterType::BRIGHTNESS:
				brightness(src_frame, dst_frame, 1.3, brightness_level);
				break;
			case FilterType::SEPIA:
				sepia(src_frame, dst_frame);
				break;
			default:
				src_frame.copyTo(dst_frame);
		}

		if (vid_writer.isOpened()) {
			vid_writer.write(dst_frame);
		}

		cv::imshow("Video", dst_frame);

		// see if there is a waiting keystroke
		int key = cv::waitKey(1);
		if (key == 'q' || key == 'Q') { // quit capture loop
			printf("'q' key pressed. Exiting...\n");
			break;
		} else if (key == 's' || key == 'S') { // save image
			printf("'s' key pressed. Saving image...\n");
			if (fil == FilterType::DEFAULT) {
				cv::imwrite("img.png", src_frame);
			} else {
				// write default and modified images
				cv::imwrite("img_mod.png", dst_frame);
				cv::imwrite("img.png", src_frame);
			}
		} else if (key == 'v' || key == 'V') { // start video recording
			if (!vid_writer.isOpened()) {
				printf("'v' key pressed. Starting video recording...\n");
				if (!vid_writer.open("vid.mp4", fourcc, 30, refS)) {
					printf("Unable to start video recording\n");
					return -1;
				}
			} else {
				printf("'v' key pressed again. Stopping video recording...\n");
				vid_writer.release();
			}
		} else if (key == 'g' || key == 'G') { // grayscale filter
			if (fil != FilterType::GRAYSCALE) {
				printf("'g' key pressed. Applying grayscale filter...\n");
				fil = FilterType::GRAYSCALE;
			} else {
				printf("'g' key pressed again. Removing grayscale filter...\n");
				fil = FilterType::DEFAULT;
			}
		} else if (key == 'h' || key == 'H') { // alternative grayscale filter
			if (fil != FilterType::ALT_GRAYSCALE) {
				printf("'h' key pressed. Applying alternative grayscale filter...\n");
				fil = FilterType::ALT_GRAYSCALE;
			} else {
				printf("'h' key pressed again. Removing alternative grayscale filter...\n");
				fil = FilterType::DEFAULT;
			}
		} else if (key == 'b' || key == 'B') { // 5x5 gaussian blur filter
			if (fil != FilterType::BLUR5x5) {
				printf("'b' key pressed. Applying blur filter...\n");
				fil = FilterType::BLUR5x5;
			} else {
				printf("'b' key pressed again. Removing blur filter...\n");
				fil = FilterType::DEFAULT;
			}
		} else if (key == 'x' || key == 'X') { // sobel x filter
			if (fil != FilterType::SOBEL_X) {
				printf("'x' key pressed. Applying horizontal sobel filter...\n");
				fil = FilterType::SOBEL_X;
			} else {
				printf("'x' key pressed again. Removing horizontal sobel filter...\n");
				fil = FilterType::DEFAULT;
			}
		} else if (key == 'y' || key == 'Y') { // sobel y filter 
			if (fil != FilterType::SOBEL_Y) {
				printf("'y' key pressed. Applying vertical sobel filter...\n");
				fil = FilterType::SOBEL_Y;
			} else {
				printf("'y' key pressed again. Removing vertical sobel filter...\n");
				fil = FilterType::DEFAULT;
			}
		} else if (key == 'm' || key == 'M') { // gradient magnitude filter
			if (fil != FilterType::GRAD_MAG) {
				printf("'m' key pressed. Applying gradient magnitude filter...\n");
				fil = FilterType::GRAD_MAG;
			} else {
				printf("'m' key pressed again. Removing gradient magnitude filter...\n");
				fil = FilterType::DEFAULT;
			}
		} else if (key == 'l' || key == 'L') { // blur and quantize filter
			if (fil != FilterType::BLUR_QUANTIZE) {
				printf("'l' key pressed. Applying blur and quantize filter...\n");
				fil = FilterType::BLUR_QUANTIZE;
			} else {
				printf("'l' key pressed again. Removing blur and quantize filter...\n");
				fil = FilterType::DEFAULT;
			}
		} else if (key == 'c' || key == 'C') { // cartoon filter
			if (fil != FilterType::CARTOON) {
				printf("'c' key pressed. Applying cartoon filter...\n");
				fil = FilterType::CARTOON;
			} else {
				printf("'c' key pressed again. Removing cartoon filter...\n");
				fil = FilterType::DEFAULT;
			}
		} else if (key == 't' || key == 'T') { // brightness filter
			// brightness filter is disabled when user presses key 6 times total
			if (fil != FilterType::BRIGHTNESS) {
				printf("'t' key pressed. Applying brightness filter...\n");
				fil = FilterType::BRIGHTNESS;
			} else if (brightness_level < 100) {
				brightness_level += 20;
			} else {
				printf("'t' key pressed 6 times. Removing brightness filter...\n");
				fil = FilterType::DEFAULT;
				brightness_level = 20;
			}
		} else if (key == 'e' || key == 'E') { // sepia filter
			if (fil != FilterType::SEPIA) {
				printf("'e' key pressed. Applying sepia filter...\n");
				fil = FilterType::SEPIA;
			} else {
				printf("'e' key pressed again. Removing sepia filter...\n");
				fil = FilterType::DEFAULT;
			}
		}
	}

	// free the memory allocated for the video device
	cv::destroyAllWindows();
	delete capdev;
	return 0;
}