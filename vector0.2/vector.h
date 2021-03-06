
#ifndef VECTOR_H
#define VECTOR_H

#include <cv.h>
#include <highgui.h>
#include <QtGui>
#include "shader.h"
#include <QThread>
#include<common.h>
using namespace cv;
using namespace std;

// 就好象auto_ptr封装指针，达到内存自动管理效果一样，封装数据 《小工到专家》
class Vdata
{
 public:
  Vdata();
  ~Vdata();

  bool initial();
  bool initial(const char* filename);
  
 public:
  IplImage* m_source;
  IplImage* m_gray;
  IplImage* m_edge;
  IplImage* m_dst;
 
};

class Vgui : public QWidget
{
  Q_OBJECT
 public:
  Vgui();
  ~Vgui();

  bool design();
  void run();
  
 public:
  	QPushButton* fileButton;
	QPushButton* runButton;
    QPushButton* meshButton;
    QPushButton* testButton;
    QPushButton* saveButton;
};

class Vshader : public QThread
{
   Q_OBJECT
 public:
     Vshader();
    ~Vshader();
 public:

  void run(int mode = 1);
  bool  initial(IplImage* img = NULL, IplImage* _edge = NULL);
  
 private:
  Shader shader;
  IplImage* source;
  IplImage* edge;
  IplImage* dst;
  
};


class Vectorize : public QObject
{
   Q_OBJECT
public:
  Vectorize();
  ~Vectorize();
  bool vconncet();

  
 public:
  Vdata m_data;
  Vgui m_gui;
  Vshader m_shader;
  
  private slots:
    bool open();
    bool run();
    bool delaunay();
    bool test();
    bool save();
};


#endif
