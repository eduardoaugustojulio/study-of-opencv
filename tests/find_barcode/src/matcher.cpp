#include    <iostream>
#include    <vector>
#include    <string>
#include    <chrono>

#include    <opencv2/core/core.hpp>
#include    <opencv2/highgui/highgui.hpp>
#include    <opencv2/features2d.hpp>
#include	  <opencv2/calib3d.hpp>

#include    "matcher.h"


namespace csi_cv
{
    namespace matcher
    {
        finder::finder(std::pair<const std::string, const std::string> path)
        {
            this->do_find(path);
        }

        finder::~finder()
        {

        }

		int finder::do_find(std::pair<const std::string, const std::string> path)
		{
			if(path.first.empty() || path.second.empty())
				return  EXIT_FAILURE;

			this->image.first = cv::imread(path.first, cv::IMREAD_GRAYSCALE);
			this->image.second = cv::imread(path.second, cv::IMREAD_GRAYSCALE);

			cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
			cv::Ptr<cv::FeatureDetector> extractor = cv::ORB::create();

			auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();

			std::pair<cv::Mat, cv::Mat> descriptors;
			std::pair<std::vector<cv::KeyPoint>,std::vector<cv::KeyPoint>> key_points;
			std::pair< std::vector<std::vector<cv::DMatch>>, std::vector<std::vector<cv::DMatch>> > matches;

			detector->detect(image.first, key_points.first);
			detector->detect(image.second, key_points.second);

			std::cout << "# key points search image: " << key_points.first.size() << std::endl;
			std::cout << "# key points objective image: " << key_points.second.size() << std::endl;

			extractor->compute(image.first, key_points.first, descriptors.first);
			extractor->compute(image.second, key_points.second, descriptors.second);

			std::cout << "# Descriptors size search image: " << descriptors.first.cols << ":" << descriptors.first.rows << std::endl;
			std::cout << "# Descriptors size objective image: " << descriptors.second.cols << ":" << descriptors.second.rows << std::endl;

			cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
			matcher->knnMatch( descriptors.first, descriptors.second, matches.first, 2 );
			matcher->knnMatch( descriptors.second, descriptors.first, matches.second, 2 );

			std::cout << "# Matches 1 - 2:" << matches.first.size() << std::endl;
			std::cout << "# Matches 2 - 1:" << matches.second.size() << std::endl;

			std::pair<std::vector<cv::DMatch>,std::vector<cv::DMatch>> good_matches;

			for(auto i = 0; i < matches.first.size(); i++){
				if(matches.first[i][0].distance < this->ratio * matches.first[i][1].distance);
				good_matches.first.push_back(matches.first[i][0]);
			}


			for(auto i = 0; i < matches.second.size(); i++){
				if(matches.second[i][0].distance < this->ratio * matches.second[i][1].distance);
				good_matches.second.push_back(matches.second[i][0]);
			}


			std::cout << "# Good matches 1 - 2" << good_matches.first.size() << std::endl;
			std::cout << "# Good matches 1 - 2" << good_matches.second.size() << std::endl;

			std::vector<cv::DMatch> better_matches;
			for(auto i = 0; i < good_matches.first.size(); i++){
				for(auto j = 0; j < good_matches.second.size(); j++){
					if(good_matches.first[i].queryIdx == good_matches.second[j].trainIdx && good_matches.second[j].queryIdx == good_matches.first[i].trainIdx){
						better_matches.push_back(cv::DMatch(good_matches.first[i].queryIdx, good_matches.first[i].trainIdx, good_matches.first[i].distance));
						break;
					}
				}
			}

			std::cout << "# Better matches: " << better_matches.size() << std::endl;

			std::vector<cv::Point2f> object, scene;
			for( size_t i = 0; i < better_matches.size(); i++) {
				object.push_back( key_points.first[ better_matches[i].queryIdx ].pt );
				scene.push_back( key_points.second[ better_matches[i].trainIdx ].pt );
			}
			

			if(!object.empty() && !scene.empty() && better_matches.size() > 4){
				cv::Mat homography = cv::findHomography(object, scene, cv::RANSAC );
				std::vector<cv::Point2f> object_corners(4), scene_corners(4);
				object_corners.at(0) = cv::Point(0, 0);
				object_corners.at(1) = cv::Point(image.first.cols, 0);
				object_corners.at(2) = cv::Point(image.first.cols, image.first.rows);
				object_corners.at(3) = cv::Point(0, image.first.rows);

				cv::perspectiveTransform(object_corners, scene_corners, homography);

				cv::line(image.second,  scene_corners[0] , scene_corners[1] , 	cv::Scalar(0,255,0), 4 );
				cv::line(image.second,  scene_corners[1] , scene_corners[2] , 	cv::Scalar(0,255,0), 4 );
				cv::line(image.second,  scene_corners[2] , scene_corners[3] , 	cv::Scalar(0,255,0), 4 );
				cv::line(image.second,  scene_corners[3]  , scene_corners[0] , 	cv::Scalar(0,255,0), 4 );

				cv::imshow("result", image.second);
			}

			cv::Mat output;
			cv::drawMatches(image.first, key_points.first, image.second, key_points.second, better_matches, output);


			auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
			std::cout << "# Total time elapsed " << (end - start) << " msec" << std::endl;
			
			cv::imshow("output", output);
		}
		}
	}
