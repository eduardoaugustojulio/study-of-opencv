#include <iostream>
#include <chrono>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"

void rotate_image_90_degree(cv::Mat &src, cv::Mat &dst, int angle)
{
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

int main(const int argc, const char **argv)
{
	auto t1 = std::chrono::high_resolution_clock::now();
	//------------------------------------

	cv::Mat src, src_gray, grad_x, grad_y, gradient;
	cv::Mat threshold, kernel, closed;

	src = cv::imread(argv[1], cv::IMREAD_COLOR);
	cv::imshow("imagem fonte", src);

	cv::cvtColor(src, src_gray, cv::COLOR_RGB2GRAY);
	cv::Sobel(src_gray, grad_x, CV_32F, 1, 0, -1);
	cv::Sobel(src_gray, grad_y, CV_32F, 0, 1, -1);

	cv::subtract(grad_x, grad_y, gradient);
	cv::convertScaleAbs(gradient, gradient);
	cv::imshow("gradiente", gradient);

	cv::blur(gradient, gradient, cv::Point(9,9));
	cv::threshold(gradient, threshold, 150 , 255, cv::THRESH_BINARY);
	cv::imshow("binaria", threshold);

	kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Point(21,7));
	cv::morphologyEx(threshold, closed, cv::MORPH_CLOSE, kernel);
	cv::imshow("fechamento", closed);

	cv::erode(closed, closed,	 4);
	cv::dilate(closed, closed, 4);
	cv::imshow("E/D", closed);

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(closed, contours, hierarchy , cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	std::vector<std::vector<cv::Point>> contours_poly(contours.size());
	std::vector<cv::Rect> boundRect(contours.size());
	std::vector<cv::Point2f>center( contours.size() );
	std::vector<float> radius(contours.size());

	for( auto i = 0; i < contours.size(); i++ ){
		cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true );
		boundRect[i] = boundingRect( cv::Mat(contours_poly[i]) );
		cv::minEnclosingCircle((cv::Mat)contours_poly[i], center[i], radius[i] );
	}

	cv::Scalar color = cv::Scalar(0,255,0);
	for(auto i = 0; i < contours.size(); i++ ){
		cv::rectangle(src, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
	}

	std::vector<double> area;
	std::vector<cv::Size> size;
	for(auto i = 0; i < boundRect.size(); i++){
		 //size.push_back();
		 area.push_back(boundRect.at(i).area());
		 std::cout << "\n\n analisando item " << i << std::endl;
		 std::cout << boundRect.at(i).area() << std::endl;
		 std::cout << boundRect.at(i).size() << std::endl;
	}

	cv::Mat barcode_image;
	auto max_area = std::max_element(area.begin(), area.end());
	for(auto i = 0; i < boundRect.size(); i++){
		if(boundRect.at(i).area() == *max_area){
			 std::cout << "\n\n maior item " << i << std::endl;
			 std::cout << boundRect.at(i).area() << std::endl;
			 std::cout << boundRect.at(i).size() << std::endl;
			 barcode_image = src(cv::Rect(boundRect.at(i)));
		}
	}

	cv::imshow("Resultado", src);
	rotate_image_90_degree(barcode_image, barcode_image, 90);
	cv::imshow("Recorte", barcode_image);

	//------------------------------------
	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << "\n** TEMPO GASTO ** " << std::chrono::duration<double, std::milli>(t2-t1).count() << "ms"<< std::endl;
	cv::waitKey(0);

	return 0;
}
