#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/filesystem.hpp>

using namespace cv;

//-----------------------------------------------------------------------------

class AppResize
{
  public:
	AppResize() {}
	~AppResize() {}
	void Run(const String &filename);

  private:
	void Resize(double fx, double fy, int inter);
	double Fx(int pos) { return 1.0 * pos / 100; }
	double Fy(int pos) { return 1.0 * pos / 100; }
	int Inter(int pos);
	static void TrackbarCallbackX(int pos, void *userdata);
	static void TrackbarCallbackY(int pos, void *userdata);
	static void TrackbarCallbackInter(int pos, void *userdata);

  private:
	Mat m_image_src;
	Mat m_image_dst;
	String m_imgname;
	double m_fx;
	double m_fy;
	int m_inter;
};

void AppResize::Run(const String &filename)
{
	if (filename.empty())
		return;

	m_image_src = imread(filename);
	m_imgname = filename;

	if (m_image_src.empty())
		return;

	namedWindow(m_imgname, WINDOW_AUTOSIZE);

	// trackbar 不附着在window上的话就是setting窗口中
	
	int pos_x = 100;
	int count_x = 100;
	createTrackbar("fx", "", &pos_x, count_x, TrackbarCallbackX, (void *)this);

	int pos_y = 100;
	int count_y = 100;
	createTrackbar("fy", "", &pos_y, count_y, TrackbarCallbackY, (void *)this);

	int pos = 0;
	int count = 6;
	createTrackbar(
		"插值方法:_____________________\n"
		"1.INTER_NEAREST - 最近邻插值___\n"
		"2.INTER_LINEAR - 线性插值______\n"
		"3.INTER_AREA - 区域插值________\n"
		"4.INTER_CUBIC - 三次样条插值____\n"
		"5.INTER_LANCZOS4 - Lanczos插值_\n"
		"6.INTER_LINEAR_EXACT__________\n"
		, "", &pos, count, TrackbarCallbackInter, (void *)this);

	// 非空格关闭后再显示
	do
	{
		Resize(Fx(pos_x), Fy(pos_y), Inter(pos));
	} while (waitKey() != 32);
}

void AppResize::Resize(double fx, double fy, int inter)
{
	m_fx = fx;
	m_fy = fy;
	m_inter = inter;

	if(inter >= 0)
	{
		resize(m_image_src, m_image_dst, Size(), m_fx, m_fy, m_inter);
		imshow(m_imgname, m_image_dst);
	}
	else
		imshow(m_imgname, m_image_src);
}

int AppResize::Inter(int pos)
{
	switch (pos)
	{
	case 1:
		return INTER_NEAREST;
	case 2:
		return INTER_LINEAR;
	case 3:
		return INTER_CUBIC;
	case 4:
		return INTER_AREA;
	case 5:
		return INTER_LANCZOS4;
	case 6:
		return INTER_LINEAR_EXACT;
	default:
		return -1;
	}
}

void AppResize::TrackbarCallbackX(int pos, void *usedata)
{
	AppResize *thisobj = (AppResize *)usedata;
	thisobj->Resize(thisobj->Fx(pos), thisobj->m_fy, thisobj->m_inter);
}

void AppResize::TrackbarCallbackY(int pos, void *usedata)
{
	AppResize *thisobj = (AppResize *)usedata;
	thisobj->Resize(thisobj->m_fx, thisobj->Fy(pos), thisobj->m_inter);
}

void AppResize::TrackbarCallbackInter(int pos, void *usedata)
{
	AppResize *thisobj = (AppResize *)usedata;
	thisobj->Resize(thisobj->m_fx, thisobj->m_fy, thisobj->Inter(pos));
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

	AppResize app;
	app.Run(argv[1]);

	return 0;
}