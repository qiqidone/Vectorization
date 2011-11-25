

#ifndef _RENDER_H_
#define _RENDER_H_

#include <stdio.h>
#include <stdlib.h>
#include "radial.h"

using Radial::Vec2d;

#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 512
#define PI 3.1416f
#define INF 9999

/* #ifndef UNIT */
/* #define UNIT */
typedef unsigned int unit;
/* #endif */

/* Position enum */
enum Position {LEFT, RIGHT, MIDDLE};

__device__ float2 operator+(float2 l, float2 r){
     float2 res;
     res.x = l.x+r.x;
     res.y = l.y + r.y;
     return res;
}

__device__ float2 operator-(float2 l, float2 r){
     float2 res;
     res.x = l.x-r.x;
     res.y = l.y - r.y;
     return res;
}

__device__ float operator*(float2 l, float2 r){
     return l.x*r.x + l.y*r.y;
}

__device__ float length(float2 l){
     return sqrt(l.x*l.x + l.y*l.y);
}

__device__ bool operator<(float3 &res, float r){
     return res.x < r && res.y < r && res.z < r;
}

__device__ bool operator>(float3 &res, float r){
     return res.x > r && res.y > r && res.z > r;
}

/* "operator=" must be a member function */

/* __device__ float3 operator=(float3 &res, float r){ */
/*      res.x = r; */
/*      res.y = r; */
/*      res.z = r; */
/*      return res; */
/* } */

__device__ float3 setValue(float3 &res, float r){
     res.x = r;
     res.y = r;
     res.z = r;
     return res;
}

__device__ float3 operator+=(float3& res, float3 r){
     res.x += r.x;
     res.y += r.y;
     res.z += r.z;
     return res;
}

__device__ float3 operator/=(float3& res, float r){
     res.x /= r;
     res.y /= r;
     res.z /= r;
     return res;
}
__device__ float3 operator-(float3& p1, float3& p2){
     float3 res;
     res.x = p1.x - p2.x;
     res.y = p1.y - p2.y;
     res.z = p1.z - p2.z;
     return res;
}
__device__ float3 operator+(float3 p1, float3 p2){
     float3 res;
     res.x = p1.x+p2.x;
     res.y = p1.y+p2.y;
     res.z = p1.z+p2.z;
     return res;
}
__device__ float operator*(float3 p1, float3 p2){
     return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;
}

__device__ float3 operator*(float3 p1, float k){
     float3 res = p1;
     res.x *= k;
     res.y *= k;
     res.z *= k;
     return res;
}
__device__ float dot(float3 p1, float3 p2){
     return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;
}


__device__ float length(float3 p){
     return sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
}

__device__ float3 norm(float3& p){
     float l = length(p);
     p.x /= l;
     p.y /= l;
     p.z /= l;
     return p;
}


__device__ float3 perpendicular(float3 p){
     float3 res;
     res.x = p.y;
     res.y = -p.x;
     res.z = p.z;
     return res;
}

__device__ float length_square(float3 p){
     return p.x*p.x + p.y*p.y + p.z*p.z;
}

__device__ float distance_p2p(float3 p1, float3 p2){
     float3 p = p1-p2;
     return length(p);
}

__device__ float distance_p2l(float3 p, float3 lstart, float3 lend){
     float3 op = p - lstart;
     float3 ol = lend - lstart;
     float f = op * ol / length_square(ol);
     if(f < 0) return distance_p2p(p, lstart);
     if(f > 1) return distance_p2p(p, lend);
     return perpendicular(ol) * op / length(ol);
}

__device__ Position position_p2l(float3 p, float3 lstart, float3 lend){
     float3 op = p - lstart;
     float3 ol = lend - lstart;
     if(perpendicular(ol) * op < 0)
          return LEFT;
     else
          return RIGHT;
}

__device__ float3 coordinate(float3 p, float3 lstart, float3 lend){
     float3 op = p - lstart;
     float3 ol = lend - lstart;
     // u为y v为x 平方进行归一化
     float u = op * ol / length_square(ol);
     float v = op * perpendicular(ol) / length_square(ol);
     float3 res = make_float3(v, u, 0.f);
     return res;
}

__device__ float gauss(float x, float a=0.5f){
     return 1.f/sqrt(2*PI)*exp(-x*x/a);
}


