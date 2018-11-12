#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>

using namespace cv;
using namespace std;

string savefolder;
int h_min_thresh = 1; //10 
int h_min_range = 1; //10
int v_min_thresh = 1; //2
int v_min_range = 1; //10

#define V_PROJECT 1
#define H_PROJECT 2

typedef struct
{
	int begin;
	int end;

} char_range_t;

//获取文本的投影用于分割字符(垂直，水平)
int GetTextProjection(Mat &src, vector<int> &pos, int mode)
{
	if (mode == V_PROJECT)
	{
		for (int i = 0; i < src.rows; i++)
		{
			uchar *p = src.ptr<uchar>(i);
			for (int j = 0; j < src.cols; j++)
			{
				if (p[j] == 0)
				{
					pos[j]++;
				}
			}
		}
	}
	else if (mode == H_PROJECT)
	{
		for (int i = 0; i < src.cols; i++)
		{

			for (int j = 0; j < src.rows; j++)
			{
				if (src.at<uchar>(j, i) == 0)
				{
					pos[j]++;
				}
			}
		}
	}
	return 0;
}

//获取每个分割字符的范围，min_thresh：波峰的最小幅度，min_range：两个波峰的最小间隔
int GetPeekRange(vector<int> &vertical_pos, vector<char_range_t> &peek_range, int min_thresh = 2, int min_range = 10)
{
	int begin = 0;
	int end = 0;
	for (int i = 0; i < vertical_pos.size(); i++)
	{
		if (vertical_pos[i] > min_thresh && begin == 0)
		{
			begin = i;
		}
		else if (vertical_pos[i] > min_thresh && begin != 0)
		{
			continue;
		}
		else if (vertical_pos[i] < min_thresh && begin != 0)
		{
			end = i;
			if (end - begin >= min_range)
			{
				char_range_t tmp;
				tmp.begin = begin;
				tmp.end = end;
				peek_range.push_back(tmp);
			}
			begin = 0;
			end = 0;
		}
		else if (vertical_pos[i] < min_thresh || begin == 0)
		{
			continue;
		}
		else
		{
			//printf("raise error!\n");
		}
	}
	return 0;
}

inline void save_cut(const Mat &img, string str)
{
	char name[128] = {0};
	sprintf(name, "./%s/%s.jpg", savefolder.c_str(), str.c_str());
	imwrite(name, img);
}

inline void save_cut(const Mat &img, int id)
{
	//颜色反转
	InputArray src(img);
	OutputArray dst(img);
	bitwise_not(src, dst);
	//生成图片
	char name[128] = {0};
	sprintf(name, "./%s/%d.jpg", savefolder.c_str(), id);
	imwrite(name, dst.getMat());
}

//切割字符
int CutChar(Mat &img, const vector<char_range_t> &v_peek_range, const vector<char_range_t> &h_peek_range, vector<Mat> &chars_set)
{
	static int count = 0;
	Mat show_img = img.clone();
	cvtColor(show_img, show_img, CV_GRAY2BGR);
	for (int i = 0; i < v_peek_range.size(); i++)
	{
		Rect r(v_peek_range[i].begin, 0, v_peek_range[i].end - v_peek_range[i].begin, img.rows);
		rectangle(show_img, r, Scalar(255, 0, 0), 2);
		Mat single_char = img(r).clone();
		chars_set.push_back(single_char);
		save_cut(single_char, count);
		count++;
	}

	//imshow("cut", show_img);
	save_cut(show_img, "dst");
	waitKey();

	return 0;
}

Mat cut_one_line(const Mat &src, int begin, int end)
{
	//printf("end:%d  begin:%d\n", end, begin);
	Mat line = src(Rect(0, begin, src.cols, end - begin)).clone();
	return line;
}

vector<Mat> CutSingleChar(Mat &img)
{
	//resize(img, img, Size(), 1.5, 1.5, INTER_LANCZOS4);
	threshold(img, img, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	//imshow("binary", img);
	save_cut(img, "src");
	vector<int> horizion_pos(img.rows, 0);
	vector<char_range_t> h_peek_range;
	GetTextProjection(img, horizion_pos, H_PROJECT);
	GetPeekRange(horizion_pos, h_peek_range, h_min_thresh, h_min_range);

	/*将每一文本行切割*/
	vector<Mat> lines_set;
	for (int i = 0; i < h_peek_range.size(); i++)
	{
		Mat line = cut_one_line(img, h_peek_range[i].begin, h_peek_range[i].end);
		lines_set.push_back(line);
	}

	vector<Mat> chars_set;
	for (int i = 0; i < lines_set.size(); i++)
	{
		Mat line = lines_set[i];
		vector<int> vertical_pos(line.cols, 0);
		vector<char_range_t> v_peek_range;
		GetTextProjection(line, vertical_pos, V_PROJECT);
		GetPeekRange(vertical_pos, v_peek_range, v_min_thresh, v_min_range);
		CutChar(line, v_peek_range, h_peek_range, chars_set);
	}
	return chars_set;
}

int main(int argc, char *argv[])
{
	if(argc > 1)
	{
		// argv[1] 图片名称提取出来作为文件夹名称
		savefolder.clear();
		savefolder.append(argv[1]);
		size_t folder_b = 0;
		size_t folder_e = savefolder.find_last_of(".");
		if (folder_e < 0 || folder_e > savefolder.size())
			folder_e = 0;
		savefolder = savefolder.substr(folder_b, folder_e - folder_b);
		DIR *dir = opendir(savefolder.c_str());
		if (NULL != dir)
		{
			closedir(dir);
			string cmd("rm -rf ");
			cmd += savefolder;
			system(cmd.c_str());
		}
		mkdir(savefolder.c_str(), S_IFDIR | S_IRWXU | S_IRWXG | S_IRWXO);
	}
	if(argc > 2)
		h_min_thresh = atoi(argv[2]);
	if(argc > 3)
		h_min_range = atoi(argv[3]);
	if(argc > 4)
		v_min_thresh = atoi(argv[4]);
	if(argc > 5)
		v_min_range = atoi(argv[5]);

	//
	Mat img = imread(argv[1], 0);
	resize(img, img, Size(), 2, 2);
	vector<Mat> chars_set = CutSingleChar(img);

	waitKey();
	return 0;
}
