/*
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/tracking.hpp>

#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <vector>
#include <string>

#include <opencv2/core/utility.hpp>

using namespace cv;
using namespace std;

vector<int> mouse_click;
void mouse_callback(int event, int x, int y, int flags, void* userdata)
{
	if(event == EVENT_LBUTTONDOWN)
	{
		mouse_click.push_back(y);
		mouse_click.push_back(x);
		cout << "pressed " << x << "  " << y << endl;
	}
}


int main(int argc, char* argv[])
{
	Mat goal_picture, gray_goal_picture, gray_image, test_image;
	VideoCapture cap("Rus-Cro_penalty.mp4");

	Ptr<Tracker> tracker = TrackerBoosting::create();

	bool track = false;

	namedWindow("Tracker");
	setMouseCallback("Tracker", mouse_callback, (void*)&mouse_click);

	if (cap.isOpened() == false)
	{
		cout << "Cannot open the video file" << endl;
		cin.get(); //wait for any key press
		return -1;
	}


	while (true)
	{
		Mat frame;

		Rect2d roi;


		bool bSuccess = cap.read(frame); // read a new frame from video

		//Breaking the while loop at the end of the video
		if (bSuccess == false)
		{
			cout << "Found the end of the video" << endl;
			break;
		}

		if(mouse_click.empty() == false)
		{
			int x = mouse_click.back();
			mouse_click.pop_back();
			int y = mouse_click.back();
			mouse_click.pop_back();

			roi=selectROI("Tracker",frame);
			tracker->init(frame,roi);
			track = true;

			cout << x << y;
			circle(frame, Point(x, y), 30, Scalar(255,0,0), 6);
			waitKey(1000);

		}


		if(track)
		{
		    tracker->update(frame,roi);
		    rectangle( frame, roi, Scalar( 255, 0, 0 ), 2, 1 );
		}


		imshow("Tracker", frame);

		if (waitKey(30) == 27)
		{
			cout << "Esc key is pressed by user. Stoppig the video" << endl;
			break;
		}

	}

	return 0;

}
*/
