#include"common.h"
#include"triangle.h"
#include <stdio.h>
#include <vector>

namespace common
{
     
     using namespace std;

     bool pred_equal(const CvPoint2D32f& vl, const CvPoint2D32f& vr)
     {
          return fabs(vl.x-vr.x) <= ZERO &&
               fabs(vl.y-vr.y) <=ZERO;
     }
     bool pred_cmp(const CvPoint2D32f& vl, const CvPoint2D32f& vr)
     {
          if( fabs(vl.y - vr.y) <= ZERO )
               return vl.x - vr.x < ZERO;
          return vl.y - vr.y < ZERO;
     }
     // 函数声明
     void triangulation(CvSubdiv2D* subdiv, IplImage* source);
     void printTriangle(list<VTriangle>& lt);
     void printPoint(list<CvPoint2D32f>& lp);
     void printPoint(vector<CvPoint2D32f>& lp);


     
     CvSubdiv2D* init_delaunay( CvMemStorage* storage, CvRect rect )
     {
          CvSubdiv2D* subdiv;

          subdiv = cvCreateSubdiv2D( CV_SEQ_KIND_SUBDIV2D, sizeof(*subdiv),
                                     sizeof(CvSubdiv2DPoint),
                                     sizeof(CvQuadEdge2D),
                                     storage );
          cvInitSubdivDelaunay2D( subdiv, rect );
          return subdiv;
     }
// 画出候选点
     void draw_subdiv_point( IplImage* img, CvPoint2D32f fp, CvScalar color )
     {
          cvCircle( img, cvPoint(cvRound(fp.x), cvRound(fp.y)), 3, color, CV_FILLED, 8, 0 );
     }
// 画 边
     void draw_subdiv_edge( IplImage* img, CvSubdiv2DEdge edge, CvScalar color )
     {
          CvSubdiv2DPoint* org_pt;
          CvSubdiv2DPoint* dst_pt;
          CvPoint2D32f org;
          CvPoint2D32f dst;
          CvPoint iorg, idst;

          org_pt = cvSubdiv2DEdgeOrg(edge);
          dst_pt = cvSubdiv2DEdgeDst(edge);

          if( org_pt && dst_pt )
          {
               org = org_pt->pt;
               dst = dst_pt->pt;

               iorg = cvPoint( cvRound( org.x ), cvRound( org.y ));
               idst = cvPoint( cvRound( dst.x ), cvRound( dst.y ));

               cvLine( img, iorg, idst, color, 1, CV_AA, 0 );
          }
     }
// 画边？
     void draw_subdiv( IplImage* img, CvSubdiv2D* subdiv,
                       CvScalar delaunay_color, CvScalar voronoi_color )
     {
          CvSeqReader  reader;
          int i, total = subdiv->edges->total;
          int elem_size = subdiv->edges->elem_size;

          cvStartReadSeq( (CvSeq*)(subdiv->edges), &reader, 0 );

          for( i = 0; i < total; i++ )
          {
               CvQuadEdge2D* edge = (CvQuadEdge2D*)(reader.ptr);

               if( CV_IS_SET_ELEM( edge ))
               {
                    draw_subdiv_edge( img, (CvSubdiv2DEdge)edge + 1, voronoi_color );
                    draw_subdiv_edge( img, (CvSubdiv2DEdge)edge, delaunay_color );
               }

               CV_NEXT_SEQ_ELEM( elem_size, reader );
          }
     }
     void locate_point( CvSubdiv2D* subdiv, CvPoint2D32f fp, IplImage* img,
                        CvScalar active_color )
     {
          CvSubdiv2DEdge e;
          CvSubdiv2DEdge e0 = 0;
          CvSubdiv2DPoint* p = 0;

          cvSubdiv2DLocate( subdiv, fp, &e0, &p );

          if( e0 )
          {
               e = e0;
               do
               {
                    draw_subdiv_edge( img, e, active_color );
                    e = cvSubdiv2DGetEdge(e,CV_NEXT_AROUND_LEFT);
               }
               while( e != e0 );
          }

          draw_subdiv_point( img, fp, active_color );
     }
// 画面
     void draw_subdiv_facet( IplImage* img, CvSubdiv2DEdge edge )
     {
          CvSubdiv2DEdge t = edge;
          int i, count = 0;
          CvPoint* buf = 0;

          // count number of edges in facet
          do
          {
               count++;
               t = cvSubdiv2DGetEdge( t, CV_NEXT_AROUND_LEFT );
          } while (t != edge );

          buf = (CvPoint*)malloc( count * sizeof(buf[0]));

          // gather points
          t = edge;
          for( i = 0; i < count; i++ )
          {
               CvSubdiv2DPoint* pt = cvSubdiv2DEdgeOrg( t );
               if( !pt ) break;
               buf[i] = cvPoint( cvRound(pt->pt.x), cvRound(pt->pt.y));
               t = cvSubdiv2DGetEdge( t, CV_NEXT_AROUND_LEFT );
          }

          if( i == count )
          {
               CvSubdiv2DPoint* pt = cvSubdiv2DEdgeDst( cvSubdiv2DRotateEdge( edge, 1 ));
               cvFillConvexPoly( img, buf, count, CV_RGB(rand()&255,rand()&255,rand()&255), CV_AA, 0 );
               cvPolyLine( img, &buf, &count, 1, 1, CV_RGB(0,0,0), 1, CV_AA, 0);
               draw_subdiv_point( img, pt->pt, CV_RGB(0,0,0));
          }
          free( buf );
     }

