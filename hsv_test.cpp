
/*
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <vector>
#include <string>

#include "Tracker.hpp"

#include <opencv2/core/utility.hpp>


/* TODO
*	Search frames by histogram
*	Search goal by matchTracking - done
*	Search goal in 5-50 frames, then calc middle point, then show
*	Track ball after strike
*	Affine or homography for finding point of shoot
*	Maybe: detect player in area around ball
*	Create picture w/ shoots for every team: if I can detect player,
*	then automatically count points,
*	else shoot by shoot
*/
/*
using namespace cv;
using namespace std;

MatND histogram(Mat& hsv)
{


	// Quantize the hue to 30 levels
	// and the saturation to 32 levels
	int hbins = 30, sbins = 32, vbins = 32;
	int histSize[] = { hbins, sbins, vbins };
	// hue varies from 0 to 179, see cvtColor
	float hranges[] = { 0, 180 };
	// saturation varies from 0 (black-gray-white) to
	// 255 (pure spectrum color)
	float sranges[] = { 0, 256 };
	float vranges[] = {0 , 256};
	const float* ranges[] = { hranges, sranges, vranges };
	MatND hist;
	// we compute the histogram from the 0-th and 1-st channels
	int channels[] = { 0, 1, 2 };

	calcHist(&hsv, 1, channels, Mat(), // do not use mask
		hist, 3, histSize, ranges,
		true, // the histogram is uniform
		false);
	double maxVal = 0;
	//minMaxLoc(hist, 0, &maxVal, 0, 0);
	return hist;
}

MatND histogram_vs(Mat& hsv)
{


	// Quantize the hue to 30 levels
	// and the saturation to 32 levels
	int vbins = 60, sbins = 8;
	int histSize[] = { vbins, sbins };
	float vranges[] = { 0, 256 };
	float sranges[] = { 0, 256 };
	const float* ranges[] = { vranges, sranges };
	MatND hist;
	// we compute the histogram from the 0-th and 1-st channels
	int channels[] = { 2, 1 };

	calcHist(&hsv, 1, channels, Mat(), // do not use mask
		hist, 2, histSize, ranges,
		true, // the histogram is uniform
		false);
	double maxVal = 0;
	minMaxLoc(hist, 0, &maxVal, 0, 0);

	return hist;

}

void Canny_detector(const Mat& gray_image)
{
	Mat canny_image;
	Canny(gray_image, canny_image, 10, 300, 7);
	vector<Vec4i> lines;
	HoughLinesP(canny_image, lines, 1, CV_PI/180, 50, 50, 10);
	for(size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(canny_image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,255), 3);
	}
	namedWindow("Canny detector");
	imshow("Canny detector", canny_image);
	//return canny_image;
}

int main(int argc, char* argv[])
{
	Mat goal_picture, gray_goal_picture, gray_image, test_image;
	VideoCapture cap("Rus-Cro_penalty.mp4");

	test_image = imread("test_image.jpg");
	goal_picture = imread("goal_bl.jpg");
	cvtColor(goal_picture, gray_goal_picture, COLOR_BGR2GRAY);
	threshold(gray_goal_picture, gray_goal_picture, 220, 255, THRESH_BINARY);

	namedWindow("img");
	imshow("img", gray_goal_picture);

	//Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_STD);


	if (cap.isOpened() == false)
	{
		cout << "Cannot open the video file" << endl;
		cin.get(); //wait for any key press
		return -1;
	}

	//cap.set(CAP_PROP_POS_MSEC, 300);

	double fps = cap.get(CAP_PROP_FPS);
	cout << "Frames per seconds : " << fps << endl;

	String window_name_gray = "Gray channel"; //goal is white

	namedWindow(window_name_gray, WINDOW_NORMAL); //create a window

	Tracker tr("Rus-Cro_penalty.mp4", "goal_bl.jpg", "test_image.jpg");
	tr.workcycle();
	while (true)
	{
		Mat frame;
		bool bSuccess = cap.read(frame); // read a new frame from video

		//Breaking the while loop at the end of the video
		if (bSuccess == false)
		{
			cout << "Found the end of the video" << endl;
			break;
		}
		Mat hsv_frame, hsv_test_image;
		MatND hist_f, hist_test;
		cvtColor(frame, hsv_frame, COLOR_BGR2HSV);
		cvtColor(test_image, hsv_test_image, COLOR_BGR2HSV);
		hist_f = histogram_vs(hsv_frame);
		hist_test = histogram_vs(hsv_test_image);
		double hist_diff = compareHist(hist_f, hist_test, 5);//correl ok, intersect better
		cout << "Diff" << hist_diff << endl;

		Mat gray_image;
		Mat goal_selected_image;
		cvtColor(frame, gray_image, COLOR_BGR2GRAY);
		threshold(gray_image, gray_image, 230, 255, THRESH_BINARY); //Gray

		matchTemplate(gray_image, gray_goal_picture, goal_selected_image, TM_CCOEFF);//CCOEFF CV_TM_CCOEFF

		double minval, maxval;
		Point minloc, maxloc;

		minMaxLoc(goal_selected_image, &minval, &maxval, &minloc, &maxloc);
		//normalize(goal_selected_image, goal_selected_image,1,0,CV_MINMAX);
		normalize(goal_selected_image, goal_selected_image,0,1,NORM_MINMAX , -1, Mat());//CV_MINMAX
		//rectangle(frame, Point(minloc.x, minloc.y), Point(minloc.x+goal_picture.cols-1, minloc.y+goal_picture.rows-1), CV_RGB(255, 0, 0), 1, 8);

		Rect goal_rect(Point(minloc.x, minloc.y), Point(minloc.x+goal_picture.cols-1, minloc.y+goal_picture.rows-1));
		rectangle(frame, goal_rect, CV_RGB(255, 0, 0), 1, 8);

		Mat goal_roi = gray_image(goal_rect);

		//resize(gray_image, gray_image, Size(800, 600));
		Mat canny_image;
		Canny_detector(goal_roi);

		if(hist_diff < -50000)
			rectangle(frame, Point(0, 0), Point(20, 20), Scalar(0, 255, 0), 5);
		else rectangle(frame, Point(0, 0), Point(20, 20), Scalar(0, 0, 255), 5);


		namedWindow("img2");
		imshow("img2", goal_roi);

		imshow(window_name_gray, gray_image);
		imshow("Original", frame);

		if (waitKey(10) == 27)
		{
			cout << "Esc key is pressed by user. Stoppig the video" << endl;
			break;
		}
	}

	return 0;

}
*/