/* Kernel */
__global__ void add(int width,  int height, float *d){
     int x = blockIdx.x*blockDim.x + threadIdx.x;
     int y = blockIdx.y*blockDim.y + threadIdx.y;
     if(x<width && y<height){
          d[y*height+x] = 1.0;
     }

}

__global__ void test(){
     float3 test0 = make_float3(0.f, 0.f, 0.f);
     float3 test1 = make_float3(0.1f, 0.1f, 0.1f);
     
}


__device__ void
diffusePix(int x, int y, float3 *imageSource, float3 *imageRes, bool *constrained){
     /* constrained */
     if(constrained[y*IMAGE_WIDTH + x] == true){
          imageRes[y*IMAGE_WIDTH + x].x = imageSource[y*IMAGE_WIDTH + x].x;
          imageRes[y*IMAGE_WIDTH + x].y = imageSource[y*IMAGE_WIDTH + x].y;
          imageRes[y*IMAGE_WIDTH + x].z = imageSource[y*IMAGE_WIDTH + x].z;
     }
     
     /* diffuse if not constrained */
     int pixcount = 0;
     imageRes[y*IMAGE_WIDTH + x] = make_float3(0.f, 0.f, 0.f);
     
     /* left: x-1*/
     if(x > 0){
          imageRes[y*IMAGE_WIDTH + x] += imageSource[y*IMAGE_WIDTH + x-1];
          pixcount ++;
     }
     /* right: x+1 */
     if(x < IMAGE_WIDTH-1){
          imageRes[y*IMAGE_WIDTH + x] += imageSource[y*IMAGE_WIDTH + x+1];
          pixcount++;
     }
     /* up: y-1 */
     if(y > 0){
          imageRes[y*IMAGE_WIDTH + x] += imageSource[(y-1)*IMAGE_WIDTH + x];
          pixcount++;
     }
     /* down: y+1 */
     if(y < IMAGE_HEIGHT-1){
          imageRes[y*IMAGE_WIDTH + x] += imageSource[(y+1)*IMAGE_WIDTH + x];
          pixcount++;
     }

     /* calculate the averaged values and store  */
     imageRes[y*IMAGE_WIDTH + x] /= pixcount;
          
}

__global__ void
diffuse(float3 *imageSource, float3 *imageRes, bool *constrained){
     int x = blockIdx.x*blockDim.x + threadIdx.x;
     int y = blockIdx.y*blockDim.y + threadIdx.y;
     if(x<IMAGE_WIDTH && y<IMAGE_HEIGHT){
          diffusePix(x, y, imageSource, imageRes, constrained);
     }
}

__global__ void
updateConstrain(bool *constrained, float3 *imageSource){
     int x = blockIdx.x*blockDim.x + threadIdx.x;
     int y = blockIdx.y*blockDim.y + threadIdx.y;
     if(imageSource[y*IMAGE_WIDTH + x] < 0.00001) /* is 0.0f */
          constrained[y*IMAGE_WIDTH + x] = false;
     else
          constrained[y*IMAGE_WIDTH + x] = true;
}
__device__ float weightPix(float3 p, float3 lstart, float3 lend){
     float res;
     res = gauss( distance_p2l(p, lstart, lend)/105 );
     return res;
}

__device__ void
distancePix(int x, int y, int lineSize, float3 *distMap,
         float3 *pointStart, float3 *pointEnd){

     float3 pix = make_float3(x, y, 0.f);
     float3 color = make_float3(1.f, 1.f, 1.f);
     float dis = INF;
     int id = 0;
     for(int i = 0; i < lineSize; ++i)
     {
          if( dis > distance_p2l(pix, pointStart[i], pointEnd[i]) ){
               dis = distance_p2l(pix, pointStart[i], pointEnd[i]);
               id = i;
          }
     }
     
     distMap[(IMAGE_HEIGHT-1-y)*IMAGE_WIDTH +x] = color * (20.f * id);

}

