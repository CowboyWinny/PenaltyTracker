
#include "../Tracker_Custom.hpp"


using namespace cv;
using namespace std;

struct StrikePoint
{
	double x;
	double y;
};

Tracker_Custom::Tracker_Custom( string video_name_input, string goal_picture_name_input,
								string test_picture_name_input, string ball_picture_name_input)
{
	cout << "0";
	video_name = video_name_input;
	test_picture_name = test_picture_name_input;
	goal_picture_name = goal_picture_name_input;
	ball_picture_name = ball_picture_name_input;


	goal_picture = imread(goal_picture_name);
	test_picture = imread(test_picture_name);
	ball_picture = imread(ball_picture_name);
	cvtColor(goal_picture, gray_goal_picture, COLOR_BGR2GRAY);
	cvtColor(ball_picture, gray_ball_picture, COLOR_BGR2GRAY);
	threshold(gray_goal_picture, gray_goal_picture, 220, 255, THRESH_BINARY);
	threshold(gray_ball_picture, gray_ball_picture, 220, 255, THRESH_BINARY);
	cvtColor(test_picture, test_picture, COLOR_BGR2HSV);

	//namedWindow("Penalty Tracker");
}

Rect Tracker_Custom::search_goal(Mat& frame)
{
	Mat gray_frame;
	Mat goal_selected_frame;
	Mat goal_roi;
	double minval, maxval;
	Point minloc, maxloc;

	cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
	threshold(gray_frame, gray_frame, 220, 255, THRESH_BINARY);

	matchTemplate(gray_frame, gray_goal_picture, goal_selected_frame, TM_CCOEFF);
	minMaxLoc(goal_selected_frame, &minval, &maxval, &minloc, &maxloc);
	normalize(goal_selected_frame, goal_selected_frame,0,1,NORM_MINMAX, -1, Mat());

	Rect goal_rect(Point(minloc.x, minloc.y), Point(minloc.x+goal_picture.cols-1, minloc.y+goal_picture.rows-1));
	//goal_roi = gray_frame(goal_rect);

	return goal_rect;
}

double Tracker_Custom::search_frame(const Mat& frame)
{
	int vbins = 30, sbins = 8;
	int histSize[] = { vbins, sbins };
	float vranges[] = { 0, 256 };
	float sranges[] = { 0, 256 };
	const float* ranges[] = { vranges, sranges };
	MatND hist_f, hist_test;
	Mat hsv_frame;
	int channels[] = { 2, 1 };

	calcHist(&test_picture, 1, channels, Mat(), hist_test, 2, histSize, ranges, true, false);

	cvtColor(frame, hsv_frame, COLOR_BGR2HSV);
	calcHist(&hsv_frame, 1, channels, Mat(), hist_f, 2, histSize, ranges, true, false);
	double hist_diff = compareHist(hist_f, hist_test, 5);
	//cout << hist_diff << endl;
	return hist_diff;
}

Rect Tracker_Custom::search_ball(Mat& frame)
{
	//TODO change search method from matching to Hough circles

	Mat gray_frame;
	Mat ball_selected_frame;
	Mat ball_roi;
	double minval, maxval;
	Point minloc, maxloc;

	cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
	threshold(gray_frame, gray_frame, 220, 255, THRESH_BINARY);

	matchTemplate(gray_frame, gray_ball_picture, ball_selected_frame, TM_CCOEFF);
	minMaxLoc(ball_selected_frame, &minval, &maxval, &minloc, &maxloc);
	normalize(ball_selected_frame, ball_selected_frame,0,1,NORM_MINMAX, -1, Mat());

	Rect ball_rect(Point(minloc.x, minloc.y), Point(minloc.x+ball_picture.cols-1, minloc.y+ball_picture.rows-1));
		//goal_roi = gray_frame(goal_rect);

	return ball_rect;
}

void Tracker_Custom::workcycle()
{
	cout << "WC";
	VideoCapture video(video_name);

	while(true)
	{

		Mat frame;

		bool bSuccess = video.read(frame);
		/*if(bSuccess == false)
		{
			cout << "EOF" <<endl;
		}
		*/
		double hist_diff = search_frame(frame);
		if(hist_diff < -50000)
		{
			Rect goal_rect = search_goal(frame);
			rectangle(frame, goal_rect, CV_RGB(255, 0, 0), 4, 8);

			Rect ball_rect = search_ball(frame);
			rectangle(frame, ball_rect, CV_RGB(0, 0, 255), 4, 8);
		}

		namedWindow("PenaltyTracker");
		imshow("PenaltyTracker", frame);
		if (waitKey(10) == 27)
		{
			cout << "Esc key is pressed by user. Stopping the video" << endl;
			break;
		}
	}
}

