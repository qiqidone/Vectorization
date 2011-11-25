/*
  
**********************     Filename : radial.h  

**********************     Comment : radial point   

**********************     Author : Qiqidone 

**********************     Time : Thu Oct 13 14:26:47 2011 

*/


#ifndef _RADIAL_H_
#define _RADIAL_H_

#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>


namespace Radial{

#define DEBUG_RENDER 0
#ifdef uint
     typedef unsigned int uint
#endif


     using namespace std;

     const double ZERO = 0.00001;
     const double PI = 3.1415926;
     const double SIGMA_SQUARE = 0.5;
     inline double gauss(double x, double sig=0.5){ return 1./sqrt(2*PI)*exp(-x*x/sig); }

     struct Vec2d{
          double x, y;
          Vec2d(double _x=0, double _y=0){ x=_x; y=_y; }
          Vec2d operator+(const Vec2d &r) const { return Vec2d(x+r.x, y+r.y); }
          Vec2d operator-(const Vec2d &r) const { return Vec2d(x-r.x, y-r.y); }
          Vec2d operator*(double b) const { return Vec2d(x*b, y*b); }
          Vec2d operator%(const Vec2d& r) const { return Vec2d(y*r.x, x*r.y); }
          Vec2d mult(const Vec2d& r) const { return Vec2d(x*r.x, y*r.y); } /* 点乘 */
          Vec2d& norm(){ return *this = *this * (1./sqrt(x*x + y*y)); }
          Vec2d perpendicular() const { return Vec2d(y, -x); }       /* 顺时针旋转90 */
          double dot(const Vec2d& r) const { return x*r.x + y*r.y; } /* cross */
          double length_square() { return x*x + y*y; }
          double length() { return sqrt(x*x + y*y); }
     };
     
     class RadialColor
     {
     public:
          RadialColor(){}
          RadialColor(int* color);
          virtual ~RadialColor(){};
          void setColor(int* color);
          void getColor(int* color);
          void print();
     public:
          int val[4];
     };

     class RadialPoint;
     class RadialLine
     {
     public:
          RadialLine(int ids, int ide, int* colorl, int* colorr);
          virtual ~RadialLine(){};
          void print();
          double dist(double tx, double ty);
          double distance_p2l(double tx, double ty);
          double weight(double tx, double ty);
          void getColor(double tx, double ty, int* color);
          double calLength();
          bool finish_initial();
          Vec2d coordinate(double tx, double ty);
          void setGauss(double gs){ sigma = gs; }
     public:
          RadialColor Cl;
          RadialColor Cr;
          vector< RadialPoint>::iterator p_start;
          vector< RadialPoint>::iterator p_end;
          Vec2d v_start;
          Vec2d v_end;
          /* Vec2d vector_me; */
          double length;
          
          double sigma;             /* gauss size */
          
          int start;
          int end;
          bool is_finish_initialed;
     };

     class RadialPoint
     {
     public:
          RadialPoint(double _x, double _y);
          virtual ~RadialPoint(){};
          void getline(const int line);
          void print();
     public:
          double x;
          double y;
          vector< int > radial_line;
     };


     class RadialImage
     {
     public:
          RadialImage(int _width=512, int _height=512);
          virtual ~RadialImage();
          // function
          void read(char* filename);
          void render();
          void gpuRender();
          void gpuField();
          void exportImage();
          void run();
          void print();
          void setPixel(int h, int w, int* color);

          void getLineStart(float** lineStart);
          void getLineEnd(float** lineEnd);
          void getLineVector(float** lineVector);
          void getLeftColor(int** leftColor);
          void getRightColor(int** rightColor);
          void getPoint(float** point);
          void getImage(int** image);
     public:
          int width;
          int height;
          int **image;
          
          int renderType;
          vector< RadialPoint > radial_point;
          vector< RadialLine > radial_line;
     };


}

#endif /* _RADIAL_H_ */
