#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
	if (argc != 2)
		return 0;

	char *imagefile = argv[1];
	Mat img = imread(imagefile);
	if (!img.data)
		return -1;
	namedWindow(imagefile, WINDOW_NORMAL); // 窗口控制大小
	imshow(imagefile, img);
	waitKey(0);

	return 0;
}