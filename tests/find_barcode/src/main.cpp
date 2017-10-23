#include <iostream>
#include <chrono>
#include <tuple>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "matcher.h"

class find_barcode
{
public:
    explicit find_barcode(const std::string &input,const std::string &output){
        if(this->do_find_barcode(input,output))
            EXIT_SUCCESS;
        EXIT_FAILURE;
    }

    int do_find_barcode(const std::string &input,const std::string &output){
        this->frame.first = cv::imread(input);

        std::pair<cv::Mat, cv::Mat> grad;
        cv::cvtColor(frame.first,frame.second, cv::COLOR_RGB2GRAY);
        cv::Sobel(this->frame.second, grad.first, CV_32F, 1, 0, -1);
        cv::Sobel(this->frame.second, grad.second, CV_32F, 0, 1, -1);
        cv::subtract(grad.first, grad.second, this->frame.second);
        cv::convertScaleAbs(this->frame.second, this->frame.second);
        cv::imshow("gradiente", this->frame.second);

        cv::blur(this->frame.second, this->frame.second, cv::Point(9,9));
        cv::threshold(this->frame.second, this->frame.second, 150 , 255, cv::THRESH_BINARY);
        cv::imshow("binaria", this->frame.second);

        cv::morphologyEx(this->frame.second, this->frame.second, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Point(21,7)));
        cv::imshow("fechamento", this->frame.second);

        cv::erode(this->frame.second, this->frame.second,	 4);
        cv::dilate(this->frame.second, this->frame.second, 4);
        cv::imshow("E/D", this->frame.second);
    }

    void rotate_frame_90_degree(cv::Mat &src, cv::Mat &dst, int angle){
        if(src.data != dst.data){
            src.copyTo(dst);
        }

        angle = ((angle / 90) % 4) * 90;

        //0 : flip vertical; 1 flip horizontal
        bool const flip_horizontal_or_vertical = angle > 0 ? 1 : 0;
        int const number = std::abs(angle / 90);

        for(int i = 0; i != number; ++i){
            cv::transpose(dst, dst);
            cv::flip(dst, dst, flip_horizontal_or_vertical);
        }
    }

    void get_input_frame(cv::Mat &in){
        this->frame.first.copyTo(in);
    }

    void get_output_frame(cv::Mat &out){
        this->frame.second.copyTo(out);
    }

private:
    std::pair<cv::Mat, cv::Mat> frame;
    std::pair<std::string, std::string> frame_path;

};

int main(const int argc, const char **argv)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    //------------------------------------

    csi_cv::matcher::finder new_finder(std::pair<std::string,std::string>("/home/eduardo/Documents/study-of-opencv/images/lenna.png","/home/eduardo/Documents/study-of-opencv/images/lenna.png"));

    //------------------------------------
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "\n** Total time elapsed ** " << std::chrono::duration<double, std::milli>(t2-t1).count() << " msec"<< std::endl;
    cv::waitKey(0);

    return 0;
}
