#ifndef TRACKER_CUSTOM_H
#define TRACKER_CUSTOM_H

#include <string>
#include <vector>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

class Tracker_Custom
{
	public:
	//vector<StrikePoint> strikesMap;
	std::string video_name = "";
	std::string goal_picture_name = "";
	std::string test_picture_name = "";
	std::string ball_picture_name = "";
	cv::Mat test_picture;
	cv::Mat goal_picture;
	cv::Mat ball_picture;
	cv::Mat gray_ball_picture;
	cv::Mat gray_goal_picture;
	//cv::VideoCapture video();

	Tracker_Custom( std::string video_name_input, std::string goal_picture_name_input,
					std::string test_picture_name_input, std::string ball_picture_name_input);//add input team names
	//void show();
	//create window w/ strikes map or return picture with strikes
	//cv::Mat drawStrikesMap();
	//create picture with strikes map
	//void createStrikesMapPicture();

	void workcycle();
	//private:
	//return frame w/ goal
	double search_frame(const cv::Mat& frame);
	//return goal coordinates. used in search_frame();
	cv::Rect search_goal(cv::Mat& frame);
	//return ball coordinates
	cv::Rect search_ball(cv::Mat& frame);
	//?? track_ball();
	//return vector of strikes coordinates
	//std::vector<StrikePoint> strikesMap();
	//return team name of player who strikes penalty
	//std::string playerTeamDetector(const cv::Mat& frame);
};

#endif
