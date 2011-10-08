#ifndef _BSPLINE_H_
#define _BSPLINE_H_

#include<vector>
#include"point.h"
using namespace std;
class BSplineSurface
{
public:
     BSpline(){}
     ~BSpline(){}
     void base(int order, float t, int npts, float n[]); /* 求基 */
     void fit(vector<VPoint>& points);
     /* member */
     vector<VPoint> m_ctrlPoints; /* 控制顶点 */
     int m_ctrlNum;             /* 控制顶点数 */
     int m_order;               /* 曲面次数 */
     
};

#endif /* _BSPLINE_H_ */
