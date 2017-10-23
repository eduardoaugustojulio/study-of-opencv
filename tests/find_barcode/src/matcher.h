#ifndef FINDBAR_H
#define FINDBAR_H

#include    <iostream>
#include    <tuple>
#include    <vector>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

namespace csi_cv
{
    namespace matcher
    {
        class finder
        {
            public:
                finder(std::pair<const std::string, const std::string> path);
                ~finder();

            private:
                std::pair<cv::Mat, cv::Mat> image;
                const double ratio = 0.8;

                int do_find(std::pair<const std::string, const std::string> path);
        };
    }
}

#endif
