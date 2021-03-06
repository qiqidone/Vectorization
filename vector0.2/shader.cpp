#include "shader.h"
#include <iostream>
//#include < cmath >
Shader::Shader()
{
     m_seg = NULL;
     // 做个ball的模板
     for(int i=0; i<=100; i++)
          for(int j=0; j<=100; j++)
          {
               m_BallMask[i][j] = (int)(sqrt( (i - 50) * (i - 50) + (j - 50) * (j - 50) ));
          }
}

Shader::~Shader()
{
     if(m_seg != NULL)
          cvReleaseMat( &m_seg );
}

// 每次更换图片时 需要调用
bool Shader::initial(IplImage* image)
{
     if(image == NULL)
          return false;

     if(m_seg != NULL)
     {
          cvReleaseMat( &m_seg );
          m_seg = NULL;    
     }

     m_seg = cvCreateMat(image->height, image->width,  CV_32SC1);
     cvConvertScale(image, m_seg, 100); // Scale 设为100 如果需要可以修改放大倍数！

  
     return true;
}

bool Shader::ball(CvMat* seg, int x, int y, int idx, int size, int step)
{
     if(x-size < 0 || y-size < 0 || x+size >= seg->cols || y+size >= seg->rows)
          return false;

     bool flag = false; // 标志，判断该圆是否全部都已经被填充
     for(int i=-size; i<=size; i++)
          for(int j=-size; j<=size; j++)
          {
               // 越界处理
               if(y+i >=seg->rows || y+i < 0 || x+j >=seg->cols || x+j < 0)
                    return false;
               // 先判断是否在size的半径内，再在不为0 以及不属于同一区域时，跳出结束
               if(m_BallMask[50+i][50+j] > size)
                    continue;
               if( CV_MAT_ELEM(*seg, int, y+i, x+j) == 0 )
                    flag = true;
               else if( CV_MAT_ELEM(*seg, int, y+i, x+j) != idx )
  					return false;
						
          }
     //	cout << x << ", " << y << " @ SHBall "  << flag << endl;
     if(!flag)
          return false;
     // 能从上边出来，说明该圆没有滚到其他区域，也没有滚到边缘，而且有未着色区域 可以归属一区
     for(int i=-size; i<=size; i++)
          for(int j=-size; j<=size; j++)
          {
               // 越界处理
               if(y+i >=seg->rows || y+i < 0 || x+j >=seg->cols || x+j < 0)
                    continue;
               // 先判断是否在size的半径
               if(m_BallMask[50+i][50+j] > size)
                    continue;
               // 赋值
               CV_MAT_ELEM(*seg, int, y+i, x+j) = idx;
               //			   cout << i << ", " << j << "@ Trappedball "  << endl;
          }

     // 上下左右，游走步长
     ball(seg, x+step, y, idx, size, step);
     ball(seg, x-step, y, idx, size, step);
     ball(seg, x, y+step, idx, size, step);
     ball(seg, x, y-step, idx, size, step);
     //	   cout << x << ", " << y << " OK @ Trappedball "  << endl;
     return true;
}
bool Shader::segment( const IplImage* scr, IplImage* dst )
{
     if(m_seg == NULL || scr == NULL)
          return false;
     if(dst == NULL)
          cvCreateImage(cvGetSize(scr), 8, 3);
     if(m_Segmentation.size() <= 0)
          return false;
     //		cout << "Seg Size : " << m_Segmentation.size() << endl;
		
     // 利用Map分类 并查集可以试下
     map<int, int> map;
     int idx = 0;	
     for(int i=0; i<m_seg->rows; i++)
     {
          for(int j=0; j<m_seg->cols; j++)
          {
               // map中有则插入到m_Segmentation， 没有则新建索引
               int elem = CV_MAT_ELEM(*m_seg, int, i, j);
               //排除边缘和未分区域
               if(elem == 0 || elem == 25500)
                    continue;
               //		cout << "elem : " << elem << endl;
               if(map.count(elem))
               {
				
                    ColorPoint cp( j, i, cvGet2D(scr, i, j) ); // 要注意x y参数的位置
                    //				cout << i << "\t"<< j << "\t" << elem << "\t" << map[elem] << endl;
                    if(map[elem] >= m_Segmentation.size())
                    {
                         cout << "map[elem] >= m_Segmentation.size() @ SHSegment(const" << endl;
                         return false;
                    }
                    else
                         m_Segmentation[ map[ elem ] ].push_back(cp);
                    //	cout << "Here" << endl;
               }
               else
               {
                    map.insert( pair<int, int>(elem, idx++) ); // idx 从0开始，否则出错
               }
          }
     }

     solve(dst);
	
     return true;
}
bool Shader::solve( IplImage* dst )
{
     if(dst == NULL)
          return false;
     // 三通道 三个方程根
     CvMat* dst1 = cvCreateMat(6, 1, CV_32FC1);
     CvMat* dst2 = cvCreateMat(6, 1, CV_32FC1);
     CvMat* dst3 = cvCreateMat(6, 1, CV_32FC1);
     for(int i = 0; i < m_Segmentation.size(); i++)
     {
          // 开的空间可能过大
          if(m_Segmentation[i].size() <= 0)
               continue;
          CvMat* A = cvCreateMat(m_Segmentation[i].size(), 6, CV_32FC1);
          CvMat* B1 = cvCreateMat(m_Segmentation[i].size(), 1, CV_32FC1);
          CvMat* B2 = cvCreateMat(m_Segmentation[i].size(), 1, CV_32FC1);
          CvMat* B3 = cvCreateMat(m_Segmentation[i].size(), 1, CV_32FC1);
          //	cout << "Ok Here ss" << endl;
          for(int j = 0; j < m_Segmentation[i].size(); j++)
          {
               int x = m_Segmentation[i][j].m_Point.x;
               int y = m_Segmentation[i][j].m_Point.y;
               double color1 = m_Segmentation[i][j].m_Color.val[0];
               double color2 = m_Segmentation[i][j].m_Color.val[1];
               double color3 = m_Segmentation[i][j].m_Color.val[2];
			
               float* pMatA = (float*)(A->data.fl + j * A->cols);
               pMatA[0] = 1;
               pMatA[1] = x;
               pMatA[2] = y;
               pMatA[3] = x * x;
               pMatA[4] = y * y;
               pMatA[5] = x * y;
            
               float* pMatB1 = (float*)(B1->data.fl + j * B1->cols);
               pMatB1[0] = color1;
               float* pMatB2 = (float*)(B2->data.fl + j * B2->cols);
               pMatB2[0] = color2;
               float* pMatB3 = (float*)(B3->data.fl + j * B3->cols);
               pMatB3[0] = color3;
			
          }

          cvSolve(A, B1, dst1, CV_SVD);
          cvSolve(A, B2, dst2, CV_SVD);
          cvSolve(A, B3, dst3, CV_SVD);
        
          //	cout << "Ok Here Solve～" << endl;
          // 求出参数后着色给dst: CV_8UC3
          for(int j=0; j<m_Segmentation[i].size(); j++)
          {
               int x = m_Segmentation[i][j].m_Point.x;
               int y = m_Segmentation[i][j].m_Point.y;

               uchar* pDst = (uchar* )(dst->imageData + y * dst->widthStep + x * 3);	
               //   
               float* para1 = (float*)(dst1->data.fl);
               pDst[0] = (uchar)(para1[0] + para1[1] * x + para1[2] * y +para1[3] * x * x +para1[4] * y * y +para1[5] * x * y);

               float* para2 = (float*)(dst2->data.fl);
               pDst[1] = (uchar)(para2[0] + para2[1] * x + para2[2] * y +para2[3] * x * x +para2[4] * y * y +para2[5] * x * y);

               float* para3 = (float*)(dst3->data.fl);
               pDst[2] = (uchar)(para3[0] + para3[1] * x + para3[2] * y +para3[3] * x * x +para3[4] * y * y +para3[5] * x * y);

          }

          cvReleaseMat(&A);
          cvReleaseMat(&B1);
          cvReleaseMat(&B2);
          cvReleaseMat(&B3);
     }

     return true;
}

