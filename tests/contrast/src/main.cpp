#include    <iostream>
#include    <vector>
#include    <string>
#include    <chrono>
#include    <tuple>
#include    <algorithm>
#include    <utility>
#include	<thread>
#include	<future>

#include	<opencv2/opencv.hpp>
#include	<opencv2/core.hpp>
#include	<opencv2/imgproc.hpp>

#include	<zbar.h>

using namespace cv;

struct Contrast{
		struct Beta{
				static const int max = -100;
				static const  int min = -180;
		};

		struct Alpha{
				static const  int max = 3;
				static constexpr double min = 1;
		};
};

bool foundContrastValues = false;
double Alpha, Beta;

int doContrastEnhancement(cv::Mat &image, int &barcodeType, std::string &barcodeValue, int(*decoding)(cv::Mat &_barcodeImage, int&_barcodeType, std::string &_barcodeValue))
{
	auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
	auto retValue = EXIT_FAILURE;


	cv::Mat enhanced;
	image.copyTo(enhanced);
	/*if(foundContrastValues){
		std::cout << "# Contrast values alpha: " << Alpha << " beta: " << Beta << std::endl;
		enhanced.convertTo(enhanced, -1, Beta, Alpha);
	}*/

	if((retValue = (*decoding)(enhanced, barcodeType, barcodeValue)) != EXIT_SUCCESS)
	{
		bool finish = false;
		foundContrastValues = false;

		for(double alpha = Contrast::Alpha::min;
			!finish && alpha < Contrast::Alpha::max; alpha+=0.5){

			for(double beta = Contrast::Beta::min;
				!finish && beta < Contrast::Beta::max; beta+=10){

				cv::imshow("CONTRAST", enhanced);
				if((retValue =  (*decoding)(enhanced, barcodeType, barcodeValue)) == EXIT_SUCCESS){
					foundContrastValues = finish = true;
					Alpha = alpha;
					Beta = beta;
					break;
				}
				image.copyTo(enhanced);
				enhanced.convertTo(enhanced, -1, alpha, beta );
				cv::waitKey(10);
				std::cout << "# Contrast values alpha: " << alpha << " beta: " << beta << std::endl;
			}
		}
		enhanced.copyTo(image);
	}


	auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
	std::cout << "# Total time elapsed in " << __FUNCTION__ << " " << (end - start) << " msec" << std::endl;
	return retValue;
}

int doContrastEnhancementTh(cv::Mat &image, int &barcodeType, std::string &barcodeValue, int(*decoding)(cv::Mat &_barcodeImage, int&_barcodeType, std::string &_barcodeValue))
{
	auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
	auto retValue = EXIT_FAILURE;


	cv::Mat enhanced;
	image.copyTo(enhanced);
	/*if(foundContrastValues){
		std::cout << "# Contrast values alpha: " << Alpha << " beta: " << Beta << std::endl;
		enhanced.convertTo(enhanced, -1, Beta, Alpha);
	}*/

	if((retValue = (*decoding)(enhanced, barcodeType, barcodeValue)) != EXIT_SUCCESS)
	{
		bool finish = false;
		foundContrastValues = false;

		for(double alpha = Contrast::Alpha::min;
			!finish && alpha < Contrast::Alpha::max; alpha+=0.5){

			for(double beta = Contrast::Beta::min;
				!finish && beta < Contrast::Beta::max; beta+=10){

				cv::adaptiveThreshold(enhanced,enhanced,255,cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 15, 3);
				cv::imshow("CONTRASTTH", enhanced);

				if((retValue =  (*decoding)(enhanced, barcodeType, barcodeValue)) == EXIT_SUCCESS){
					foundContrastValues = finish = true;
					Alpha = alpha;
					Beta = beta;
					break;
				}

				image.copyTo(enhanced);
				enhanced.convertTo(enhanced, -1, alpha, beta );
				cv::waitKey(10);

				std::cout << "# Contrast values alpha: " << alpha << " beta: " << beta << std::endl;
			}
		}
		enhanced.copyTo(image);
	}


	auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
	std::cout << "# Total time elapsed in " << __FUNCTION__ << " " << (end - start) << " msec" << std::endl;
	return retValue;
}