__global__ void
distanceMap(int lineSize, float3 *distMap, float3 *pointStart, float3 *pointEnd)
{
     int x = blockIdx.x*blockDim.x + threadIdx.x;
     int y = blockIdx.y*blockDim.y + threadIdx.y; 
     if(x<IMAGE_WIDTH && y<IMAGE_HEIGHT){
          distancePix(x, y, lineSize, distMap,
                   pointStart, pointEnd);
     }
}     
__device__ void
fieldPix(int x, int y, int lineSize,
         float3 *imageRes,
         float3 *pointStart, float3 *pointEnd, float3 *pointVector,
         float3 *leftColor, float3 *rightColor){
     /* for each pix */
     float3 pix = make_float3(x, y, 0.f);
     float3 colorSum = make_float3(.0f, .0f, .0f);
     float weightSum = .0f;
     for(int i = 0; i < lineSize; ++i)
     {
          /* weight */
          float w = weightPix(pix, pointStart[i], pointEnd[i]);
          weightSum += w;
          /* set color */
          if(position_p2l(pix, pointStart[i], pointEnd[i]) == LEFT)
               colorSum += leftColor[i] * w;
          else
               colorSum += rightColor[i] * w;
     }

     //colorSum /= weightSum;
     if(colorSum > 255.f)
          setValue(colorSum, 255.f);
     imageRes[(IMAGE_HEIGHT-1-y)*IMAGE_WIDTH +x] = colorSum; /* 坐标上下置换 */
}

__global__ void
field(float3 *imageRes, int lineSize,
      float3 *pointStart, float3 *pointEnd, float3 *pointVector,
      float3 *leftColor, float3 *rightColor)
{
     int x = blockIdx.x*blockDim.x + threadIdx.x;
     int y = blockIdx.y*blockDim.y + threadIdx.y; 
     if(x<IMAGE_WIDTH && y<IMAGE_HEIGHT){
          fieldPix(x, y, lineSize, imageRes,
                   pointStart, pointEnd, pointVector,
                   leftColor, rightColor);
     }
}



/* extern */

extern "C" void
gpuWriteImage(char* filename, int imageSize, float3 *imageRes)
{
     FILE *fp = fopen(filename, "w");
     fprintf(fp, "P3\n%d %d\n%d\n", IMAGE_WIDTH, IMAGE_HEIGHT, 255);
     for(int i=0; i<imageSize; ++i){
          fprintf(fp,  "%d %d %d ", (int)imageRes[i].x, (int)imageRes[i].y, (int)imageRes[i].z);
     }
     fclose(fp);
}

extern "C" void
gpuDiffuseRender(int **image)
{
     printf("Gpu Diffuse Start.....\n");
     float3 *imageSource, *imageRes, *imageTmp;
     bool *constrained;
     unsigned int imageSize = IMAGE_WIDTH * IMAGE_HEIGHT;
     cudaMalloc((void**)&imageSource,
                imageSize*sizeof(float3));
     cudaMalloc((void**)&imageRes,
                imageSize*sizeof(float3));
     cudaMalloc((void**)&constrained,
                imageSize*sizeof(bool));
     imageTmp = new float3[imageSize];

     /* printf("Malloc Succeed\n"); */
     
     /* initialize */
     for(int i=0; i<imageSize; ++i){
          imageTmp[i].x = image[i][0];
          imageTmp[i].y = image[i][1];
          imageTmp[i].z = image[i][2];
     }
     cudaMemcpy(imageSource, imageTmp, imageSize*sizeof(float3), cudaMemcpyHostToDevice);
     
     /* printf("Initialize Succeed\n"); */
     /* set block */
     dim3 block (16,16,1);
     dim3 grid(IMAGE_WIDTH/16,IMAGE_HEIGHT/16,1);

     int it = 100;
     for(int i=0; i<it; ++i){
          updateConstrain<<<grid,block>>>(constrained, imageSource);
          diffuse<<<grid,block>>>(imageSource, imageRes, constrained);
          cudaMemcpy(imageSource, imageRes, imageSize*sizeof(float3), cudaMemcpyDeviceToDevice); /* frome res -> source */
          //printf("\r%d iterator Succeed...", i+1);
     }
     /* test<<<grid,block>>>(); */

     /* write to file */
     printf("Write to file...\n");
     cudaMemcpy(imageTmp, imageRes, imageSize*sizeof(float3), cudaMemcpyDeviceToHost);
     FILE *fp = fopen("image_diffuse.ppm", "w");
     fprintf(fp, "P3\n%d %d\n%d\n", IMAGE_WIDTH, IMAGE_HEIGHT, 255);
     for(int i=0; i<imageSize; ++i){
          fprintf(fp,  "%d %d %d ", (int)imageTmp[i].x, (int)imageTmp[i].y, (int)imageTmp[i].z);
     }
     fclose(fp);
     //printf("Succeed...\n");

     cudaFree(imageSource);
     cudaFree(imageRes);
     cudaFree(constrained);
     delete[] imageTmp;
     return;
}


