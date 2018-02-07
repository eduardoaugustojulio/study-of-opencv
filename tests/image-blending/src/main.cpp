#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int main(const int argc, const char **argv)
{
	if(argc < 3){
		std::cout << "usage: " << argv[0] << " {image 1}  {image 2} " << std::endl;
		return EXIT_FAILURE;
	}
	
	cv:: Mat src1, src2, dst;
	 
    src1 = cv::imread(argv[1]);
    src2 = cv::imread(argv[2]);
	
    cv::addWeighted( src1, 0.5, src2, 0.5, 0.0, dst);
    
    cv::imshow( "Linear Blend", dst );
    cv::waitKey(0);

    
    return EXIT_SUCCESS;
}
