#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/filesystem.hpp>

using namespace cv;

//-----------------------------------------------------------------------------
// 模糊
class AppImread
{
  public:
	AppImread() {}
	~AppImread() {}
	void Run(const String &filename);

  private:
	ImreadModes ImreadMode(int pos);
	String ImreadModeName(ImreadModes mode);
	String ImreadModeDescription(ImreadModes mode);
	void ReadAndShow(ImreadModes mode);
	static void TrackbarCallbackBlur(int pos, void *userdata);

  private:
	Mat m_image;
	ImreadModes m_mode;
	String m_imgname;
	String m_winname;
	String m_trackbarname;
};

void AppImread::Run(const String &filename)
{
	if (filename.empty())
		return;

	m_winname = "read mode";
	namedWindow(m_winname, WINDOW_NORMAL); // 窗口控制大小

	m_imgname = filename;

	m_trackbarname = "模式";
	int pos = 1;	// 滑动条游标初始位置
	int count = 12; // 滑动条切份
	createTrackbar(m_trackbarname, m_winname, &pos, count, TrackbarCallbackBlur, (void *)this);

	// 非空格关闭后再显示
	do
	{
		ReadAndShow(ImreadMode(pos));
	} while (waitKey() != 32);
}

ImreadModes AppImread::ImreadMode(int pos)
{
	switch (pos)
	{
	case 0:
		return IMREAD_UNCHANGED;
	case 1:
		return IMREAD_GRAYSCALE;
	case 2:
		return IMREAD_COLOR;
	case 3:
		return IMREAD_ANYDEPTH;
	case 4:
		return IMREAD_ANYCOLOR;
	case 5:
		return IMREAD_LOAD_GDAL;
	case 6:
		return IMREAD_REDUCED_GRAYSCALE_2;
	case 7:
		return IMREAD_REDUCED_COLOR_2;
	case 8:
		return IMREAD_REDUCED_GRAYSCALE_4;
	case 9:
		return IMREAD_REDUCED_COLOR_4;
	case 10:
		return IMREAD_REDUCED_GRAYSCALE_8;
	case 11:
		return IMREAD_REDUCED_COLOR_8;
	case 12:
		return IMREAD_IGNORE_ORIENTATION;
	default:
		return IMREAD_GRAYSCALE;
	}
}

String AppImread::ImreadModeName(ImreadModes mode)
{
	switch (mode)
	{
	case IMREAD_UNCHANGED:
		return "-1  - IMREAD_UNCHANGED";
	case IMREAD_GRAYSCALE:
		return " 0  - IMREAD_GRAYSCALE";
	case IMREAD_COLOR:
		return " 1  - IMREAD_COLOR";
	case IMREAD_ANYDEPTH:
		return " 2  - IMREAD_ANYDEPTH";
	case IMREAD_ANYCOLOR:
		return " 4  - IMREAD_ANYCOLOR";
	case IMREAD_LOAD_GDAL:
		return " 8  - IMREAD_LOAD_GDAL";
	case IMREAD_REDUCED_GRAYSCALE_2:
		return " 16 - IMREAD_REDUCED_GRAYSCALE_2";
	case IMREAD_REDUCED_COLOR_2:
		return " 17 - IMREAD_REDUCED_COLOR_2";
	case IMREAD_REDUCED_GRAYSCALE_4:
		return " 32 - IMREAD_REDUCED_GRAYSCALE_4";
	case IMREAD_REDUCED_COLOR_4:
		return " 33 - IMREAD_REDUCED_COLOR_4";
	case IMREAD_REDUCED_GRAYSCALE_8:
		return " 64 - IMREAD_REDUCED_GRAYSCALE_8";
	case IMREAD_REDUCED_COLOR_8:
		return " 65 - IMREAD_REDUCED_COLOR_8";
	case IMREAD_IGNORE_ORIENTATION:
		return "128 - IMREAD_IGNORE_ORIENTATION";
	default:
		return " X - error mode";
	}
}

String AppImread::ImreadModeDescription(ImreadModes mode)
{
	switch (mode)
	{
	case IMREAD_UNCHANGED:
		return "If set, return the loaded image as is (with alpha channel, otherwise it gets cropped).";
	case IMREAD_GRAYSCALE:
		return "If set, always convert image to the single channel grayscale image.";
	case IMREAD_COLOR:
		return "If set, always convert image to the 3 channel BGR color image.";
	case IMREAD_ANYDEPTH:
		return "If set, return 16-bit/32-bit image when the input has the corresponding depth, otherwise convert it to 8-bit.";
	case IMREAD_ANYCOLOR:
		return "If set, the image is read in any possible color format.";
	case IMREAD_LOAD_GDAL:
		return "If set, use the gdal driver for loading the image.";
	case IMREAD_REDUCED_GRAYSCALE_2:
		return "If set, always convert image to the single channel grayscale image and the image size reduced 1/2.";
	case IMREAD_REDUCED_COLOR_2:
		return "If set, always convert image to the 3 channel BGR color image and the image size reduced 1/2.";
	case IMREAD_REDUCED_GRAYSCALE_4:
		return "If set, always convert image to the single channel grayscale image and the image size reduced 1/4.";
	case IMREAD_REDUCED_COLOR_4:
		return "If set, always convert image to the 3 channel BGR color image and the image size reduced 1/4.";
	case IMREAD_REDUCED_GRAYSCALE_8:
		return "If set, always convert image to the single channel grayscale image and the image size reduced 1/8.";
	case IMREAD_REDUCED_COLOR_8:
		return "If set, always convert image to the 3 channel BGR color image and the image size reduced 1/8.";
	case IMREAD_IGNORE_ORIENTATION:
		return "If set, do not rotate the image according to EXIF's orientation flag.";
	default:
		return "error mode";
	}
}

void AppImread::ReadAndShow(ImreadModes mode)
{
	m_mode = mode;
	m_image = imread(m_imgname, m_mode);
	imshow(m_winname, m_image);
	String overlay;
	overlay += ImreadModeName(m_mode);
	overlay += "\n";
	overlay += ImreadModeDescription(m_mode);
	displayOverlay(m_winname, overlay);
	//displayStatusBar(m_winname, ImreadModeDescription(m_mode));
}

void AppImread::TrackbarCallbackBlur(int pos, void *usedata)
{
	AppImread *thisobj = (AppImread *)usedata;
	ImreadModes mode = thisobj->ImreadMode(pos);
	if(mode != thisobj->m_mode)
		thisobj->ReadAndShow(mode);
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("argc error !");
		return 0;
	}

	printf("cwd = %s\n", utils::fs::getcwd().c_str());

	AppImread app;
	app.Run(argv[1]);

	return 0;
}