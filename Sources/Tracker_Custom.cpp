
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
	//TODO calculate hist for test picture here

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

	Mat picture(160, 360, CV_8UC3, Scalar(100, 220, 80));

	strikes_map = picture;
	rectangle(strikes_map, Point(26, 26), Point(334, 134), CV_RGB(255, 255, 255), 8, 8);
	line(strikes_map, Point(0, 134), Point(360, 134), Scalar(255,255,255), 8);

	//namedWindow("Penalty Tracker");
}

cv::Rect Tracker_Custom::dnn_search_goal(cv::Mat& frame)
{
	Rect goal_rect;
	return goal_rect;
}

Rect Tracker_Custom::search_goal(Mat& frame)
{
	Mat gray_frame;
	Mat goal_selected_frame;
	Mat goal_roi;
	double minval, maxval;
	Point minloc, maxloc;

	cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
	threshold(gray_frame, gray_frame, 230, 255, THRESH_BINARY);

	matchTemplate(gray_frame, gray_goal_picture, goal_selected_frame, TM_CCOEFF);
	minMaxLoc(goal_selected_frame, &minval, &maxval, &minloc, &maxloc);

	//cout << "Minval = " << minval << "; Maxval = " << maxval << " Medium = " << (minval+maxval)/2 << endl;
	normalize(goal_selected_frame, goal_selected_frame,0,1,NORM_MINMAX, -1, Mat());

	Rect goal_rect(Point(minloc.x, minloc.y), Point(minloc.x+goal_picture.cols-1, minloc.y+goal_picture.rows-1));

	Mat canny_image;
	Mat goal_frame;

	goal_frame = gray_frame(goal_rect);

	Point left_max(goal_frame.cols,goal_frame.rows);
	Point left_min(0,0);
	Point right_max(0,0);
	Point right_min(0,0);

	Canny(goal_frame, canny_image, 10, 300, 7);
	vector<Vec4i> lines;
	HoughLinesP(canny_image, lines, 1, CV_PI/180, 50, 50, 10);
	for(size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];

		if((l[2]-l[0]) < 5 && (l[2]-l[0]) > -5)
		{
			line(canny_image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,255), 3);
				/*if(l[0] < canny_image.cols/2 && l[1] > l[3])
				{
					if(left_min.y < l[1])
					{
						left_min.x = l[0];
						left_min.y = l[1];
					}
				}
				if(l[0] < canny_image.cols/2 && l[1] < l[3])
				{
					if(left_max.y > l[1])
					{
						left_max.x = l[0];
						left_max.y = l[1];
					}
				}
				if(l[2] < canny_image.cols/2 && l[3] < l[1])
				{
					if(left_max.y > l[3])
					{
						left_max.x = l[2];
						left_max.y = l[3];
					}
				}
				if(l[2] < canny_image.cols/2 && l[3] > l[1])
				{
					if(left_min.y < l[3])
					{
						left_min.x = l[2];
						left_min.y = l[3];
					}
				}

				circle(canny_image, left_min, 20, Scalar(255,255,255), 6);
				circle(canny_image, left_max, 20, Scalar(255,255,255), 6);*/
		}
	}
	namedWindow("Canny detector");
	imshow("Canny detector", canny_image);

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

Rect Tracker_Custom::search_ball(Mat& frame, cv::Rect& frame_center)
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

void Tracker_Custom::draw_strikes_map(vector<Point> strikes_coords) // x and y are % of goal width and height(0..100)
{
	for(size_t i = 0; i < strikes_coords.size(); i++)
	{
		Point coord = strikes_coords[i];
		coord.x = coord.x * 3 + 30;
		coord.y = coord.y + 30; // from left upper corner, if from lower corner = 100 - coord.y + 30
		cout << coord.x << "	" << coord.y << endl;
		circle(strikes_map, coord, 10, Scalar(0,0,255), 6, 16);
	}
}
void Tracker_Custom::save_strikes_map()
{
	imwrite("Strikes_map.jpg", strikes_map);
}

void Tracker_Custom::workcycle()
{
	cout << "WC";
	VideoCapture video(video_name);

	vector<Point> strikes_coords {Point(0, 0), Point(30, 60), Point(90, 5), Point(85, 8)};
	cout << strikes_coords.size() << endl;
	draw_strikes_map(strikes_coords);
	save_strikes_map();


	while(true)
	{

		Mat frame;
		Point a(frame.cols/2 + 0.15 * frame.cols, frame.rows/2 - 0.07 * frame.rows);
		Point b(frame.cols/2 + 0.32 * frame.cols, frame.rows/2 + 0.17 * frame.rows);
		Rect frame_center(a, b);

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

			Rect ball_rect = search_ball(frame, frame_center);
			//rectangle(frame, ball_rect, CV_RGB(0, 0, 255), 4, 8);


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

