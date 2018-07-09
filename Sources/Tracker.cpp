
#include <string>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "../Tracker.hpp"

using namespace cv;
using namespace std;

struct StrikePoint
{
	double x;
	double y;
};

Tracker::Tracker(string video_name_input, string goal_picture_name_input)
{
	video_name = video_name_input;
	goal_picture_name = goal_picture_name_input;
	video(video_name);

	goal_picture = imread(goal_picture_name);
	cvtColor(goal_picture, gray_goal_picture, COLOR_BGR2GRAY);
	threshold(gray_goal_picture, gray_goal_picture, 220, 255, THRESH_BINARY);

	namedWindow("Penalty Tracker");
}

