#include    <iostream>
#include    <tuple>
#include    <vector>
#include    <algorithm>
#include    <functional>

#include <opencv2/opencv.hpp>

int main(const int argc, const char **argv)
{
    cv::VideoCapture cap(0);

    int threshold_value = 0;

    cv::namedWindow("resultado", 1);
    cv::namedWindow("binaria", 1);


    cv::createTrackbar("threshold", "binaria", &threshold_value, 225);


    if(!cap.isOpened())
        return -1;

    std::pair<cv::Mat, cv::Mat> frame;
    std::pair<cv::Mat, cv::Mat> grad;

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    for(;;){
        cap >> frame.first;

        cv::cvtColor(frame.first, frame.second, cv::COLOR_RGB2GRAY);
        cv::Sobel(frame.second, grad.first,  CV_32F, 1, 0, -1);
        cv::Sobel(frame.second, grad.second, CV_32F, 0, 1, -1);
        cv::subtract(grad.first, grad.second, frame.second);
        cv::convertScaleAbs(frame.second, frame.second);
        cv::blur(frame.second, frame.second, cv::Point(9,9));
        cv::threshold(frame.second, frame.second, threshold_value, 225,cv::THRESH_BINARY);
        cv::imshow("binaria",frame.second);


        cv::morphologyEx(frame.second, frame.second, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Point(21,7)));
        cv::erode(frame.second, frame.second,  4);
        cv::dilate(frame.second, frame.second, 4);
        cv::findContours(frame.second, contours, hierarchy , cv::RETR_CCOMP, cv::CHAIN_APPROX_TC89_L1, cv::Point(0, 0));

        std::vector<std::vector<cv::Point>> contours_poly(contours.size());
        std::vector<cv::Rect> rectangle(contours.size());

        for(auto i = 0; i < contours.size(); i++){
            cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
            rectangle[i] = boundingRect(cv::Mat(contours_poly[i]));
        }

        auto compare_area = [](cv::Rect a, cv::Rect b) { return (a.area() < b.area());};
        auto max_area = std::max_element(rectangle.begin(), rectangle.end(), compare_area);
        auto pos = std::distance(rectangle.begin(), std::find(rectangle.begin(), rectangle.end(), *max_area));

        std::cout << *max_area << std::endl;

        cv::Scalar GREEN = cv::Scalar(0,225,0);
        cv::rectangle(frame.first, rectangle[pos].tl(), rectangle[pos].br(), GREEN, 2, 8, 0);


        cv::imshow("resultado",frame.first);

        if(cv::waitKey(10) == 27 ) break; // stop capturing by pressing ESC
    }


    return 0;
}
