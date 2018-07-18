#ifndef TRACKER_CUSTOM_H
#define TRACKER_CUSTOM_H

#include <string>
#include <vector>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
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
	cv::Mat strikes_map;
	//cv::VideoCapture video();

	Tracker_Custom( std::string video_name_input, std::string goal_picture_name_input,
					std::string test_picture_name_input, std::string ball_picture_name_input);//add input team names


	void workcycle();
	void draw_strikes_map(std::vector<cv::Point> strikes_coords); // x and y are % of goal width and height
	void save_strikes_map();

	private:
	//return frame w/ goal
	double search_frame(const cv::Mat& frame);
	//return goal coordinates. used in search_frame();
	cv::Rect search_goal(cv::Mat& frame);
	//return ball coordinates
	cv::Rect search_ball(cv::Mat& frame, cv::Rect& frame_center);
	cv::Rect dnn_search_goal(cv::Mat& frame);
	//?? track_ball();
	//return vector of strikes coordinates
	//std::vector<StrikePoint> strikesMap();
	//return team name of player who strikes penalty
	//std::string playerTeamDetector(const cv::Mat& frame);
};

#endif