// trapBall 是主函数，对外开放
bool Shader::trapBall( IplImage* scr, IplImage* dst,  int size )
{
     if(m_seg == NULL)
     {
          cout << "m_seg == NULL @ Trappedball" << endl;
          return false;
     }
     if(scr == NULL)
     {
          cout << "scr == NULL @ Trappedball" << endl;
          return false;
     }
		
     int idx = 1; // idx 记录 该region的标志，也便于统计总的m_seg数目,idx 不能从0开始

     while(size >= 2) //直到size变成0 ，让整个ball能滚遍所有区域
     {
          for(int i=0; i<m_seg->rows; i++)
               for(int j=0; j<m_seg->cols; j++)
               {
					if( CV_MAT_ELEM(*m_seg, int, i, j) != 0 )
                         continue;
					else
                         if( ball(m_seg, j, i, idx, size, 2) ) //ball 函数是 x y ，这里注意 应该是 j i 
                         {
                              idx ++;
                              // 想看看这步完成了什么
                              //	SHWriteVideo(m_seg);
                              //	cvWaitKey(50);
                         }
               }
          // 每分一次，就grow一次，但是一定要确保这个grow不太太过分，否则分区少了，细节也没了.任何事情有得有失
          size /= 2; // 论文里是每次 -1
            
          break;          
     }

     // trapall之后的初始化m_Segmentation
     m_Segmentation.clear();
     m_Segmentation.resize(idx+1);

     // dst == NULL 说明不想在这里完成solve操作
     if(dst == NULL)
          return true;
     else
     {
          segment(scr, dst);   
     }
        
        
     return true;
}