extern "C" void
gpuFieldRender(float **ps, float **pe, float **pv, int **lc, int **rc, int **iimage, int size){
     /* Malloc */
     float3 *pointStart, *pointEnd, *pointVector;
     float3 *leftColor, *rightColor;
     float3 *imageRes;
     float3 *imageDist;
     int lineSize = size, imageSize = IMAGE_WIDTH*IMAGE_HEIGHT;
     cudaMalloc((void**)&pointStart,lineSize*sizeof(float3));
     cudaMalloc((void**)&pointEnd,lineSize*sizeof(float3));
     cudaMalloc((void**)&pointVector,lineSize*sizeof(float3));
     cudaMalloc((void**)&leftColor,lineSize*sizeof(float3));
     cudaMalloc((void**)&rightColor,lineSize*sizeof(float3));
     cudaMalloc((void**)&imageRes,imageSize*sizeof(float3));
     cudaMalloc((void**)&imageDist,imageSize*sizeof(float3));
     
     /* Initialize */
     float3 *tmp = new float3[lineSize];
     float3 *imageTmp = new float3[imageSize];
     /* pointStart */
     for(int i = 0; i < lineSize; ++i)
     {
          tmp[i].x = ps[i][0];
          tmp[i].y = ps[i][1];
          tmp[i].z = ps[i][2];
     }
     cudaMemcpy(pointStart, tmp, lineSize*sizeof(float3), cudaMemcpyHostToDevice);
     /* pointEnd */
     for(int i = 0; i < lineSize; ++i)
     {
          tmp[i].x = pe[i][0];
          tmp[i].y = pe[i][1];
          tmp[i].z = pe[i][2];
     }
     cudaMemcpy(pointEnd, tmp, lineSize*sizeof(float3), cudaMemcpyHostToDevice);
     /* pointVector */
     for(int i = 0; i < lineSize; ++i)
     {
          tmp[i].x = pv[i][0];
          tmp[i].y = pv[i][1];
          tmp[i].z = pv[i][2];
     }
     cudaMemcpy(pointVector, tmp, lineSize*sizeof(float3), cudaMemcpyHostToDevice);
     /* leftColor */
     for(int i = 0; i < lineSize; ++i)
     {
          tmp[i].x = lc[i][0];
          tmp[i].y = lc[i][1];
          tmp[i].z = lc[i][2];
     }
     cudaMemcpy(leftColor, tmp, lineSize*sizeof(float3), cudaMemcpyHostToDevice);
     /* rightColor */
     for(int i = 0; i < lineSize; ++i)
     {
          tmp[i].x = rc[i][0];
          tmp[i].y = rc[i][1];
          tmp[i].z = rc[i][2];
     }
     cudaMemcpy(rightColor, tmp, lineSize*sizeof(float3), cudaMemcpyHostToDevice);

     /* Initialize Over */
     printf("Total %d Line \n", lineSize);
     /* Render */
     
     /* set block and do */
     dim3 block (16,16,1);
     dim3 grid(IMAGE_WIDTH/16,IMAGE_HEIGHT/16,1);

     distanceMap<<<grid, block>>>(lineSize, imageDist, pointStart, pointEnd);
     field<<<grid, block>>>(imageRes, lineSize, pointStart, pointEnd, pointVector, leftColor, rightColor);

     
     /* feed back result image */
     cudaMemcpy(imageTmp, imageDist, imageSize*sizeof(float3), cudaMemcpyDeviceToHost);
     gpuWriteImage("imageDist.ppm",imageSize, imageTmp);
     
     cudaMemcpy(imageTmp, imageRes, imageSize*sizeof(float3), cudaMemcpyDeviceToHost);

     for(int i = 0; i < imageSize; ++i)
     {
          iimage[i][0] = imageTmp[i].x;
          iimage[i][1] = imageTmp[i].y;
          iimage[i][2] = imageTmp[i].z;
     }

     gpuDiffuseRender(iimage);
     /* free */
     cudaFree(pointStart);
     cudaFree(pointEnd);
     cudaFree(pointVector);
     cudaFree(leftColor);
     cudaFree(rightColor);
     cudaFree(imageRes);
     cudaFree(imageDist);
     delete[] tmp;
     delete[] imageTmp;
}



#endif /* _RENDER_H_ */
