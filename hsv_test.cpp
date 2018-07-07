#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include <string>
/* TODO
*	search by shape - ball
*	search by hist - goal and frames with goal
*	draw strikes map
*	template matching for goal ???
*/
using namespace cv;
using namespace std;

void histogram(Mat& hsv)
{
	

	// Quantize the hue to 30 levels
	// and the saturation to 32 levels
	int hbins = 30, sbins = 32;
	int histSize[] = { hbins, sbins };
	// hue varies from 0 to 179, see cvtColor
	float hranges[] = { 0, 180 };
	// saturation varies from 0 (black-gray-white) to
	// 255 (pure spectrum color)
	float sranges[] = { 0, 256 };
	const float* ranges[] = { hranges, sranges };
	MatND hist;
	// we compute the histogram from the 0-th and 1-st channels
	int channels[] = { 0, 1 };

	calcHist(&hsv, 1, channels, Mat(), // do not use mask
		hist, 2, histSize, ranges,
		true, // the histogram is uniform
		false);
	double maxVal = 0;
	minMaxLoc(hist, 0, &maxVal, 0, 0);

	int scale = 10;
	Mat histImg = Mat::zeros(sbins*scale, hbins * 10, CV_8UC3);

	for (int h = 0; h < hbins; h++)
		for (int s = 0; s < sbins; s++)
		{
			float binVal = hist.at<float>(h, s);
			int intensity = cvRound(binVal * 255 / maxVal);
			rectangle(histImg, Point(h*scale, s*scale),
				Point((h + 1)*scale - 1, (s + 1)*scale - 1),
				Scalar::all(intensity),
				CV_FILLED);
		}

	
	namedWindow("H-S Histogram", 1);
	imshow("H-S Histogram", histImg);
	//waitKey();
}

void histogram_vs(Mat& hsv)
{


	// Quantize the hue to 30 levels
	// and the saturation to 32 levels
	int vbins = 30, sbins = 32;
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

	int scale = 10;
	Mat histImg = Mat::zeros(sbins*scale, vbins * 10, CV_8UC3);

	for (int h = 0; h < vbins; h++)
		for (int s = 0; s < sbins; s++)
		{
			float binVal = hist.at<float>(h, s);
			int intensity = cvRound(binVal * 255 / maxVal);
			rectangle(histImg, Point(h*scale, s*scale),
				Point((h + 1)*scale - 1, (s + 1)*scale - 1),
				Scalar::all(intensity),
				CV_FILLED);
		}


	namedWindow("V-S Histogram", 1);
	imshow("V-S Histogram", histImg);
	//waitKey();
}

void Canny_detector(const Mat& gray_image)
{
	Mat canny_image;
	Canny(gray_image, canny_image, 10, 300, 7);

	namedWindow("Canny detector");
	imshow("Canny detector", canny_image);
}

int main(int argc, char* argv[])
{
	Mat goal_picture, gray_image;
	VideoCapture cap("C:\\Users\\cowbo\\Documents\\C++\\PenaltyTracker\\Rus-Esp_penalty.mp4");
	goal_picture = imread("C:\\Users\\cowbo\\Documents\\C++\\PenaltyTracker\\goal.jpg");
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


	while (true)
	{
		Mat frame;
		bool bSuccess = cap.read(frame); // read a new frame from video
		
		vector<Vec4f> lines_std;

		//Breaking the while loop at the end of the video
		if (bSuccess == false)
		{
			cout << "Found the end of the video" << endl;
			break;
		}
		Mat hsv;

		cvtColor(frame, hsv, COLOR_BGR2HSV);

		Mat gray_image;
		cvtColor(frame, gray_image, COLOR_BGR2GRAY);

		threshold(gray_image, gray_image, 190, 255, THRESH_BINARY); //Gray

		//Canny_detector(gray_image);

		resize(gray_image, gray_image, Size(800, 600));

		//ls->detect(gray_image, lines_std);
		//ls->drawSegments(gray_image, lines_std);

		histogram_vs(hsv);

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