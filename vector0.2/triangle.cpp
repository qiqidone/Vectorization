#include"triangle.h"
#include<list>
#include<set>
#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;

//
// 操作符重载 包括CvPoint2D32f 、 VTriangle
//只有VTriangle使用，所以放在此cpp中
// 如果要inline的话，必须写在.h里面

bool operator==(const CvPoint2D32f& vl, const CvPoint2D32f& vr)
{
     return fabs(vl.x-vr.x) <= ZERO &&
          fabs(vl.y-vr.y) <=ZERO;
}
bool operator!=(const CvPoint2D32f& vl, const CvPoint2D32f& vr)
{
     return !(vl == vr);
}
bool operator<(const CvPoint2D32f& vl, const CvPoint2D32f& vr)
{
     if( fabs(vl.y - vr.y) <= ZERO )
          return vl.x - vr.x < ZERO;
     return vl.y - vr.y < ZERO;
}
// CvPoint2D32f标准输出
ostream& operator<<(ostream& out, const CvPoint2D32f& t)
{
     // 标准化输出不要输出endl 让用户去做
     out << "(" << t.x << ", " << t.y << ")";
     return out;
}
// 判断颜色相似
// 阀值设为 5
bool operator==(const CvScalar l, const CvScalar r)
{
     return fabs(l.val[0] - r.val[0]) <= COLOR_DELTA &&
          fabs(l.val[1] - r.val[1]) <= COLOR_DELTA &&
          fabs(l.val[2] - r.val[2]) <= COLOR_DELTA &&
          fabs(l.val[3] - r.val[3]) <= COLOR_DELTA;          
}

bool operator==(const VTriangle& vl, const VTriangle& vr)
{
     //设定为友元函数
     return vl.m_Point[0] == vr.m_Point[0] &&
          vl.m_Point[1] == vr.m_Point[1] &&
          vl.m_Point[2] == vr.m_Point[2];
}
bool operator!=(const VTriangle& vl, const VTriangle& vr)
{
     return !(vl == vr);
}
// 根据最小的y判断，用于sort
bool operator<(const VTriangle& vl, const VTriangle& vr)
{
     if(vl.m_Point[0] == vr.m_Point[0])
     {
          if(vl.m_Point[1] == vr.m_Point[1])
          {
               return vl.m_Point[2] < vr.m_Point[2];
          }
          else
          {
               return vl.m_Point[1] < vr.m_Point[1];
          }
     }
     else
     {
          return vl.m_Point[0] < vr.m_Point[0];
     }
}

// VTriangle标准输出操作
ostream& operator<<(ostream& out, const VTriangle& t)
{
     out << t.m_Point[0] << " " << t.m_Point[1] << " " << t.m_Point[2];
     return out;
}


bool gt(const CvPoint2D32f& p1, const CvPoint2D32f& p2)
{
     return !(p1 < p2);
}

// 判断函数
// 颜色相近
bool is_similar(const VTriangle& vl, const VTriangle& vr)
{
     return vl.m_Color == vr.m_Color;
}
// 距离相近，意味共用一边
// 暂时先使用set，6次插入，不知道还有没有更好的选择
// 还要测试
bool is_neighbour(const VTriangle& vl, const VTriangle& vr)
{
     set<CvPoint2D32f> set_points;
     for (int i = 0; i < 3; ++i)
     {
          set_points.insert(vl.m_Point[i]);
          set_points.insert(vr.m_Point[i]);
     }
     return set_points.size() <= 4;
}


// 判断是否归类的函数 取名groupon
bool groupon(const VTriangle& vl, const VTriangle& vr)
{
     if( !is_neighbour(vl, vr) )
          return false;
     if( !is_similar(vl, vr) )
          return false;
     return true;
}
// VTriangle

int VTriangle::Total = 0;

