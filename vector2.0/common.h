#ifndef _COMMON_H_
#define _COMMON_H_


#include <cv.h>
#include <highgui.h>

/* define 常量定义 */
#define GO_DEBUG false
static const float INFINITE = 9999.0f;
static const float ZERO = 0.0001f;

namespace common
{
     void delaunay( IplImage* edge, IplImage* img, IplImage* source );
     void test();

}

#endif /* _COMMON_H_ */
