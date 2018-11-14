#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/filesystem.hpp>

using namespace cv;

//-----------------------------------------------------------------------------
// 模糊
class AppThreshold
{
  public:
	AppThreshold() {}
	~AppThreshold() {}
	void Run(const String &filename);

  private:
	void Show(ThresholdTypes type, ThresholdTypes flag, double thresh, double maxval);
	ThresholdTypes ThresholdType(int pos);
	ThresholdTypes ThresholdFlag(int pos);
	String ThresholdTypeName(ThresholdTypes type);
	String ThresholdTypeDescription(ThresholdTypes type);
	static void TrackbarCallbackType(int pos, void *userdata);
	static void TrackbarCallbackFlag(int pos, void *userdata);
	static void TrackbarCallbackThresh(int pos, void *userdata);
	static void TrackbarCallbackMaxval(int pos, void *userdata);

  private:
	Mat m_image_src;
	Mat m_image_dst;
	ThresholdTypes m_type;
	ThresholdTypes m_flag;
	double m_thresh;
	double m_maxval;
	String m_imgname;
	String m_winname;
	String m_trackbarname_type;
	String m_trackbarname_flag;
	String m_trackbarname_thresh;
	String m_trackbarname_maxval;
};

void AppThreshold::Run(const String &filename)
{
	if (filename.empty())
		return;

	m_winname = "read mode";
	namedWindow(m_winname, WINDOW_NORMAL); // 窗口控制大小

	m_imgname = filename;
	m_image_src = imread(m_imgname, ImreadModes::IMREAD_GRAYSCALE);

	m_trackbarname_type = "Type";
	int pos_type = 0;
	int count_type = 4;
	createTrackbar(m_trackbarname_type, m_winname, &pos_type, count_type, TrackbarCallbackType, (void *)this);

	if (m_image_src.type() == CV_8UC1)
	{
		m_trackbarname_flag = "flag";
		int pos_flag = 0;
		int count_flag = 2;
		createTrackbar(m_trackbarname_flag, m_winname, &pos_flag, count_flag, TrackbarCallbackFlag, (void *)this);
	}

	m_trackbarname_thresh = "Thresh";
	int pos_thresh = 0;
	int count_thresh = 255;
	createTrackbar(m_trackbarname_thresh, m_winname, &pos_thresh, count_thresh, TrackbarCallbackThresh, (void *)this);

	m_trackbarname_maxval = "Maxval";
	int pos_maxval = 255;
	int count_maxval = 255;
	createTrackbar(m_trackbarname_maxval, m_winname, &pos_maxval, count_maxval, TrackbarCallbackMaxval, (void *)this);

	// 非空格关闭后再显示
	do
	{
		m_type = ThresholdType(pos_type);
		m_flag = (ThresholdTypes)0;
		m_thresh = 1.0 * pos_thresh;
		m_maxval = 1.0 * pos_maxval;
		imshow(m_winname, m_image_src);
	} while (waitKey() != 32);
}

void AppThreshold::Show(ThresholdTypes type, ThresholdTypes flag, double thresh, double maxval)
{
	m_type = type;
	m_flag = flag;
	m_thresh = thresh;
	m_maxval = maxval;
	threshold(m_image_src, m_image_dst, m_thresh, m_maxval, m_type | m_flag);
	imshow(m_winname, m_image_dst);
	String overlay;
	overlay += ThresholdTypeName(type);
	overlay += "\n";
	overlay += ThresholdTypeDescription(type);
	overlay += "\n";
	if(m_flag & THRESH_OTSU)
	{
		overlay += ThresholdTypeName(THRESH_OTSU);
		overlay += "\n";
		overlay += ThresholdTypeDescription(THRESH_OTSU);
		overlay += "\n";
	}
	if(m_flag & THRESH_TRIANGLE)
	{
		overlay += ThresholdTypeName(THRESH_TRIANGLE);
		overlay += "\n";
		overlay += ThresholdTypeDescription(THRESH_TRIANGLE);
		overlay += "\n";
	}
	displayOverlay(m_winname, overlay);
}