VTriangle::VTriangle(const CvPoint2D32f points[])
{
     id = VTriangle::Total++;
     group = -1;
     m_Point[0] = points[0];
     m_Point[1] = points[1];
     m_Point[2] = points[2];
     m_Color = cvScalar(200,200,200);
     sort(m_Point,m_Point+3,gt);
     
     // for(int i=0; i<3; i++)
     // {
     //      cout <<  __FUNCTION__ << "\t" << id << "\t" <<  m_Point[i].y  << endl;
     // }
          
}
VTriangle::VTriangle(const CvPoint points[])
{
     id = VTriangle::Total++;
     group = -1;
     m_Point[0] = cvPointTo32f(points[0]);
     m_Point[1] = cvPointTo32f(points[1]);
     m_Point[2] = cvPointTo32f(points[2]);
     m_Color = cvScalar(200,200,200);
     sort(m_Point,m_Point+3,gt);
     // for(int i=0; i<3; i++)
     // {
     //      cout <<  __FUNCTION__ << "\t" << id << "\t" <<  m_Point[i].y  << endl;
     // }
}
VTriangle::~VTriangle()
{
     VTriangle::Total --;
}
// 设置颜色
void VTriangle::setColor(VTriangle& vt)
{
     // CvScalar color = vt.getColor();
     m_Color = vt.getColor();
}

