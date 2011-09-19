
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <cv.h>
#include <istream>
#include <highgui.h>
#include "common.h"

using namespace std;

/* VTriangle 三角形 */
class VTriangle
{
private:
     CvPoint2D32f m_Point[3];
     CvScalar m_Color;
     int id;
     int group;
public:
     static int Total;
public:
     VTriangle(const CvPoint2D32f points[]);
     VTriangle(const CvPoint points[]);
     ~VTriangle();
     inline CvPoint2D32f* getPoint(){ return m_Point; }

     inline CvScalar getColor(){ return m_Color; }
     inline void setColor(CvScalar color){ m_Color = color; }
     inline void setColor(int color[]){ m_Color = cvScalar(color[0], color[1], color[2]); }
     void setColor(VTriangle& vt);

     bool is_grouped(){ return group > 0; }
     int get_group(){ return group; }
     void set_group(int g){ group = g; }

     void print(ostream& out);  /* 打印 */
     void draw(IplImage* img);           /* 绘制三角形 opencv cvPoligon*/
     void traversal_draw(IplImage* img, int size=1); /* 遍历绘制 测试用 */
     void traversal(IplImage* img=NULL); /* 采样 色彩拟合 */
     void superTraversal(IplImage* img=NULL, int size=1);
     /* 一些判定函数 设为友元*/
     friend bool is_similar(const VTriangle& vl, const VTriangle& vr);
     friend bool is_neighbour(const VTriangle& vl, const VTriangle& vr);
     /* 定义操作符重载 */
     /* bool operator<(const VTriangle& vr){ return 1; } */
     /* bool operator==(const VTriangle& vr){ return 1; } */
     /* 友元 friend */
     friend bool operator==(const VTriangle& vl, const VTriangle& vr);
     friend bool operator!=(const VTriangle& vl, const VTriangle& vr);
     friend bool operator<(const VTriangle& vl, const VTriangle& vr);
     friend ostream& operator<<(ostream& out, const VTriangle& t);
     /* 判断是否合并的函数 很关键！ */
     friend bool groupon(const VTriangle&vl, const VTriangle& vr);
};


#endif /* _TRIANGLE_H_ */
