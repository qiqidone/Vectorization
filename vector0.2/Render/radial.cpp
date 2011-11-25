#include "radial.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <cstring>
#include <cassert>

using namespace Radial;
using namespace std;

enum RenderTypes
{
     CPU,
     GPU,
};

extern "C" void gpuDiffuseRender(int **image);
extern "C" void gpuFieldRender(float **ps, float **pe, float **pv, int **lc, int **rc, int **iimage, int size);
// Point
RadialPoint::RadialPoint(double _x, double _y)
{
     x = _x;
     y = _y;
}

void RadialPoint::getline(const int line)
{
     // vector 从0开始 所以指标-1
     radial_line.push_back(line-1);
}
void RadialPoint::print()
{
     printf("v %.2f %.2f\n", x, y);
}

// Color
void RadialColor::setColor(int* color)
{
     val[0] = color[0];
     val[1] = color[1];
     val[2] = color[2];
     val[3] = color[3]; 
}

void RadialColor::getColor(int* color)
{
     for(int i = 0; i < 4; ++i)
     {
          color[i] = val[i];
     }
}

RadialColor::RadialColor(int* color)
{
     val[0] = color[0];
     val[1] = color[1];
     val[2] = color[2];
     val[3] = color[3];

}
void RadialColor::print()
{
     printf("c %d %d %d %d\n",
            val[0],val[1],val[2],val[3]);
}


// Line

RadialLine::RadialLine(int ids, int ide, int* colorl, int* colorr)
     :start(ids),end(ide),length(0.0),sigma(1.0)
{
     // start = ids;
     // end = ide;
     // length = 0.;
     // sigma = 1.;
     is_finish_initialed = false;
     Cl.setColor(colorl);
     Cr.setColor(colorr);
}

double RadialLine::calLength()
{
     double x = p_end->x - p_start->x;
     double y = p_end->y - p_start->y;
     length = sqrt(x*x + y*y);
     Vec2d PQ = v_end - v_start;
     
     return length;
}

double RadialLine::dist(double tx, double ty)
{
     double x1 = p_end->x - p_start->x;
     double y1 = p_end->y - p_start->y;

     double x2 = tx - p_start->x;
     double y2 = ty - p_start->y;

     return (x1*x2 + y1*y2) / (length*length);

}

double RadialLine::distance_p2l(double tx, double ty)
{
     double x1 = p_end->x - p_start->x;
     double y1 = p_end->y - p_start->y;

     double x2 = tx - p_start->x;
     double y2 = ty - p_start->y;

     double x3 = tx - p_end->x;
     double y3 = ty - p_end->y;
     
     Vec2d ol(x1, y1);
     Vec2d op(x2, y2);
     Vec2d lp(x3, y3);
     
     double f = ol.dot(op) / ol.length_square();
     if(f < 0.) {
          return op.length();
     }
     if(f > 1.) {
          return lp.length();
     }
     return ol.perpendicular().dot(op) / ol.length();
}
double RadialLine::weight(double tx, double ty)
{
     Vec2d uv = coordinate(tx, ty);
     // 要保证weight<1
     uint c = 3;
     switch(c)
     {
     case 1:
          // 方案1：uv绝对值成反比
          return length / (sigma +abs(uv.x));
     case 2:
          // 方案2：到线段距离长度成反比 guass
          //if(abs( distance_p2l(tx, ty) ) < ZERO) return 0;
          return gauss( distance_p2l(tx, ty) , sigma);
     case 3:
          // 方案3：距离平方成反比
          return gauss( uv.length(), sigma );
     case 4:
          // 方案4
          return 0;
     default:
          // 默认
          return 1. / (sigma +abs(uv.x));
     }
}

