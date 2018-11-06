#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;

int main(int argc, char *argv[])
{
	if (argc != 2)
		return 0;

	char *imagefile = argv[1];
	IplImage *pImg = NULL; //declare a Iplimage pointer
	if (pImg = cvLoadImage(imagefile, 1))
	{
		cvNamedWindow(imagefile, CV_WINDOW_NORMAL);  //create a window
		cvShowImage(imagefile, pImg); //show image
		cvWaitKey(0);				  //wait for key pressing
		cvDestroyWindow(imagefile);   //destroy window
		cvReleaseImage(&pImg);		  //release the image
	}

	return 0;
}