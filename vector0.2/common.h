#ifndef _COMMON_H_
#define _COMMON_H_


#include <cv.h>
#include <highgui.h>
#include <list>
#include<iostream>
#include<fstream>

/* define 常量定义 */
#define GO_DEBUG false
#define GO_DEBUG_SAVE false
#define GO_DEBUG_MERGE false
static const float INFINITE = 9999.0f;
static const float ZERO = 0.0001f;
static const float COLOR_DELTA = 5.0f;

class VTriangle;

namespace common
{
     void delaunay( IplImage* edge, IplImage* img, IplImage* source );
     void test();
}

#endif /* _COMMON_H_ */