void RadialLine::getColor(double tx, double ty, int* color)
{
     // 判断左右
     Vec2d uv = coordinate(tx, ty);
     // // >1 or <-1
     // if( uv.y < 0 )
     // {
     //      //return;
     // } 
     
     
     int source[4] ={0};
     // 左右
     if(uv.x > ZERO)
     {
          Cr.getColor(source);
     }
     else if(uv.x < -ZERO)
     {
          Cl.getColor(source);
          //memset(source, 0, 4);
     }
     else
     {
          for(int i = 0; i < 4; ++i)
          {
               source[i] = (Cr.val[i] + Cl.val[i]) / 2;
          }
     }
     // 赋值
     double w = weight(tx, ty);
     if(w > 1)
     {
          printf("%.1f, %.1f w = %.2f\n",tx, ty, w);
          w = 1.0;
     }
     for(int i = 0; i < 4; ++i)
     {
          color[i] += w * source[i];
     }
}

Vec2d RadialLine::coordinate(double tx, double ty)
{
     // vector PQ  and point X
     Vec2d P(p_start->x, p_start->y);
     Vec2d Q(p_end->x, p_end->y);
     Vec2d X(tx, ty);
     Vec2d PQ = Q-P;
     Vec2d PX = X-P;
     // result
     double u,v;                // u为y v为x
     u = PX.dot(PQ) / PQ.length_square();
     v = PX.dot( PQ.perpendicular() ) / PQ.length_square();
     return Vec2d(v, u);
}

bool RadialLine::finish_initial()
{
     if(is_finish_initialed == true)
          return true;

     // set length
     calLength();
     if(length < ZERO)
     {
          cout <<  __FUNCTION__ << "\t" << "warning... ";
          printf("Line:\n\t");
          print();
          length = 0.0;
     }    

     // sigma 为正
     assert(sigma > ZERO);
     
     is_finish_initialed = true;

     return is_finish_initialed;
}

void RadialLine::print()
{
     printf("l %d %d length = %.2f\n",start, end, length);
     printf("\t");
     Cl.print();
     printf("\t");
     Cr.print();
     printf("\tGauss Sigma = %.2f\n", sigma);
}
// Image

RadialImage::RadialImage(int _width, int _height):
     width(_width),height(_height)
{
     image = new int*[height*width];
     for(int i=0; i<width*height; ++i){
          image[i] = new int[4];
          memset(image[i], 0, sizeof(int)*4);
     }
}
RadialImage::~RadialImage()
{
     for(int i = 0; i < height*width; ++i)
     {
          delete[] image[i];
     }
     delete[] image;
}

