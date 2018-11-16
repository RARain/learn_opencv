#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <vector>

using namespace cv;
using namespace std;

//-----------------------------------------------------------------------------

class Rectangle
{
  public:
	Rectangle();
	~Rectangle();

	void Read(const String &filename);
	void Draw(int hthresh, int hrange, int vthresh, int vrange, bool force = false);
	void Save();
	void Cut();

	int m_horizontal_thresh = 2;
	int m_horizontal_range = 10;
	int m_vertical_thresh = 10;
	int m_vertical_range = 10;

	String m_imgname;
	Mat m_img;
	Mat m_show;

  private:
	// 分割字符顺序：从左到右，从上往下
	// 因此根据竖直面投影划分出行
	// 再根据水平面投影来分割字符
	vector<int> m_vertical_projection;
	vector<Range> m_vertical_projection_range;
	vector<Rect> m_rect;
};

Rectangle::Rectangle()
{
	//
}

Rectangle::~Rectangle()
{
	//
}

void Rectangle::Read(const String &filename)
{
	m_imgname = filename;
	m_img = imread(filename, ImreadModes::IMREAD_GRAYSCALE);

	//黑字白底
	threshold(m_img, m_img, 0, 255, THRESH_BINARY | THRESH_OTSU);

	//竖直面投影
	m_vertical_projection.clear();
	m_vertical_projection.resize(m_img.rows, 0);
	for (int row = 0; row < m_img.rows; row++)
	{
		for (int col = 0; col < m_img.cols; col++)
		{
			if (m_img.at<uchar>(row, col) == 0)
				m_vertical_projection[row]++;
		}
	}
}

void Rectangle::Draw(int hthresh, int hrange, int vthresh, int vrange, bool force)
{
	auto projection_range_func = [](const vector<int> &projection, vector<Range> &projection_range, int thresh, int range) {
		projection_range.clear();
		Range temp(0, 0);
		for (int i = 0; i < projection.size(); i++)
		{
			if (projection[i] >= thresh)
			{
				if (temp.start == 0)
					temp.start = i;
				else
					continue;
			}
			else if (temp.start > 0)
			{
				if (i - temp.start > range)
				{
					temp.end = i;
					projection_range.emplace_back(temp);
				}

				temp.start = 0;
				temp.end = 0;
			}
		}
	};

	//竖直面投影
	if (vthresh != m_vertical_thresh || vrange != m_vertical_range || force)
	{
		m_vertical_thresh = vthresh;
		m_vertical_range = vrange;
		projection_range_func(
			m_vertical_projection,
			m_vertical_projection_range,
			m_vertical_thresh,
			m_vertical_range);
	}

	//水平面
	if (hthresh != m_horizontal_thresh || hrange != m_horizontal_range || vthresh != m_vertical_thresh || vrange != m_vertical_range || force)
	{
		m_show = m_img.clone();
		m_rect.clear();
		m_horizontal_thresh = hthresh;
		m_horizontal_range = hrange;

		for (int i = 0; i < m_vertical_projection_range.size(); i++)
		{
			//条状图片
			Mat line = m_img(Rect(0, m_vertical_projection_range[i].start, m_img.cols, m_vertical_projection_range[i].size())).clone();

			//条状图片水平投影
			vector<int> line_horizontal_projection(line.cols, 0);
			for (int col = 0; col < line.cols; col++)
			{
				for (int row = 0; row < line.rows; row++)
				{
					if (line.at<uchar>(row, col) == 0)
						line_horizontal_projection[col]++;
				}
			}

			//条状图片水平投影范围
			vector<Range> line_horizontal_projection_range;
			projection_range_func(
				line_horizontal_projection,
				line_horizontal_projection_range,
				m_horizontal_thresh,
				m_horizontal_range);

			//绘制矩形
			for (int m = 0; m < line_horizontal_projection_range.size(); m++)
			{
				Rect r(
					line_horizontal_projection_range[m].start,
					m_vertical_projection_range[i].start,
					line_horizontal_projection_range[m].size(),
					m_vertical_projection_range[i].size());
				m_rect.emplace_back(r);
				rectangle(m_show, r, Scalar(0));
			}
		}
	}
}

void Rectangle::Save()
{
	//
}

void Rectangle::Cut()
{
	//
}

//-----------------------------------------------------------------------------

class AppRectangle
{
  public:
	AppRectangle() {}
	~AppRectangle() {}
	void Run(const String &filename, bool show);

  private:
	static void TrackbarCallbackTypeHorizontalThresh(int pos, void *userdata);
	static void TrackbarCallbackTypeHorizontalRange(int pos, void *userdata);
	static void TrackbarCallbackTypeVerticalThresh(int pos, void *userdata);
	static void TrackbarCallbackTypeVerticalRange(int pos, void *userdata);
	static void ButtonCallbackSave(int state, void *userdata);
	static void ButtonCallbackCut(int state, void *userdata);