     void paint_voronoi( CvSubdiv2D* subdiv, IplImage* img )
     {
          CvSeqReader  reader;
          int i, total = subdiv->edges->total;
          int elem_size = subdiv->edges->elem_size;

          cvCalcSubdivVoronoi2D( subdiv );

          cvStartReadSeq( (CvSeq*)(subdiv->edges), &reader, 0 );

          for( i = 0; i < total; i++ )
          {
               CvQuadEdge2D* edge = (CvQuadEdge2D*)(reader.ptr);

               if( CV_IS_SET_ELEM( edge ))
               {
                    CvSubdiv2DEdge e = (CvSubdiv2DEdge)edge;
                    // left
                    draw_subdiv_facet( img, cvSubdiv2DRotateEdge( e, 1 ));

                    // right
                    draw_subdiv_facet( img, cvSubdiv2DRotateEdge( e, 3 ));
               }

               CV_NEXT_SEQ_ELEM( elem_size, reader );
          }
     }

     void tmp_triangles(CvSubdiv2D* subdiv, IplImage* img)   
     {   
          CvSeqReader  reader;   
          int i, total = subdiv->edges->total;   
          int elem_size = subdiv->edges->elem_size;   
   
    
          cvStartReadSeq( (CvSeq*)(subdiv->edges), &reader, 0 );   
          int num = 0;   
          srand( (unsigned)time( NULL ) );   
          for( i = 0; i < total; i++ )   
          {   
               CvQuadEdge2D* edge = (CvQuadEdge2D*)(reader.ptr);   
   
               if( CV_IS_SET_ELEM( edge ))   
               {   
                    CvSubdiv2DEdge e = (CvSubdiv2DEdge)edge;   
                    CvSubdiv2DEdge t = e;   
                    CvPoint buf[3];   
                    CvPoint *pBuf = buf;   
                    int iPointNum = 3;   
               
                    int j;
                    for(j = 0; j < iPointNum; j++ ){   
                         CvSubdiv2DPoint* pt = cvSubdiv2DEdgeOrg( t );   
                         if( !pt ) break;   
                         buf[j] = cvPoint( cvRound(pt->pt.x), cvRound(pt->pt.y));   
                         t = cvSubdiv2DGetEdge( t, CV_NEXT_AROUND_LEFT );   
                    }   
                    if (abs(buf[0].x) != 1800 && abs(buf[0].y) != 1800 &&   
                        abs(buf[1].x) != 1800 && abs(buf[1].y) != 1800 &&   
                        abs(buf[2].x) != 1800 && abs(buf[2].y) != 1800 && j == 3) {   
                         cvFillConvexPoly( img, buf, iPointNum, CV_RGB(0,250,0), CV_AA, 0 );   
                         cvPolyLine( img, &pBuf, &iPointNum, 1, 1, CV_RGB(0,0,0), 1, CV_AA, 0);   
                         printf("%d: (%d, %d)-(%d, %d)-(%d, %d)\n", num++, buf[0].x, buf[0].y, buf[1].x, buf[1].y, buf[2].x, buf[2].y);   
                    }   
   
//////////////////////////////////////////////////////////////////////////   
                    CvSubdiv2DEdge e1 = (CvSubdiv2DEdge)edge+2;   
//          CvSubdiv2DEdge e1 = cvSubdiv2DRotateEdge((CvSubdiv2DEdge)edge,2);   
                    t = e1;   
               
                    for(j = 0; j < iPointNum; j++ ){   
                         CvSubdiv2DPoint* pt = cvSubdiv2DEdgeOrg( t );   
                         if( !pt ) break;   
                         buf[j] = cvPoint( cvRound(pt->pt.x), cvRound(pt->pt.y));   
                         t = cvSubdiv2DGetEdge( t, CV_NEXT_AROUND_LEFT );   
                    }   
                    if (abs(buf[0].x) != 1800 && abs(buf[0].y) != 1800 &&   
                        abs(buf[1].x) != 1800 && abs(buf[1].y) != 1800 &&   
                        abs(buf[2].x) != 1800 && abs(buf[2].y) != 1800 && j == 3) {   
                         cvFillConvexPoly( img, buf, iPointNum, CV_RGB(rand()&255,rand()&255,rand()&255), CV_AA, 0 );   
                         cvPolyLine( img, &pBuf, &iPointNum, 1, 1, CV_RGB(0,0,0), 1, CV_AA, 0);   
                         printf("%d: (%d, %d)-(%d, %d)-(%d, %d)\n", num++, buf[0].x, buf[0].y, buf[1].x, buf[1].y, buf[2].x, buf[2].y);   
                    }   
   
               
               }   
           
               CV_NEXT_SEQ_ELEM( elem_size, reader );   
          }   
   
     }   

