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


int main(int argc, char* argv[])
{

	VideoCapture cap("C:\\Users\\cowbo\\Documents\\C++\\PenaltyTracker\\Rus-Esp_penalty.mp4");

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

		resize(gray_image, gray_image, Size(800, 600));

		//ls->detect(gray_image, lines_std);
		//ls->drawSegments(gray_image, lines_std);

		histogram(hsv);

		imshow(window_name_gray, gray_image);
		imshow("Original", frame);

		//wait for for 10 ms until any key is pressed.  
		//If the 'Esc' key is pressed, break the while loop.
		//If the any other key is pressed, continue the loop 
		//If any key is not pressed withing 10 ms, continue the loop
		if (waitKey(10) == 27)
		{
			cout << "Esc key is pressed by user. Stoppig the video" << endl;
			break;
		}
	}

	return 0;

}