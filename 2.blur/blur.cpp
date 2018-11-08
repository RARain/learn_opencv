#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/filesystem.hpp>

using namespace cv;

//-----------------------------------------------------------------------------
// 模糊
class AppBlur
{
public:
	AppBlur(){}
	~AppBlur(){}
	void Run(const String& filename);

private:
	void Show(int pos);
	static void TrackbarCallbackBlur(int pos, void* userdata);

private:
	Mat m_image_src;
	Mat m_image_dst;
	int m_image_blur; // 模糊度
	String m_imgname;
	String m_winname;
	String m_trackbarname;
};

void AppBlur::Run(const String& filename)
{
	if(filename.empty())
		return;

	m_imgname = filename;
	m_image_src = imread(m_imgname);

	if(m_image_src.empty())
		return;

	m_winname = filename;
	namedWindow(m_winname, WINDOW_GUI_NORMAL); // 窗口控制大小

	m_trackbarname = "模糊度";
	int pos = 0; // 滑动条游标初始位置
	int count = 100; // 滑动条切份
	createTrackbar(m_trackbarname, m_winname, &pos, count, TrackbarCallbackBlur, (void *)this);

	// 非空格关闭后再显示
	do
	{
		Show(pos);
	}while(waitKey() != 32);
}

void AppBlur::Show(int pos)
{
	if(pos > 0)
	{
		if(m_image_blur != pos)
		{
			m_image_blur = pos;
			blur(m_image_src, m_image_dst, Size(pos, pos));
		}
		imshow(m_imgname, m_image_dst);
	}
	else
		imshow(m_imgname, m_image_src);
}

void AppBlur::TrackbarCallbackBlur(int pos, void* usedata)
{
	AppBlur *thisobj = (AppBlur *)usedata;
	thisobj->Show(pos);
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

	AppBlur ab;
	ab.Run(argv[1]);
	
	return 0;
}