     void run(void)
     {
          char win[] = "source";
          int i;
          CvRect rect = { 0, 0, 600, 600 };
          CvMemStorage* storage;
          CvSubdiv2D* subdiv;
          IplImage* img;
          CvScalar active_facet_color, delaunay_color, voronoi_color, bkgnd_color;

          active_facet_color = CV_RGB( 255, 0, 0 );
          delaunay_color  = CV_RGB( 0,0,0);
          voronoi_color = CV_RGB(0, 180, 0);
          bkgnd_color = CV_RGB(255,255,255);

          img = cvCreateImage( cvSize(rect.width,rect.height), 8, 3 );
          cvSet( img, bkgnd_color, 0 );

          cvNamedWindow( win, 1 );

          storage = cvCreateMemStorage(0);
          subdiv = init_delaunay( storage, rect );

          printf("Delaunay triangulation will be build now interactively.\n"
                 "To stop the process, press any key\n\n");

          for( i = 0; i < 50; i++ )
          {
               CvPoint2D32f fp = cvPoint2D32f( (float)(rand()%(rect.width-10)+5),
                                               (float)(rand()%(rect.height-10)+5));

               locate_point( subdiv, fp, img, active_facet_color );
               cvShowImage( win, img );

               if( cvWaitKey( 100 ) >= 0 )
                    break;

               cvSubdivDelaunay2DInsert( subdiv, fp );
               cvCalcSubdivVoronoi2D( subdiv );
               cvSet( img, bkgnd_color, 0 );
               draw_subdiv( img, subdiv, delaunay_color, voronoi_color );
               cvShowImage( win, img );

               if( cvWaitKey( 100 ) >= 0 )
                    break;
               //    cvWaitKey(0);
          }

          cvSet( img, bkgnd_color, 0 );
          paint_voronoi( subdiv, img );
          cvShowImage( win, img );

          cvWaitKey(0);

          // tmp_triangles(subdiv, img);
          cvShowImage( win, img );
          triangulation(subdiv, img);
          cvWaitKey(0);

          cvReleaseMemStorage( &storage );
          cvReleaseImage(&img);
          cvDestroyWindow( win );
     }