void VTriangle::print(ostream& out)
{
     out << m_Point[0] << " " << m_Point[1] << " " << m_Point[2];
}
// 采样函数
// 1.可以超采样 比如放大3倍
// 2.色彩可以不单一 使用拟合模型
void VTriangle::traversal(IplImage* img)
{
     if(img == NULL)
     {
          cout <<  __FUNCTION__ << "\t" << "img == NULL" << endl;
          return;
     }
     //
     int color[3] = {0};
     int total = 0;
     
     //开始遍历
      // y值如果相等 那么delta将是无限大
     float delta12, delta01, delta02;
     if(fabs(m_Point[0].y-m_Point[1].y) > ZERO)
          delta01 = (m_Point[0].x-m_Point[1].x) / (m_Point[0].y-m_Point[1].y);
     else
          delta01 = INFINITE;
     
     if(fabs(m_Point[0].y-m_Point[2].y) > ZERO)
          delta02 = (m_Point[0].x-m_Point[2].x) / (m_Point[0].y-m_Point[2].y);
     else
          delta02 = INFINITE;
     
     if(fabs(m_Point[1].y-m_Point[2].y) > ZERO)
          delta12 = (m_Point[1].x-m_Point[2].x) / (m_Point[1].y-m_Point[2].y);
     else
          delta12 = INFINITE;

     // up
     if(delta01 < INFINITE-1)
     {
          int y = (int)m_Point[0].y;
          float x1,x2;
          x1 = x2 = m_Point[0].x;
          while(y-m_Point[1].y >=ZERO)
          {
               int xl = x1>=x2 ? x2:x1;
               int xr = x1<x2 ? x2:x1;
               // cout << xl << " " << xr << endl;
               for(int i=xl; i<=xr; i++)
               {
                    if(i<img->width && i>=0 && y>=0 && y<img->height)
                    {
                         try{
                              color[0] += CV_IMAGE_ELEM(img,uchar,y,i*3+0);
                              color[1] += CV_IMAGE_ELEM(img,uchar,y,i*3+1);
                              color[2] += CV_IMAGE_ELEM(img,uchar,y,i*3+2);
                              total ++;
                          
                         }
                         catch(...){
                              cout <<  __FUNCTION__ << "\t" << "EXCEPT" << endl;
                              break;
                         }
                    }
                    
               }
               x1 -= delta01;
               x2 -= delta02;
               y--;
          }
     }
     
     // 2->1 down
     if(delta12 < INFINITE-1)
     {
          int y = (int)m_Point[2].y;
          float x1,x2;
          x1 = x2 = m_Point[2].x;
          while(y-m_Point[1].y <=ZERO)
          {
               float xl = x1>=x2 ? x2:x1;
               float xr = x1<x2 ? x2:x1;
               // cout << xl << " " << xr << endl;
               for(int i=(int)xl; i<=(int)xr; i++)
               {
                    try{
                         color[0] += CV_IMAGE_ELEM(img,uchar,y,i*3+0);
                         color[1] += CV_IMAGE_ELEM(img,uchar,y,i*3+1);
                         color[2] += CV_IMAGE_ELEM(img,uchar,y,i*3+2);
                         total ++;
                    }
                    catch(...){
                         cout <<  __FUNCTION__ << "\t" << "EXCEPT" << endl;
                         break;
                    }                   
               }
               x1 += delta12;
               x2 += delta02;
               y++;
          }
     }
     // total == 0 的情况不是不可能！特别注意
     if(total != 0)
          setColor(cvScalar(color[0]/total, color[1]/total, color[2]/total));
     else
          setColor( cvScalar(0,0,0) );
}
// 未考虑有两点在同一直线上
void VTriangle::superTraversal(IplImage* img, int size)
{
     if(img == NULL)
     {
          cout <<  __FUNCTION__ << "\t" << "img == NULL" << endl;
          return;
     }
     if(size <= 0)
     {
          cout <<  __FUNCTION__ << "\t" << "size <= 0" << endl;
          return;
     }
     //开始遍历

     // 基本参数
     int color[3] = {0};        // 初始颜色
     int total = 0;             // 总数
     float step = 1.0/size;     // 步伐
     // y值如果相等 那么delta将是无限大
     float delta12, delta01, delta02;
     if(fabs(m_Point[0].y-m_Point[1].y) > ZERO)
          delta01 = -(m_Point[0].x-m_Point[1].x) / (m_Point[0].y-m_Point[1].y);
     else
          delta01 = INFINITE;
     
     if(fabs(m_Point[0].y-m_Point[2].y) > ZERO)
          delta02 = -(m_Point[0].x-m_Point[2].x) / (m_Point[0].y-m_Point[2].y);
     else
          delta02 = INFINITE;
     
     if(fabs(m_Point[1].y-m_Point[2].y) > ZERO)
          delta12 = -(m_Point[1].x-m_Point[2].x) / (m_Point[1].y-m_Point[2].y);
     else
          delta12 = INFINITE;
     cout <<  __FUNCTION__ << "\t" << delta12  << endl;
     cout <<  __FUNCTION__ << "\t" << delta01 << endl;
     cout <<  __FUNCTION__ << "\t" << delta02 << endl;
     // 向上
 
     float deltaL = delta01 < delta02 ? delta01 : delta02;
     float deltaR = delta01 < delta02 ? delta02 : delta01;
     float x = m_Point[0].x;
     float y = m_Point[0].y;
     // assert(x>0);
     int n = 0;
     while(y - m_Point[1].y > ZERO)
     {
          float xl = (x + 1.0 * n * step * deltaL);
          float xr = (x + 1.0 * n * step * deltaR);
          if(delta01 > INFINITE - 1) break;
          assert(xl>=1e-10);
          float xi = xl;
          while(xi <= xr)
          {
               for(int q=0; q<3;q++)
                    color[q] += CV_IMAGE_ELEM(img,uchar,(int)y,(int)x*3+q);
               // cout <<  n * step * deltaL  << "up\t" << y << ": " << x  << endl;
               // cout <<  __FUNCTION__ << " " <<  total <<"\t" << color[0] << ", " << color[1] << ", " << color[2] << ", " << endl;
               total += 1;
               xi += step;
          }
          y -= step;
          n += 1;
     }
     
     // 向下 deltaL大 deltaR
     deltaL = delta12 > delta02 ? delta12 : delta02;
     deltaR = delta12 > delta02 ? delta02 : delta12;
     x = m_Point[2].x;
     y = m_Point[2].y;
     n = 0;
     while(y - m_Point[1].y < -ZERO )
     {
          float xl = (x - 1.0 * n * step * deltaL);
          float xr = (x - 1.0 * n * step * deltaR);
          float xi = xl;
          if(delta12 > INFINITE-1) break;
          // cout <<  __FUNCTION__ << "\t" << xl << endl;
          assert(xl>=1e-10);         // 断言失败的原因是两个Y可能相等
          while(xi <= xr)
          {
               for(int q=0; q<3;q++)
                    color[q] += CV_IMAGE_ELEM(img,uchar,(int)y,(int)x*3+q);
               total += 1;
               xi += step;
                // cout <<  __FUNCTION__ << "down\t" << y << ": " << x  << endl;
               // cout <<  __FUNCTION__ << " " <<  total <<"\t" << color[0] << ", " << color[1] << ", " << color[2] << ", " << endl;
          }
          y += step;
          n += 1;
     }
     // cout <<  __FUNCTION__ << " " <<  total <<"\t" << color[0] << ", " << color[1] << ", " << color[2] << ", " << endl;
     
      if(total != 0){
          color[0] /= total;
          color[1] /= total;
          color[2] /= total;
      
           cout <<  __FUNCTION__ << total <<"\t" << color[0] << ", " << color[1] << ", " << color[2] << ", " << endl;
     }
     
     setColor( cvScalar(color[0], color[1], color[2]) );
}

