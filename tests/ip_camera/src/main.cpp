#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"

int main (const int argc, const char **argv)
{
		cv::VideoCapture video_cap("rtsp://admin:admin@155.1.1.202/11");
		if(!video_cap.isOpened()){
			std::cout << "error while opening video stream file" << std::endl;
			return -1;
		}

		cv::Mat frame;
		do{
			video_cap >> frame;
			cv::imshow("test", frame);
			if(cv::waitKey(1) == 27) break;
		}while(frame.empty());

		return 0;
}
