#ifndef _SHADER_H_
#define _SHADER_H_

#include <cv.h>
#include <highgui.h>
#include <vector>

using namespace cv;
using namespace std;

struct ColorPoint
{
	CvPoint m_Point;
	CvScalar m_Color;
	ColorPoint(int x, int y, CvScalar color)
	{
		m_Point.x = x;
		m_Point.y = y;
		m_Color = color;
	}
	ColorPoint(CvPoint point, CvScalar color)
	{
		m_Point = point;
		m_Color = color;
	}
};

typedef vector< vector<ColorPoint> > Segmentation;

class Shader
{
 public:
  Shader();
  ~Shader();
  bool initial(IplImage* image = NULL);
  bool trapBall( IplImage* scr, IplImage* dst=NULL,  int size=4);
  bool trangle( IplImage* scr, IplImage* dst=NULL );
  bool mesh( IplImage* scr, IplImage* dst =NULL );

 // 私有函数 不对外开放，防止随意调用
 private:
  bool ball(CvMat* seg, int x, int y, int idx, int size=4, int step=2);
  bool segment( const IplImage* scr, IplImage* dst);//scr采样，dst输出
  bool solve( IplImage* dst);//解方程并输出dst
  
 private:
  CvMat* m_seg;
  int m_BallMask[101][101]; // 中心在（50， 50）
  vector< vector<ColorPoint> > m_Segmentation;
};

#endif
