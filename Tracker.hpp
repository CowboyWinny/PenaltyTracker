#ifndef TRACKER_H
#define TRACKER_H

class Tracker
{
	public:
	//vector<StrikePoint> strikesMap;
	std::string video_name = "";
	std::string goal_picture_name = "";
	cv::Mat goal_picture;
	cv::Mat gray_goal_picture;
	cv::VideoCapture video;

	Tracker(std::string video_name_input, std::string goal_picture_name_input);//add input team names
	//void show();
	//create window w/ strikes map or return picture with strikes
	//cv::Mat drawStrikesMap();
	//create picture with strikes map
	//void createStrikesMapPicture();

	private:
	//return frame w/ goal
	cv::Mat search_frame();
	//return goal coordinates
	std::vector<double> search_goal();
	//return ???
	//??? trackBall();
	//return vector of strikes coordinates
	//std::vector<StrikePoint> strikesMap();
	//return team name of player who strikes penalty
	//std::string playerTeamDetector(const cv::Mat& frame);
};

#endif
