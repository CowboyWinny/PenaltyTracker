#include <opencv2/tracking.hpp>

#include "Tracker_Custom.hpp"
using namespace std;
using namespace cv;

int main()
{
	cout << "1";
	Tracker_Custom tr("Rus-Cro_penalty.mp4", "goal_bl.jpg", "test_image.jpg", "ball.jpg");
	cout << "2";
	tr.workcycle();
	return 0;
}