void RadialImage::setPixel(int h, int w, int* color)
{
     for(int i = 0; i < 4; ++i)
     {
          image[h*width+w][i] = color[i];
     }
}
void RadialImage::read(char* filename)
{
     // 第一步 读取点
     ifstream file;
     file.open(filename,ios::in);
     if(!file)
     {
          cout <<  __FUNCTION__ << "\t" << "file error" << endl;
          return ;
     }
     // 清空数据
     radial_point.clear();
     radial_line.clear();
     // read 
     string line;     
     while(getline(file, line))
     {
          istringstream iss(line);
          string primitive;
          iss >> primitive;

          int id,ids,ide,colorl[4],colorr[4];
          double gs;
          double x,y;
          switch(primitive[0])
          {
          case 'v':
               iss >> id >> x >> y; 
               radial_point.push_back(RadialPoint(x,y));
               break;
          case 'l':
               // 读入start end 的id
               iss >> id >> ids >> ide;
               // 读入Cl的rgbt
               iss >> colorl[0] >> colorl[1] >> colorl[2] >> colorl[3];
               // 读入Cr的rgbt
               iss >> colorr[0] >> colorr[1] >> colorr[2] >> colorr[3];
               // gauss size
               iss >> gs;
               radial_line.push_back(RadialLine(ids, ide, colorl, colorr));
               radial_line.back().setGauss(gs);
               printf("Read in Sigma %.2f\n", radial_line.back().sigma);
               radial_point[id].getline(id);
               break;
          case '#':
               break;
          default:
               cout <<  __FUNCTION__ << "\t" << "?" << endl;
               break;
          }
     }

     // 为point 设置指针
     for(unsigned i = 0; i < radial_line.size(); ++i)
     {
          radial_line[i].p_start = radial_point.begin() + radial_line[i].start-1;
          radial_line[i].p_end = radial_point.begin() + radial_line[i].end-1;

          radial_line[i].v_start = Vec2d(radial_line[i].p_start->x, radial_line[i].p_start->y);
          radial_line[i].v_end = Vec2d(radial_line[i].p_end->x, radial_line[i].p_end->y);
          // finish initial
          radial_line[i].finish_initial();
     }

     file.close();
     file.clear();
     // 第二步 建立拓扑
     
     // 第三步 生成数据集
}
void RadialImage::exportImage()
{
     fprintf(stderr,"\nexport Image: ...");

     FILE* f = fopen("image.ppm", "w");
     fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
     for(int i = 0; i < width*height; ++i)
     {
           if(image[i][0] > 255 || image[i][1] > 255 || image[i][2] > 255)
           {
                fprintf(stderr, "warning...%d %d \n",i%width, i/width);
                image[i][0] = image[i][1] = image[i][2] = 255;
           }

          fprintf(f, "%d %d %d ", image[i][0], image[i][1],image[i][2]);
     }

     fprintf(stderr,"\rSuccessed export image.\n");
     //system("display image.ppm");
}
void RadialImage::render()
{

     // 第四步 渲染
     for(int i = 0; i < height; ++i)
     {
          for(int j = 0; j < width; ++j) // 坐标系转换成 (j, height-1-i)
          {
               fprintf(stderr, "\rRendering (%d * %d) %5.2f%%",
                       height, width, 100.*i/(height-1));

               int color[4] = {0};
               double weightsum = 0;
#if DEBUG_RENDER
               // fprintf
               fprintf(stderr,"%d %d : %d %d %d\n",i,j,color[0],color[1],color[2]);
#endif
               // 先求weightsum 再计算
               for(unsigned l=0; l<radial_line.size(); ++l)
               {
                    double w = radial_line[l].weight(j, height-1-i);
                    //fprintf(stderr, "%f\n", w);
                    weightsum += w;
                    radial_line[l].getColor(j, height-1-i, color);  // x y -> j i
               }
               
#if DEBUG_RENDER
               fprintf(stderr,"%d %d : %d %d %d\n",i,j,color[0],color[1],color[2]);
#endif
               for(int k = 0; k < 4; ++k)
               {
                    color[k] /= radial_line.size();
               }
#if DEBUG_RENDER
               // printf
               fprintf(stderr,"%d %d : %d %d %d\n",i,j,color[0],color[1],color[2]);
#endif
               setPixel(i, j, color);
          }
     }
     // 画 点和线
     // draw point 注意坐标转换
     for(uint i=0; i<radial_point.size(); ++i)
     {
          int point_color[4]={0,255,0,0};
          for(uint j=0; j<4; ++j){
               int x = (int)radial_point[i].x;
               int y = (int)radial_point[i].y;
               setPixel(height-1-y, x, point_color);
          }
     }
}     

void RadialImage::gpuRender()
{
     // GPU
     int **tmp_image;
     tmp_image = new int*[512*512];
     for(int i = 0; i < 512*512; ++i)
     {
          tmp_image[i] = new int[4];
          memset(tmp_image[i], 0, sizeof(int)*4);
     }
     
     for(int i=0; i<512*512; ++i){
          if(i % 512 == 256)
          {
               tmp_image[i][0] = 252;
               tmp_image[i][1] = 252;
               tmp_image[i][2] = 252;
          }
     }

     printf("\nGPU Diffuse Render\n");
     //gpuDiffuseRender(image);
     gpuDiffuseRender(tmp_image);

     for(int i = 0; i < 512*512; ++i)
     {
          delete[] tmp_image[i];
     }
     delete[] tmp_image;

}