     void delaunay( IplImage* edge, IplImage* img, IplImage* source = NULL )
     {
          if( edge == NULL )
          {
               cout << "edge == NULL @ delaunay" << endl;
               return;
          }
          else if( edge->nChannels != 1 )
          {
               cout << "nChannel != 1 @ delaunay" << endl;
          }

          cout <<  __FUNCTION__ << "\t" << "Delaunay start......" << endl;
          vector< CvPoint2D32f > points;
          for (int i = 0; i < edge->height; ++i)
          {
               for (int j = 0; j < edge->width; ++j)
               {
                    if( CV_IMAGE_ELEM( edge, uchar, i, j ) != 0 )
                         points.push_back( cvPoint2D32f( j, i ) );
               }

          }
          // 为了把边缘的部分也描述出来，决定手工地增加散点
          int h = points.size() / img->width; // 根据点在所有像素中的比例来计算边上的比例
          int w = points.size() / img->height;
          float h_num = 1.0 * img->height / h;
          float w_num = 1.0 * img->width / w;
          for(int i = 0; i < w; i++)
          {
               points.push_back( cvPoint2D32f(i*w_num, img->height-1));
               points.push_back( cvPoint2D32f(i*w_num, 0));
          }
          for(int i = 0; i < h; i++)
          {
               points.push_back( cvPoint2D32f(img->width-1, i*h_num) );
               points.push_back( cvPoint2D32f(0, i*h_num) );
          }
          // 为了避免点过少时右下角的点被剔除，就而外加一点
          points.push_back( cvPoint2D32f(0, 0) );
          points.push_back( cvPoint2D32f(img->width-1, img->height-1) );
          
          char win[] = "source";
          static CvMemStorage* storage = NULL;
          CvSubdiv2D* subdiv = NULL;
          CvScalar active_facet_color, delaunay_color, voronoi_color, bkgnd_color;
          
          active_facet_color = CV_RGB( 255, 0, 0 );
          delaunay_color  = CV_RGB( 0,0,0);
          voronoi_color = CV_RGB(0, 180, 0);
          bkgnd_color = CV_RGB(255,255,255);          

          storage = cvCreateMemStorage( 0 );
          CvRect rect = { 0, 0, img->width, img->height };
          
          subdiv = init_delaunay( storage, rect );
          for( int i = 0; i < points.size(); i++ )
          {
               CvPoint2D32f fp = points[ i ];
               locate_point( subdiv, fp, img, active_facet_color );
               cvShowImage( win, img );

               cvSubdivDelaunay2DInsert( subdiv, fp );
               cvCalcSubdivVoronoi2D( subdiv );
               cvSet( img, bkgnd_color, 0 );
               draw_subdiv( img, subdiv, delaunay_color, voronoi_color ); 
#if GO_DEBUG_DELAUNAY
               cvShowImage( win, img );
               if( cvWaitKey(10) >= 0 )
                    break;
#endif               
          }

          // cvSet( img, bkgnd_color, 0 );
          // paint_voronoi( subdiv, img );
          // cvShowImage( win, img );

          cvSaveImage("Delaunay.png", img);
          // cvWaitKey(0);
          cout <<  __FUNCTION__ << "\t" << "Delaunay finished......" << endl;
          if(source != NULL)
               triangulation(subdiv, source);
          else
               cout << "Source is NULL" << endl;
          
          
          cvReleaseMemStorage( &storage );
          cvDestroyWindow(win);
     }
     // 寻找三角形
     bool findTriangle(CvSubdiv2DEdge& e, list<VTriangle>& lt)
     {
          CvSubdiv2DEdge t = e;
          CvPoint2D32f buf[3];
          int iPointNum = 3;

          int j;
          for(j = 0; j < iPointNum; j++ ){
               CvSubdiv2DPoint* pt = cvSubdiv2DEdgeOrg( t );
               if( !pt ) break;
               buf[j] = cvPoint2D32f(pt->pt.x, pt->pt.y);
               t = cvSubdiv2DGetEdge( t, CV_NEXT_AROUND_LEFT );
          }
          if(fabs(buf[0].x) <= 1199 && fabs(buf[0].y) <= 1199 &&   
             fabs(buf[1].x) <= 1199 && fabs(buf[1].y) <= 1199 &&   
             fabs(buf[2].x) <= 1199 && fabs(buf[2].y) <= 1199 &&
             buf[0].x >= 0 && buf[0].y >= 0 && // 没有=0造成了边缘没点
             buf[1].x >= 0 && buf[1].y >= 0 &&
             buf[2].x >= 0 && buf[2].y >= 0 && j == 3)
          {
               lt.push_back(VTriangle(buf));        // 添加三角形
               // cout <<  __FUNCTION__ << "\t" << lt.size() << endl;
               return true;  
          }

          return false; 
     }
     // 剔除重复的三角形 使用list.unique是个不错的选择
     void pickTriangle(list<VTriangle>& lt)
     {
          if(lt.size() < 2)
          {
               cout <<  __FUNCTION__ << "\t" << "list size <= 1" << endl;
               return;
          }

          lt.sort();
          /* unique */
#if GO_DEBUG
          for(list<VTriangle>::iterator it=lt.begin(); it!=lt.end(); ++it)
               cout <<  __FUNCTION__ << "\t" << *it << endl;
#endif
          lt.unique();
#if GO_DEBUG
          for(list<VTriangle>::iterator it=lt.begin(); it!=lt.end(); ++it)
               cout <<  __FUNCTION__ << "\t" << *it << endl;
#endif
          /* my own function 这个效果其实还好 为了可读性*/
          // list<VTriangle>::iterator it = lt.begin();
          // while( it != lt.end() )
          // {
          //      list<VTriangle>::iterator tmp = it++;
          //      if(*it == *tmp)
          //      {
          //           lt.erase(it);
          //           it = tmp;
          //      }
          // }
     }
     // 保存成obj格式
     // std的unique find 以及 sort作孽， 考虑自己写个吧 fuc
     // 段错误
     void saveTriangle(list<VTriangle>& lt)
     {
          cout <<  __FUNCTION__ << "\t" << "Start Save OBJ......" << endl;
          char filename[] = "image.obj";
          ofstream outfile(filename);
          //
          if(!outfile)
          {
               cout <<  __FUNCTION__ << "\t" << "outFile Error" << endl;
               return;
          }

          // 内存中转化数据 vector为了能够直接拿到序号
          vector<CvPoint2D32f> points;
          // 插入点
          list<VTriangle>::iterator it;
          for (it = lt.begin(); it != lt.end(); ++it)
          {
               CvPoint2D32f* p = it->getPoint();
               points.push_back( p[0] );
               points.push_back( p[1] );
               points.push_back( p[2] );
          }
#if GO_DEBUG_SAVE
          cout <<  __FUNCTION__ << "\t" << "push points.../n start sort" << endl;
          printTriangle(lt);
          printPoint(points);
#endif
          sort(points.begin(), points.end(), pred_cmp);

#if GO_DEBUG_SAVE
          cout <<  __FUNCTION__ << "\t" << "sort and unique points...\n start write" << endl;
#endif
          vector<CvPoint2D32f>::iterator last = unique(points.begin(), points.end(),pred_equal);
          vector<CvPoint2D32f>::iterator ip;
          for(ip = points.begin(); ip != last; ++ip)
          {
               outfile << "v " << ip->x << "\t" << ip->y << "\t" << 0.0 << endl;
          }
          
          // 插入三角边          
          for(it = lt.begin(); it != lt.end(); ++it)
          {
               outfile << "f ";
               CvPoint2D32f* p = it->getPoint();
               for(int i=0; i<3; i++)
               {
                    // ip = find(points.begin(), last, p[i]);
                    for(ip=points.begin(); ip!=last; ip++)
                         if( pred_equal(*ip, p[i]) ) break;
                    if(ip == last)
                         cout <<  __FUNCTION__ << "\t" << "ERROR find" << endl;
                    int pos= ip - points.begin() + 1;
                    outfile << pos;
                    if(i!=2)
                         outfile << " ";
               }
               outfile << endl;
          }
          outfile.close();
          cout <<  __FUNCTION__ << "\t" << "Finish Save OBJ......" << endl;      
     }
     // 输出Triangle
     void printTriangle(list<VTriangle>& lt)
     {
          char filename[] = "Debug/Debug_Triangle.txt";
          ofstream outfile(filename);
          //
          if(!outfile)
          {
               cout <<  __FUNCTION__ << "\t" << "outFile Error" << endl;
               return;
          }

          list<VTriangle>::iterator it;
          for(it=lt.begin(); it!=lt.end(); it++)
          {
               outfile <<  __FUNCTION__ << "\t" << *it << endl;
          }
     }
     // 输出points
     void printPoint(list<CvPoint2D32f>& lp)
     {
          char filename[] = "Debug/Debug_Points.txt";
          ofstream outfile(filename);
          //
          if(!outfile)
          {
               cout <<  __FUNCTION__ << "\t" << "outFile Error" << endl;
               return;
          }

          list<CvPoint2D32f>::iterator it;
          for(it=lp.begin(); it!=lp.end(); it++)
          {
               outfile <<  __FUNCTION__ << "\t" << it->x << ", " << it->y << endl;
          }
     }
     void printPoint(vector<CvPoint2D32f>& lp)
     {
          char filename[] = "Debug/Debug_Points.txt";
          ofstream outfile(filename);
          //
          if(!outfile)
          {
               cout <<  __FUNCTION__ << "\t" << "outFile Error" << endl;
               return;
          }

          vector<CvPoint2D32f>::iterator it;
          for(it=lp.begin(); it!=lp.end(); it++)
          {
               outfile <<  __FUNCTION__ << "\t" <<  it->x << ", " << it->y  << endl;
          }
     }