// trangle 三角化
bool Shader::trangle( IplImage* scr, IplImage* dst )
{
     if( scr == NULL )
     {
          cout << "scr == NULL @ Shader::trangle" << endl;
     }
     if( dst == NULL )
     {
          cout << "dst == NULL @ Shader::trangle" << endl;
     }

     
          
     return true;
}


bool Shader::mesh( IplImage* scr, IplImage* dst )
{
     IplImage* gray = NULL;
     if( scr->nChannels != 1)
      {
           gray = cvCreateImage( cvGetSize( scr), 8, 1);
           cvCvtColor( scr, gray, CV_BGR2GRAY );
      }
     if( dst == NULL )
          return false;
     else
          cvZero( dst );
     const static int step = 5;
     const static int delta = 5;
     const CvScalar color = cvScalar( 250, 250, 250 );
     enum direction
     {
          left,
          up,
          right,
          down
     };
     for( int i = step; i < gray->height-step; i += step )
     {
          for( int j=step; j< gray->width-step; j+= step )
               {
                    for (int x=1; x<step; ++x)
                    {
                         if( abs( (int)CV_IMAGE_ELEM(gray, uchar, i, j-x) - (int)CV_IMAGE_ELEM(gray, uchar, i, j) ) > delta )
                              {
                                   cvCircle( dst, cvPoint( j-x, i ), 1, color );
                                   break;
                              }
                              
                    }
                     for (int x=1; x<step; ++x)
                    {
                         if( abs( (int)CV_IMAGE_ELEM(gray, uchar, i, j+x) - (int)CV_IMAGE_ELEM(gray, uchar, i, j) ) > delta )
                              {
                                   cvCircle( dst, cvPoint( j+x, i ), 1, color );
                                   break;
                              }
                              
                    }
                      for (int x=1; x<step; ++x)
                    {
                         if( abs( (int)CV_IMAGE_ELEM(gray, uchar, i-x, j) - (int)CV_IMAGE_ELEM(gray, uchar, i, j) ) > delta )
                              {
                                   cvCircle( dst, cvPoint( j, i-x ), 1, color );
                                   break;
                              }
                              
                    }
                       for (int x=1; x<step; ++x)
                    {
                         if( abs( (int)CV_IMAGE_ELEM(gray, uchar, i+x, j) - (int)CV_IMAGE_ELEM(gray, uchar, i, j) ) > delta )
                              {
                                   cvCircle( dst, cvPoint( j, i+x ), 1, color );
                                   break;
                              }
                              
                    }
               }
                    
     }
          if( gray )
               cvReleaseImage( &gray );
               
          return true;
}
