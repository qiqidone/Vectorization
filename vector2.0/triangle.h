#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <cv.h>
#include <istream>
#include <highgui.h>
#include "common.h"

using namespace std;
/* Vline 边结构 */
class VLine
{
public:
     float y_max;
     float deltaX;              /* deltaX per y */
     int step;

public:
     VLine(CvPoint2D32f p1, CvPoint2D32f p2);
     ~VLine();
};

/* VTriangle 三角形 */
class VTriangle
{
private:
     CvPoint2D32f m_Point[3];
     CvScalar m_Color;
     int id;
public:
     static int Total;
public:
     VTriangle(const CvPoint2D32f points[]);
     VTriangle(const CvPoint points[]);
     ~VTriangle();
     void inline setColor(CvScalar color){ m_Color = color; }
     void inline setColor(int color[]){ m_Color = cvScalar(color[0], color[1], color[2]); }
     void draw(IplImage* img);           /* 绘制三角形 opencv cvPoligon*/
     void traversal_draw(IplImage* img, int size=1); /* 遍历绘制 测试用 */
     void traversal(IplImage* img=NULL); /* 采样 色彩拟合 */
     void superTraversal(IplImage* img=NULL, int size=1);
     /* 定义操作符重载 */
     /* bool operator<(const VTriangle& vr){ return 1; } */
     /* bool operator==(const VTriangle& vr){ return 1; } */
     /* 友元 friend */
     friend bool operator==(const VTriangle& vl, const VTriangle& vr);
     friend bool operator!=(const VTriangle& vl, const VTriangle& vr);
     friend bool operator<(const VTriangle& vl, const VTriangle& vr);
     friend ostream& operator<<(ostream& out, const VTriangle& t);
};


#endif /* _TRIANGLE_H_ */