     // Merge
     // 还没想好
     void merge_compress(list<VTriangle>& lv)
     {
          cout <<  __FUNCTION__ << "\t" << "Merge Triangles start..." << endl;
          // 转换成vector，为了能随机访问
          vector< list<VTriangle>::iterator > vector_iterator;
          for( list<VTriangle>::iterator it = lv.begin(); it != lv.end(); it++ )
          {
               vector_iterator.push_back( it );
          }

          // 初始化父节点
          int* father = new int[ vector_iterator.size() ];
          for(int i=0; i<vector_iterator.size(); i++)
          {
               father[i] = i;
          }

          // 开始合并 union
          vector< list<VTriangle>::iterator >::iterator it_it;
          vector< list<VTriangle>::iterator >::iterator it_jt;
          int merge_num = 0;
          for( int i=0; i<vector_iterator.size(); i++ )
          {
               for( int j=0; j<i; j++ )
               {
                    if( is_similar(*vector_iterator[i], *vector_iterator[j]) &&
                        is_neighbour(*vector_iterator[i], *vector_iterator[j]) )
                    {
                         father[j] = father[i];
                         merge_num++;
                         break;
                    }
               }
          }
          // 合并完毕 开始赋予颜色
          for( int i=0; i<vector_iterator.size(); i++ )
          {
               (*vector_iterator[i]).setColor( *vector_iterator[ father[i] ] );
          }

          cout <<  __FUNCTION__ << "\t" << "Total Num: " << vector_iterator.size()-merge_num << endl;
          delete[] father;
     }
     // groupon
     // group 要先分好空间 group.size() == lv.size()
     // VTriangle 要提供is_grouped() set_group() 以及 groupon
     void mergeTriangle(list<VTriangle>& lv, vector< vector<VTriangle> >& group)
     {
          cout <<  __FUNCTION__ << "\t" << "Merge Triangles start..." << lv.size() << endl;
          // 转换成vector，为了能随机访问
          vector<VTriangle> vt;
          for( list<VTriangle>::iterator it = lv.begin(); it != lv.end(); it++ )
          {
               vt.push_back( *it );
          }
          
          // 准循环开始
          int last_index = -1;
          for(int i=0; i!=vt.size(); i++)
          {
               // cout << i << endl;
               int current_index;
               if( !vt[i].is_grouped() )
               {
                    current_index = ++last_index;
                    // cout << current_index << endl;
                    group[current_index].push_back(vt[i]);
                    vt[i].set_group(current_index);
               }
               else
               {
                    current_index = vt[i].get_group();
               }
               // 逐一比较
               for(int j=i+1; j!=vt.size(); j++)
               {
#if GO_DEBUG_MERGE
                    cout <<  __FUNCTION__ << "\t" << i << " vs " << j << endl;
#endif 
                    if( groupon( vt[i], vt[j] ) )
                    {
                         group[current_index].push_back(vt[j]);
#if GO_DEBUG_MERGE
                         cout <<  __FUNCTION__ << "\t" << "Groupon~~" << endl;
#endif
                    }
               }
               
          }
          cout <<  __FUNCTION__ << "\t" << "Merge Triangles end..." << group.size() << endl;
     }
     void drawGroup(vector< vector<VTriangle> >& group, IplImage* img)
     {
          for (int i = 0; i < group.size(); ++i)
          {
               for (int j = 0; j < group.size(); ++j)
               {
                    group[i][j].draw(img);
               }
          }
     }
     void printGroup(vector< vector<VTriangle> >& group, ostream& out)
     {
          for (int i = 0; i < group.size(); ++i)
          {
               out << "Group: " << i << endl;
               for (int j = 0; j < group.size(); ++j)
               {
                    group[i][j].print(out);
                    out << endl;
               }
          }
     }
     // Blur
     // 根据三角形边缘进行blur，blur的masK根据原图得到
     // version 0.1
     void blur(IplImage* scr, IplImage* dst)
     {
          cout <<  __FUNCTION__ << "\t" << "Blur Triangles start" << endl;
          // cvSmooth(scr, dst, CV_BLUR, 3, 3, 0.5, 0.5);
     }
     // 三角化 剖分完后的主程
     void triangulation(CvSubdiv2D* subdiv, IplImage* source)
     {
          CvSeqReader  reader;
          list<VTriangle> triangles;
          list<VTriangle>::iterator it;
          int i, total = subdiv->edges->total;
          int elem_size = subdiv->edges->elem_size;

          cvStartReadSeq( (CvSeq*)(subdiv->edges), &reader, 0 );

          for( i = 0; i < total; i++ )
          {
               CvQuadEdge2D* edge = (CvQuadEdge2D*)(reader.ptr);

               if( CV_IS_SET_ELEM( edge ))
               {                                            
                    CvSubdiv2DEdge e = (CvSubdiv2DEdge)edge;
                    findTriangle(e,triangles);
                    // CvSubdiv2DEdge e1 = (CvSubdiv2DEdge)edge+2; //即next[2]
                    CvSubdiv2DEdge e1 = cvSubdiv2DRotateEdge((CvSubdiv2DEdge)edge,2);
                    findTriangle(e1,triangles);
               }

               CV_NEXT_SEQ_ELEM( elem_size, reader );
          }
          
          // 剔除重复三角形
          cout << endl << "befor unique : " << triangles.size() << endl;
          pickTriangle(triangles);
          cout << "after unique : " << triangles.size() << endl;

// 合并、优化三角形
          vector< vector<VTriangle> > group( triangles.size() );
          mergeTriangle(triangles, group);
          
#if GO_DEBUG_SAVE
          cout <<  __FUNCTION__ << "\t" << "start save obj" << endl;
#endif
          // 保存数据
          saveTriangle(triangles);
          // 绘制所有三角形
          cvNamedWindow("draw", 1);
          IplImage* canvas = cvCreateImage(cvGetSize(source), 8, 3);
          cvZero(canvas);
          for(it = triangles.begin(); it != triangles.end(); it++)
          {
               it->traversal(source);
               it->draw(canvas);
#if GO_DEBUG
               cvWaitKey(10);
               cvShowImage("draw",canvas);
                cout <<  __FUNCTION__ << "\t" << "draw triangles......" << endl;
#endif
          }
#if GO_DEBUG
          cout <<  __FUNCTION__ << "\t" << "Finish draw triangles......" << endl;
#endif          
          // blur(canvas, canvas);
          cvShowImage("draw",canvas);
          cvWaitKey();
          cvSaveImage("result.jpg", canvas);
          cvReleaseImage(&canvas);
          cvDestroyWindow("draw");
#if GO_DEBUG
          cout <<  __FUNCTION__ << "\t" << "exit......" << endl;
#endif
     }