void RadialImage::gpuField()
{
     printf("gpuFieldRender Start...\n");
     int lineSize = radial_line.size();

     // new
     float **pointStart = new float*[lineSize];
     float **pointEnd = new float*[lineSize];
     float **pointVector = new float*[lineSize];
     int **leftColor = new int*[lineSize];
     int **rightColor = new int*[lineSize];
     for(int i = 0; i < lineSize; ++i)
     {
          pointStart[i] = new float[3];
          pointEnd[i] = new float[3];
          pointVector[i] = new float[3];
          leftColor[i] = new int[3];
          rightColor[i] = new int[3];
     }

     // initial
     printf("initial data...\n");
     getLineStart(pointStart);
     getLineEnd(pointEnd);
     getLineVector(pointVector);
     getLeftColor(leftColor);
     getRightColor(rightColor);

     printf("gpu...running...\n");
     gpuFieldRender(pointStart, pointEnd, pointVector, leftColor, rightColor,image, lineSize);
     

     // delete
     for(int i = 0; i < lineSize; ++i)
     {
          delete[] pointStart[i];
          delete[] pointEnd[i];
          delete[] pointVector[i];
          delete[] leftColor[i];
          delete[] rightColor[i];
     }
     delete[] pointStart;
     delete[] pointEnd;
     delete[] pointVector;
     delete[] leftColor;
     delete[] rightColor;

     printf("Field Render Succeed!\n");
}     
void RadialImage::run()
{
     char filename[] = "data.txt";
     read(filename);
     // print
     print();
     //render();
     if(renderType == CPU)        // CPU = 0
          printf("CPU Mode:\n"),render();
     else if(renderType == GPU)   // GPU = 1
          printf("GPU Mode:\n"),gpuField();


     exportImage();

}

void RadialImage::print()
{
     // points
     fprintf(stderr,"Point : %d\n",radial_point.size());
     for (unsigned int i = 0; i < radial_point.size(); ++i)
     {
          radial_point[i].print();
     }

     // lines
     fprintf(stderr,"Line ： %d\n", radial_line.size());
     for (unsigned int i = 0; i < radial_line.size(); ++i)
     {
          radial_line[i].print();
     }
}

void RadialImage::getLineStart(float** lineStart)
{
     for(unsigned int i = 0; i < radial_line.size(); ++i)
     {
          lineStart[i][0] = radial_line[i].p_start->x;
          lineStart[i][1] = radial_line[i].p_start->y;
     }
}

void RadialImage::getLineEnd(float** lineEnd)
{
     for(unsigned int i = 0; i < radial_line.size(); ++i)
     {
          lineEnd[i][0] = radial_line[i].p_end->x;
          lineEnd[i][1] = radial_line[i].p_end->y;
     }

}     
void RadialImage::getLineVector(float** lineVector)
{
     for(unsigned int i = 0; i < radial_line.size(); ++i)
     {
          lineVector[i][0] = radial_line[i].p_start->x - radial_line[i].p_end->x;
          lineVector[i][1] = radial_line[i].p_start->y - radial_line[i].p_end->y;
     }

}     
void RadialImage::getLeftColor(int** leftColor)
{
     for(unsigned int i = 0; i < radial_line.size(); ++i)
     {
          leftColor[i][0] = radial_line[i].Cl.val[0];
          leftColor[i][1] = radial_line[i].Cl.val[1];
          leftColor[i][2] = radial_line[i].Cl.val[2];
     }
}     
void RadialImage::getRightColor(int** rightColor)
{
     for(unsigned int i = 0; i < radial_line.size(); ++i)
     {
          rightColor[i][0] = radial_line[i].Cr.val[0];
          rightColor[i][1] = radial_line[i].Cr.val[1];
          rightColor[i][2] = radial_line[i].Cr.val[2];
     }
}     
void RadialImage::getPoint(float** point)
{
     point = NULL;
}     
void RadialImage::getImage(int** im)
{
     im = image;
}     
// namespace
