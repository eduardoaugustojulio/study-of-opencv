#include <iostream>
#include <tuple>

#include <opencv2/opencv.hpp>

int main(const int argc, const char argv)
{

    cv::VideoCapture cap(1);

    if(!cap.isOpened())
        return -1;

    std::pair<cv::Mat, cv::Mat> frame;
    for(;;){
        cap >> frame.first;
        cv::imshow("input", frame.first);
        if(cv::waitKey(10) == 27)break;
    }

    return 0;
}