     // 将图像导出成obj
     void export_to_obj(IplImage* img)
          {
               if(!img){
                    cout <<  __FUNCTION__ << "\t" << "img == NULL" << endl;
                    return;
               }
               // gray image for test
               if(img->nChannels != 1){
                    cout <<  __FUNCTION__ << "\t" << "nChannel != 1" << endl;
                    return ;
               }
               
               // export start
               int step = 5;    // step 让数据小一点
               double beta = 100.0f; // 让rgb值和xy值不要差太大
               ofstream fileout("out.obj");
               for (int i = 0; i < img->height; i+=step)
               {
                    for (int j = 0; j < img->width; j+=step)
                    {
                         fileout << "v "<< j << "\t" << i << "\t"
                                 << (double)CV_IMAGE_ELEM(img, uchar, i, j)/beta << endl;
                    }
               }
               for (int i = 0; i < img->height-1; i+=step)
               {
                    for (int j = 0; j < img->width-1; j+=step)
                    {
                         int id = i*img->width + j;
                         fileout << "f " << id << "\t" << id + img->width
                                 << "\t" << id + img->width + 1 << endl;
                         fileout << "f " << id << "\t" << id + img->width + 1
                                 << "\t" << id + 1 << endl;                         
                    }
               }

               fileout.close();
          }