ThresholdTypes AppThreshold::ThresholdType(int pos)
{
	switch (pos)
	{
	case 0:
		return THRESH_BINARY;
	case 1:
		return THRESH_BINARY_INV;
	case 2:
		return THRESH_TRUNC;
	case 3:
		return THRESH_TOZERO;
	case 4:
		return THRESH_TOZERO_INV;
	case 5:
		return THRESH_MASK;
	case 6:
		return THRESH_OTSU;
	case 7:
		return THRESH_TRIANGLE;
	default:
		return THRESH_MASK;
	}
}

ThresholdTypes AppThreshold::ThresholdFlag(int pos)
{
	switch (pos)
	{
	case 1:
		return THRESH_OTSU;
	case 2:
		return THRESH_TRIANGLE;
	default:
		return (ThresholdTypes)0;
	}
}

String AppThreshold::ThresholdTypeName(ThresholdTypes type)
{
	switch (type)
	{
	case THRESH_BINARY:
		return " 0  - THRESH_BINARY";
	case THRESH_BINARY_INV:
		return " 1  - THRESH_BINARY_INV";
	case THRESH_TRUNC:
		return " 2  - THRESH_TRUNC";
	case THRESH_TOZERO:
		return " 3  - THRESH_TOZERO";
	case THRESH_TOZERO_INV:
		return " 4  - THRESH_TOZERO_INV";
	case THRESH_MASK:
		return " 7  - THRESH_MASK";
	case THRESH_OTSU:
		return " 8  - THRESH_OTSU";
	case THRESH_TRIANGLE:
		return " 16 - THRESH_TRIANGLE";
	default:
		return " X - error";
	}
}

String AppThreshold::ThresholdTypeDescription(ThresholdTypes type)
{
	switch (type)
	{
	case THRESH_BINARY:
		return "if src(x,y) > thresh, dst(x,y) = maxval;\n otherwise, dst(x,y) = 0.";
	case THRESH_BINARY_INV:
		return "if src(x,y) > thresh, dst(x,y) = 0;\n otherwise, dst(x,y) = maxval.";
	case THRESH_TRUNC:
		return "if src(x,y) > thresh, dst(x,y) = threshold;\n otherwise, dst(x,y) = src(x,y).";
	case THRESH_TOZERO:
		return "if src(x,y) > thresh, dst(x,y) = src(x,y);\n otherwise, dst(x,y) = 0.";
	case THRESH_TOZERO_INV:
		return "if src(x,y) > thresh, dst(x,y) = 0;\n otherwise, dst(x,y) = src(x,y).";
	case THRESH_MASK:
		return "";
	case THRESH_OTSU:
		return "flag, use Otsu algorithm to choose the optimal threshold value";
	case THRESH_TRIANGLE:
		return "flag, use Triangle algorithm to choose the optimal threshold value";
	default:
		return " X - error";
	}
}

void AppThreshold::TrackbarCallbackType(int pos, void *usedata)
{
	AppThreshold *thisobj = (AppThreshold *)usedata;
	ThresholdTypes type = thisobj->ThresholdType(pos);
	if (type != thisobj->m_type)
		thisobj->Show(type, thisobj->m_flag, thisobj->m_thresh, thisobj->m_maxval);
}

void AppThreshold::TrackbarCallbackFlag(int pos, void *usedata)
{
	AppThreshold *thisobj = (AppThreshold *)usedata;
	ThresholdTypes flag = thisobj->ThresholdFlag(pos);
	if (flag != thisobj->m_flag)
		thisobj->Show(thisobj->m_type, flag, thisobj->m_thresh, thisobj->m_maxval);
}

void AppThreshold::TrackbarCallbackThresh(int pos, void *usedata)
{
	AppThreshold *thisobj = (AppThreshold *)usedata;
	double thresh = 1.0 * pos;
	if (thresh != thisobj->m_thresh)
		thisobj->Show(thisobj->m_type, thisobj->m_flag, thresh, thisobj->m_maxval);
}

void AppThreshold::TrackbarCallbackMaxval(int pos, void *usedata)
{
	AppThreshold *thisobj = (AppThreshold *)usedata;
	double maxval = 1.0 * pos;
	if (maxval != thisobj->m_maxval)
		thisobj->Show(thisobj->m_type, thisobj->m_flag, thisobj->m_thresh, maxval);
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

	AppThreshold app;
	app.Run(argv[1]);

	return 0;
}