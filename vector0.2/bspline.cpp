#include"bspline.h"

void BSplineSurface::basis(int order, float t, int npts, float n[])
{
	int nplusc = npts + order;//节点向量数目
	vector<float> temp(nplusc+1);

    /* calculate the first order basis functions n[i][1]*/
	for (int i = 1; i<= nplusc-1; i++)
	{
    	if (( t >= m_knotVector[i]) && (t < m_knotVector[i+1]))
			temp[i] = 1;
	    else
			temp[i] = 0;
	}

   /* calculate the higher order basis functions */
	float d,e;
	for (int k = 2; k <= m_order; k++)
	{
    	for (int i = 1; i <= nplusc-k; i++)
		{
        	if (temp[i] != 0)    /* if the lower order basis function is zero skip the calculation */
           		d = ((t-m_knotVector[i])*temp[i])/(m_knotVector[i+k-1]-m_knotVector[i]);
	        else
				d = 0;

    	    if (temp[i+1] != 0)     /* if the lower order basis function is zero skip the calculation */
        		e = ((m_knotVector[i+k]-t)*temp[i+1])/(m_knotVector[i+k]-m_knotVector[i+1]);
	        else
    			e = 0;

    	    temp[i] = d + e;
		}
	}

	if (t == (float)m_knotVector[nplusc])		/*    pick up last point	*/
 		temp[npts] = 1;

    /* put in n array	*/
	for (int i = 1; i <= npts; i++) {
    	n[i] = temp[i];
	}
}

// 输入的是待求解的points，输出的是m_ctrlPoints
// points是已经处理过的点
// 
void BSplineSurface::fit(vector<VPoint>& points)
{
     int bn = m_ctrlNum;        //控制顶点数目，即几维beizer
     int n = bn * bn;           // 未知数数目，即所有的控制顶点
     int m = points.size();        // 输入顶点数目,即方程总数
     // 构造矩阵A:m*n 下标从1开始
     float **A = new float*[m+1];
     for(int i=0; i<m+1; ++i)
     {
          A[i] = new float[n+1];
     }
     for (int row = 0; row < m+1; ++row)
     {
          for (int col = 0; col < n; ++col)
               A[row][col] = 0;
     }
     // 储存基函数
     float **N = new float*[m+1]; // for u
     float **M = new float*[m+1]; // for v
     for (int row = 0; row < m+1; ++row)
     {
          N[row] = new float[bn+1];
          M[row] = new float[bn+1];
     }
     //初始化
     for (int row = 0; row < m+1; ++row)
     {
          for (int col = 0; col < bn+1; ++col)
          {
               N[row][col] = 0;
               M[row][col] = 0;
          }
     }
     // 对于每个拟合的候选点
     // 求得矩阵A
     for(int row=0; row<m+1; ++row)
     {
          // 活得
          double u;
          double v;
          basis(m_order, u, bn, N[row]);
          basis(m_order, v, bn, M[row]);
          
          int col = 1;
          for (int i = 1; i <= bn; ++i) 
          {
               for (int j = 1; j <= bn; ++j)
               {
                    A[row][col] = N[row][i] * M[row][j];
                    ++col;
               }
          }
     }

     //方程右边已知列向量 AD=P
     float *P = new float[m+1];
     float *D = new float[n+1];
     for (int i = 0; i < 3; ++i)
     {
          for (int row = 1; row < m+1; ++row)
          {
               // VPoint::getElement 得到xyz
               P[row] = points[row-1].getElement(i+1); // row-1 正好对应相应的point[j] 
          }
          // 解方程,用CV的还是其他的？Adapter
          // A D P 从1开始
          common::solveLinearSystem(n, m, A, P, D);
          //赋值控制顶点
          for(int p=0; p<n; ++p)
          {
               // 要么VPoint 重载操作符[] 要么写函数set(i,val);
               // VPoint::setElement 设置xyz
               m_ctrlPoints[p].setElement(i, D[p+1] ); // D是从1开始的,而p从0开始
          }
     }
}     