     // 构建BeizerPatch
     // 映射到uv空间
     vector< VPoint > get_points(IplImage* img);
     void bezierpatch(IplImage* img)
          {
               vector< VPoint > points = get_points(img);
               if(points.size() <= 0)
               {
                    cout <<  __FUNCTION__ << "\t" << "points.size() <= 0" << endl;
                    return;
               }
              
               // point 到 inner_point
               int* hash_table = new int[points.size()];
               float* flag = new float[points.size()];
               float lamda = 1.0 / 8.0;
               // 初始化矩阵
               vector< VPoint > inner_points;
               for(int i=0; i<points.size(); ++i)
               {
                    // 边界求值 内部点为负
                    if(i<img->width || i+img->width >= points.size() ||
                       i%img->width == 0 || (i+1)%img->width == 0)
                    {
                         flag[i] = true; // true变成累加旋长
                    }
                    else
                    {
                         flag[i] = -1.0f;
                         inner_points.push_back( points[i] );
                         hash_table[i] = inner_points.size();
                    }
               }

               int inner_size = inner_points.size();
               // 初始化矩阵为0
               CvMat* A = cvCreateMat(inner_size, inner_size, CV_32FC1);
               CvMat* BU = cvCreateMat(inner_size, 1, CV_32FC1);
               CvMat* BV = cvCreateMat(inner_size, 1, CV_32FC1);
               CvMat* U = cvCreateMat(inner_size, 1, CV_32FC1);
               CvMat* V = cvCreateMat(inner_size, 1, CV_32FC1);
               cvSetZero(A);
               cvSetZero(BU);
               cvSetZero(BV);
               // 设置矩阵
               for(int i=0; i<inner_points.size(); ++i)
               {
                    // 边界则B行列式里增加，若非边界则A矩阵参数变化
                    if(flag[ inner_points[i].id ] < 0)
                         continue;
                    
                    CV_MAT_ELEM(*A, float, i, i) += 1;
                    for(vector<int>::iterator c = inner_points[i].connect.begin();
                        c != inner_points[i].connect.end(); ++c)
                    {
                         if(flag[*c] < 0) // 非边界
                         {
                              CV_MAT_ELEM(*A, float, i, 0) += lamda * flag[*c];
                         }
                         else
                         {
                              CV_MAT_ELEM(*BU, float, i, hash_table[*c]) += lamda * flag[*c];
                              CV_MAT_ELEM(*BV, float, i, hash_table[*c]) += lamda * flag[*c];
                         }
                    }
               }
               // slove 求解得到UV空间的点
               cvSolve(A, BU, U, CV_SVD);
               cvSolve(A, BV, V, CV_SVD);

               
               // release
               cvReleaseMat(&A);
               cvReleaseMat(&BU);
               cvReleaseMat(&BV);
               delete[] flag;
               delete[] hash_table;
          }
     //得到初始点
     vector< VPoint > get_points(IplImage* img)
     {
               vector< VPoint > points;
               
               if(!img){
                    cout <<  __FUNCTION__ << "\t" << "img == NULL" << endl;
                    return points;
               }
               // gray image for test
               if(img->nChannels != 1){
                    cout <<  __FUNCTION__ << "\t" << "nChannel != 1" << endl;
                    return points;
               }

               // export start
               int step = 5;    // step 让数据小一点
               double beta = 100.0f; // 让rgb值和xy值不要差太大
               ofstream fileout("out.obj");
               for (int i = 0; i < img->height; i+=step)
               {
                    for (int j = 0; j < img->width; j+=step)
                    {
                         points.push_back( VPoint(j, i, (double)CV_IMAGE_ELEM(img, uchar, i, j)/beta,i*img->width+j) );
                    }
               }
               // 建立连接关系
               for(int i=0; i<points.size(); ++i)
               {
                         // 左右
                         if(i-1 >= 0 && i-1 <points.size())
                              points[i].connect.push_back(i-1);
                         if(i+1 >= 0 && i+1 <points.size())
                              points[i].connect.push_back(i+1);
                         // 上下
                         if(i-img->width >= 0 && i-img->width <points.size())
                              points[i].connect.push_back(i-img->width);
                         if(i+img->width >= 0 && i+img->width <points.size())
                              points[i].connect.push_back(i+img->width);
                         // 上 左右
                         if(i-img->width+1 >= 0 && i-img->width+1 <points.size())
                              points[i].connect.push_back(i-img->width+1);
                         if(i-img->width-1 >= 0 && i-img->width-1 <points.size())
                              points[i].connect.push_back(i-img->width-1);
                         // 下 左右
                         if(i+img->width+1 >= 0 && i+img->width+1 <points.size())
                              points[i].connect.push_back(i+img->width+1);
                         if(i+img->width-1 >= 0 && i+img->width-1 <points.size())
                              points[i].connect.push_back(i+img->width-1);
               }
               return points;
     }
     // 初始化矩阵系数
     void initial_matrix(CvMat* A, CvMat* B)
     {
               
     }

