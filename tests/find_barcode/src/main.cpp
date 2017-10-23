#include <iostream>
#include <chrono>
#include <tuple>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "matcher.h"

int main(const int argc, const char **argv)
{
	auto t1 = std::chrono::high_resolution_clock::now();

	csi_cv::matcher::finder new_finder(std::pair<std::string,std::string>("/home/geckos/Documents/study-of-opencv/images/recorte.jpg", "/home/geckos/Documents/study-of-opencv/images/original.jpg"));

	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << "\n** Total time elapsed ** " << std::chrono::duration<double, std::milli>(t2-t1).count() << " msec"<< std::endl;
	cv::waitKey(0);

	return 0;
}