int doDecoding(cv::Mat &barcodeImage, int &barcodeType, std::string &barcodeValue)
{
	auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
	auto retValue = EXIT_SUCCESS;

	zbar::ImageScanner scanner;
	scanner.set_config(static_cast<zbar::zbar_symbol_type_t>(barcodeType), zbar::ZBAR_CFG_ENABLE, true);
	zbar::Image image(barcodeImage.cols, barcodeImage.rows, "Y800", barcodeImage.data, barcodeImage.cols * barcodeImage.rows);

	auto scanned = scanner.scan(image);
	if(scanned){
		for (zbar::Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
			if(barcodeType == symbol->get_type())
				barcodeValue.append(symbol->get_data());
			else
				return EXIT_FAILURE;

			if (symbol->get_location_size() == 4) {
				cv::line(barcodeImage, cv::Point(symbol->get_location_x(0), symbol->get_location_y(0)), cv::Point(symbol->get_location_x(1), symbol->get_location_y(1)), cv::Scalar(0, 255, 0), 2, 8, 0);
				cv::line(barcodeImage, cv::Point(symbol->get_location_x(1), symbol->get_location_y(1)), cv::Point(symbol->get_location_x(2), symbol->get_location_y(2)), cv::Scalar(0, 255, 0), 2, 8, 0);
				cv::line(barcodeImage, cv::Point(symbol->get_location_x(2), symbol->get_location_y(2)), cv::Point(symbol->get_location_x(3), symbol->get_location_y(3)), cv::Scalar(0, 255, 0), 2, 8, 0);
				cv::line(barcodeImage, cv::Point(symbol->get_location_x(3), symbol->get_location_y(3)), cv::Point(symbol->get_location_x(0), symbol->get_location_y(0)), cv::Scalar(0, 255, 0), 2, 8, 0);
			}
			break;
		}
		image.set_data(NULL, 0);
		std::cout << "# Decoded barcode in image " << barcodeValue << std::endl;
		retValue = EXIT_SUCCESS;
	}else{
		retValue = EXIT_FAILURE;
		std::cout << "# Can not decode the barcode in image " << retValue  << std::endl;
	}

	auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
	std::cout << "# Total time elapsed in " << __FUNCTION__ << " " << (end - start) << " msec" << std::endl;

	return retValue;
}

void testContrast(const int argc, const char **argv)
{

	bool finish = true;
	int barcodeType = 25;
	std::string barcodeValue;

	std::vector<cv::String> filename;
	cv::String path = "/media/eduardo/KINGSTON/camera/";
	cv::glob(path , filename);


	for(auto i = 0; i < filename.size(); i++){

		cv::Mat img , src = cv::imread(filename.at(i), cv::IMREAD_GRAYSCALE);
		//cv::rotate(src, src,cv::ROTATE_90_COUNTERCLOCKWISE);
		//cv::resize(src, src, cv::Size(640, 480), 0, 0, cv::INTER_AREA);
		cv::namedWindow("CONTRAST", CV_WINDOW_AUTOSIZE);
		cv::namedWindow("CONTRASTTH", CV_WINDOW_AUTOSIZE);

		do{
			auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();

			src.copyTo(img);

			std::future<int> future1 = std::async(std::launch::async, doContrastEnhancement, std::ref(img), std::ref(barcodeType), std::ref(barcodeValue), std::ref(doDecoding));
			std::future<int> future2 = std::async(std::launch::async, doContrastEnhancementTh, std::ref(img), std::ref(barcodeType), std::ref(barcodeValue), std::ref(doDecoding));

			auto a = future1.get();
			auto b = future2.get();

			if(a == EXIT_SUCCESS && b == EXIT_SUCCESS)
				std::cout << "LEU COM SUCESSO" << std::endl;
			else
				std::cout << "NÃO LEU" << std::endl;
			auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
			std::cout << "# Total time elapsed in " << __FUNCTION__ << " " << (end - start) << " msec" << std::endl;

			barcodeValue.clear();
			if(cv::waitKey(0) == 27)
				finish = false;
		}while(finish);
	}
}

int main(const int argc, const char **argv){

	testContrast(argc, argv);

	return 0;
}