// 扫描线绘制
void VTriangle::traversal_draw(IplImage* img,int size)
{
     if(img == NULL)
     {
          cout <<  __FUNCTION__ << "\t" << "img == NULL" << endl;
          return;
     }
     // 
     //开始遍历
      // y值如果相等 那么delta将是无限大
     float delta12, delta01, delta02;
     if(fabs(m_Point[0].y-m_Point[1].y) > ZERO)
          delta01 = (m_Point[0].x-m_Point[1].x) / (m_Point[0].y-m_Point[1].y);
     else
          delta01 = INFINITE;
     
     if(fabs(m_Point[0].y-m_Point[2].y) > ZERO)
          delta02 = (m_Point[0].x-m_Point[2].x) / (m_Point[0].y-m_Point[2].y);
     else
          delta02 = INFINITE;
     
     if(fabs(m_Point[1].y-m_Point[2].y) > ZERO)
          delta12 = (m_Point[1].x-m_Point[2].x) / (m_Point[1].y-m_Point[2].y);
     else
          delta12 = INFINITE;
     // cout <<  __FUNCTION__ << "\t" << delta12  << endl;
     // cout <<  __FUNCTION__ << "\t" << delta01 << endl;
     // cout <<  __FUNCTION__ << "\t" << delta02 << endl;

    
     // up
     if(delta01 < INFINITE-1)
     {
          int y = (int)m_Point[0].y;
          float x1,x2;
          x1 = x2 = m_Point[0].x;
          while(y-m_Point[1].y >=ZERO)
          {
               int xl = x1>=x2 ? x2:x1;
               int xr = x1<x2 ? x2:x1;
               // cout << xl << " " << xr << endl;
               for(int i=xl; i<=xr; i++)
               {
                    try{
                          CV_IMAGE_ELEM(img,uchar,y,i*3+1) = 200;
                    }
                    catch(...){
                         cout <<  __FUNCTION__ << "\t" << "EXCEPT" << endl;
                         break;
                    }                 
               }
               x1 -= delta01;
               x2 -= delta02;
               y--;
          }
     }
     
     // 2->1 down
     if(delta12 < INFINITE-1)
     {
          int y = (int)m_Point[2].y;
          float x1,x2;
          x1 = x2 = m_Point[2].x;
          while(y-m_Point[1].y <=ZERO)
          {
               float xl = x1>=x2 ? x2:x1;
               float xr = x1<x2 ? x2:x1;
               // cout << xl << " " << xr << endl;
               for(int i=(int)xl; i<=(int)xr; i++)
               {
                    try{
                         CV_IMAGE_ELEM(img,uchar,y,i*3+1) = 200;
                    }
                    catch(...){
                         cout <<  __FUNCTION__ << "\t" << "EXCEPT" << endl;
                         break;
                    }                   
               }
               x1 += delta12;
               x2 += delta02;
               y++;
          }
     }
     
}

// 利用cvFillPoly绘制
void VTriangle::draw(IplImage* img)
{
     
     if(img)
     {
          const int npts  = 3;
          CvPoint* points = new CvPoint[3];
          points[0] = cvPointFrom32f(m_Point[0]);
          points[1] = cvPointFrom32f(m_Point[1]);
          points[2] = cvPointFrom32f(m_Point[2]);

          cvFillPoly(
               img,
               &points,
               &npts,
               1,
               m_Color);

          delete[] points;
     }
     else
     {
          cout <<  __FUNCTION__ << "\t" << "img == NULL" << endl;
     }
 
}

