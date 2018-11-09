#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/filesystem.hpp>

using namespace cv;

//-----------------------------------------------------------------------------
// 截图
class AppROI
{
public:
	AppROI(){}
	~AppROI(){}
	void Run(const String& filename);

private:
	static void ButtonCallbackROI(int state, void* userdata);

private:
	Mat m_image_src;
	String m_imgname;
	String m_winname;
};

void AppROI::Run(const String& filename)
{
	if(filename.empty())
		return;

	m_imgname = filename;
	m_image_src = imread(m_imgname);

	if(m_image_src.empty())
		return;

	m_winname = filename;
	namedWindow(m_winname, WINDOW_NORMAL); // 窗口控制大小

	createButton("截图", ButtonCallbackROI, this, QT_PUSH_BUTTON, false);

	// 非空格关闭后再显示
	imshow(m_winname, m_image_src);
	waitKey(0);
}

void AppROI::ButtonCallbackROI(int state, void* userdata)
{
	AppROI *thisobj = (AppROI *)userdata;
	Rect roi = selectROI(thisobj->m_winname, thisobj->m_image_src, true, false);
	if(!roi.empty())
		thisobj->m_image_src = thisobj->m_image_src(roi);
	imshow(thisobj->m_winname, thisobj->m_image_src);
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

	AppROI app;
	app.Run(argv[1]);
	
	return 0;
}