     // 解线性方程
     // 矩阵A:m*n 下标从1开始 D:n未知数个数 P:m方程条数
     // A*D=P
     // 先用opencv的解法
     void solveLinearSystem(int n,int m, float** A, float* P, float* D)
     {
           CvMat* MA = cvCreateMat(m, n, CV_32FC1);
           CvMat* MP = cvCreateMat(m, 1, CV_32FC1);
           CvMat* MD = cvCreateMat(n, 1, CV_32FC1);
           // A to MA
           for(int i=0; i<m; ++i)
                for(int j=0; j<n; ++j)
                {
                     CV_MAT_ELEM(*MA, float, i, j) = A[i+1][j+1];
                }
           // P to MP
           for(int i=0; i<m; ++i)
                CV_MAT_ELEM(*MP, float, i, 0) = P[i+1];
           // D to MD : not necessary
           for(int i=0; i<n;++i)
                CV_MAT_ELEM(*MD, float, i, 0) = D[i+1];

           // solve
           cvSolve(MA, MP, MD, CV_SVD);

           // result: D
           for(int i=0; i<n; ++i)
           {
                D[i+1] = CV_MAT_ELEM(*MD, float, i, 0);
           }

           // release Mat
           cvReleaseMat(&MA);
           cvReleaseMat(&MP);
           cvReleaseMat(&MD);
     }

// 测试，增加些测试的东西
     void test()
     {
          CvPoint p[3];
          p[0] = cvPoint(50,85);
          p[1] = cvPoint(200,80);
          p[2] = cvPoint(100, 80);
          VTriangle a1(p);
          a1.setColor(cvScalar(60, 157, 199));
          
          // p2
          CvPoint p2[3];
          p2[0] = cvPoint(50,183);
          p2[1] = cvPoint(200,180);
          p2[2] = cvPoint(100, 400);
          VTriangle a2(p2);
          a2.setColor(cvScalar(160, 57, 199));
          
          IplImage* s = cvCreateImage(cvSize(400,400), 8, 3);
          IplImage* d = cvCreateImage(cvSize(400,400), 8, 3);
          a1.draw(s);
          //a1.superTraversal(s);
          a1.traversal(s);
          a1.draw(d);
          // a2.superTraversal(s);
          a2.draw(s);
          a2.traversal(s);
          a2.draw(d);
          cvShowImage("hello s", s);
          cvShowImage("hello d", d);
          cvWaitKey(0);
          cvReleaseImage(&s);
          cvReleaseImage(&d);

          // run
          run();
 
     }
     
}
