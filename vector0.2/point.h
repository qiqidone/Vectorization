/*
  
**********************     Filename : point.h  

**********************     Comment : VPoint   

**********************     Author : Qiqidone 

**********************     Time : Tue Sep 27 10:18:48 2011 

*/

#ifndef _POINT_H_
#define _POINT_H_


#include<iostream>
#include<vector>
#include<istream>
using namespace std;

class VPoint
{
public:
     VPoint(double _x=0, double _y=0, double _z=0, int _id=0):x(_x),y(_y),z(_z),id(_id){}
     virtual ~VPoint(){}
     double getElement(int i);
     void setElement(int i, double val);
public:
     double x;
     double y;
     double z;
     double u;
     double v;
     int id;
     bool is_inner;
     vector<int> connect; 
};

class VPoints
{
private:
     vector< VPoint > r_points;
     vector< VPoint > g_points;
     vector< VPoint > b_points;
     /* function */
public:
     VPoints(){}
     ~VPoints(){}
     void push_back(VPoint r, VPoint g, VPoint b);
     void print(ostream& out);  /* 打印 */
//     void 
};

#endif /* _POINT_H_ */