  private:
	Rectangle m_rectangle;
};

void AppRectangle::Run(const String &filename, bool show)
{
	m_rectangle.Read(filename);
	m_rectangle.Draw(
		m_rectangle.m_horizontal_thresh,
		m_rectangle.m_horizontal_range,
		m_rectangle.m_vertical_thresh,
		m_rectangle.m_vertical_range,
		true);

	if (show)
	{
		namedWindow(m_rectangle.m_imgname, WINDOW_NORMAL);

		int pos_horizontal_thresh = m_rectangle.m_horizontal_thresh;
		int count_horizontal_thresh = 100;
		createTrackbar("水平投影最低像素", "", &pos_horizontal_thresh, count_horizontal_thresh, TrackbarCallbackTypeHorizontalThresh, (void *)this);

		int pos_horizontal_range = m_rectangle.m_horizontal_range;
		int count_horizontal_range = 100;
		createTrackbar("水平投影最低范围", "", &pos_horizontal_range, count_horizontal_range, TrackbarCallbackTypeHorizontalRange, (void *)this);

		int pos_vertical_thresh = m_rectangle.m_vertical_thresh;
		int count_vertical_thresh = 100;
		createTrackbar("竖直投影最低像素", "", &pos_vertical_thresh, count_vertical_thresh, TrackbarCallbackTypeVerticalThresh, (void *)this);

		int pos_vertical_range = m_rectangle.m_vertical_range;
		int count_vertical_range = 100;
		createTrackbar("竖直投影最低范围", "", &pos_vertical_range, count_vertical_range, TrackbarCallbackTypeVerticalRange, (void *)this);

		createButton("保存配置", ButtonCallbackSave, (void *)this);

		createButton("切割图片", ButtonCallbackCut, (void *)this);

		// 非空格关闭后再显示
		do
		{
			imshow(m_rectangle.m_imgname, m_rectangle.m_show);
		} while (waitKey() != 32);
	}
	else
	{
		m_rectangle.Cut();
	}
}

void AppRectangle::TrackbarCallbackTypeHorizontalThresh(int pos, void *userdata)
{
	AppRectangle *thisobj = (AppRectangle *)userdata;
	thisobj->m_rectangle.Draw(
		pos,
		thisobj->m_rectangle.m_horizontal_range,
		thisobj->m_rectangle.m_vertical_thresh,
		thisobj->m_rectangle.m_vertical_range);
	imshow(thisobj->m_rectangle.m_imgname, thisobj->m_rectangle.m_show);
}

void AppRectangle::TrackbarCallbackTypeHorizontalRange(int pos, void *userdata)
{
	AppRectangle *thisobj = (AppRectangle *)userdata;
	thisobj->m_rectangle.Draw(
		thisobj->m_rectangle.m_horizontal_thresh,
		pos,
		thisobj->m_rectangle.m_vertical_thresh,
		thisobj->m_rectangle.m_vertical_range);
	imshow(thisobj->m_rectangle.m_imgname, thisobj->m_rectangle.m_show);
}

void AppRectangle::TrackbarCallbackTypeVerticalThresh(int pos, void *userdata)
{
	AppRectangle *thisobj = (AppRectangle *)userdata;
	thisobj->m_rectangle.Draw(
		thisobj->m_rectangle.m_horizontal_thresh,
		thisobj->m_rectangle.m_horizontal_range,
		pos,
		thisobj->m_rectangle.m_vertical_range);
	imshow(thisobj->m_rectangle.m_imgname, thisobj->m_rectangle.m_show);
}

void AppRectangle::TrackbarCallbackTypeVerticalRange(int pos, void *userdata)
{
	AppRectangle *thisobj = (AppRectangle *)userdata;
	thisobj->m_rectangle.Draw(
		thisobj->m_rectangle.m_horizontal_thresh,
		thisobj->m_rectangle.m_horizontal_range,
		thisobj->m_rectangle.m_vertical_thresh,
		pos);
	imshow(thisobj->m_rectangle.m_imgname, thisobj->m_rectangle.m_show);
}

void AppRectangle::ButtonCallbackSave(int state, void *userdata)
{
	AppRectangle *thisobj = (AppRectangle *)userdata;
	thisobj->m_rectangle.Save();
}

void AppRectangle::ButtonCallbackCut(int state, void *userdata)
{
	AppRectangle *thisobj = (AppRectangle *)userdata;
	thisobj->m_rectangle.Cut();
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	printf("cwd = %s\n", utils::fs::getcwd().c_str());

	AppRectangle app;
	app.Run(argv[1], true);

	return 0;
}