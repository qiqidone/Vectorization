
#include "vector.h"
#include <iostream>

/****************   class Vdata  ******************/
Vdata::Vdata()
{
     m_source = NULL;
     m_gray = NULL;
     m_edge = NULL;
}
Vdata::~Vdata()
{
     if(m_source != NULL)
     {
          cvReleaseImage( &m_source );
          m_source = NULL;
     }
  
     if(m_gray != NULL)
     {
          cvReleaseImage( &m_gray );
          m_gray = NULL;
     }
     
     if(m_edge != NULL)
     {
          cvReleaseImage( &m_edge );
          m_edge = NULL;
     }
  
     if(m_dst != NULL)
     {
          cvReleaseImage( &m_dst );
          m_dst = NULL;
     }
     
}
// 数据清空
bool Vdata::initial()
{
     if(m_source != NULL)
     {
          cvReleaseImage( &m_source );
          m_source = NULL;
     }
     if(m_gray != NULL)
     {
          cvReleaseImage( &m_gray );
          m_gray = NULL;
     }
  
     if(m_edge != NULL)
     {
          cvReleaseImage( &m_edge );
          m_edge = NULL;
     }

     if(m_dst != NULL)
     {
          cvReleaseImage( &m_dst );
          m_dst = NULL;
     }
     printf("%s", __FUNCTION__);
     return true;
  
}
bool Vdata::initial(const char* filename)
{
     // 先清空
     initial();

     // source
     m_source =  cvLoadImage(filename, 1);
     // gray
     m_gray = cvCreateImage(cvGetSize(m_source), 8, 1);
     cvCvtColor(m_source, m_gray, CV_BGR2GRAY);
     //edge
     m_edge = cvCreateImage(cvGetSize(m_source), 8, 1);
     cvCanny(m_gray, m_edge, 50, 150);
     //dst
     m_dst = cvCreateImage(cvGetSize(m_source), 8, 3);
     return true;
}

/****************   class Vdata end...... ******************/


/****************   class Vgui  ******************/
Vgui::Vgui()
{
     design();
}
Vgui::~Vgui()
{
}
// 设计Gui
bool Vgui::design()
{
     fileButton = new QPushButton(tr("Open"));
     runButton = new QPushButton(tr("Run Fill"));
     meshButton = new QPushButton(tr("Mesh"));
     testButton = new QPushButton(tr("Test"));
     saveButton = new QPushButton(tr("Save"));
     // new完毕，开始布局	
     QVBoxLayout* mainLayout = new QVBoxLayout();
     mainLayout->addWidget(fileButton);
     mainLayout->addWidget(runButton);
     mainLayout->addWidget(meshButton);
     mainLayout->addWidget(testButton);
     mainLayout->addWidget(saveButton);
     
     // main Layout
     setLayout(mainLayout);
     setWindowTitle(tr("Vectorize "));

     // conncet ?
  
     return true;
}
void Vgui::run()
{
     show();
}

/****************   class Vgui end...... ******************/



/*************** class Vshader ********************/
/********************    多线程 的设计 不会全部无响应 主窗口无响应************************/
Vshader::Vshader()
{
     source = NULL;
     edge = NULL;
     dst = NULL;
}
Vshader::~Vshader()
{
     if(edge != NULL)
     {
          cvReleaseImage( &edge );
          edge = NULL;
     }

     if(source != NULL)
     {
          cvReleaseImage( &source );
          source = NULL;
     }
  
     if(dst != NULL)
     {
          cvReleaseImage( &dst );
          dst = NULL;
     }
  
}

void Vshader::run(int mode)
{
     switch(mode)
          {
          case 1:
               shader.trapBall(source, dst);
               break;
          case 2:
               common::delaunay( edge, dst, source );
               break;
          case 3:
               shader.mesh( source, dst );
               break;
          case 4:
               common::test( );
               break;
          default:
               cout << "Mode Error" << endl;
               break;
          }

     exec();
}
bool Vshader::initial(IplImage* img, IplImage* _edge)
{
     if(img == NULL || _edge == NULL)
          return false;
  
     if(source != NULL)
     {
          cvReleaseImage(&source);
          source = NULL;
     }
     source = cvCreateImage(cvGetSize(img), 8, 3);
     cvCopy(img, source);
    
     if(edge != NULL)
     {
          cvReleaseImage(&edge);
          edge = NULL;
     }
     edge = cvCreateImage(cvGetSize(_edge), 8, 1);
     cvCopy(_edge, edge);
  
     if(dst != NULL)
     {
          cvReleaseImage( &dst );
          dst = NULL;
     }
     dst = cvCreateImage(cvGetSize(img), 8, 3);
   
     return shader.initial(edge);
}

/*************** class Vshader  end.......********************/


//	 ==
//				||  Vectorize 2.0 ～ 《设计模式》+ 《Thinking in C++ 第二卷》 +《从小工到专家》
//				||
/****************   class Vectorize . ******************/

Vectorize::Vectorize()
{
     vconncet();
     m_gui.run();
}

Vectorize::~Vectorize()
{
}

bool Vectorize::vconncet()
{
     connect(m_gui.fileButton, SIGNAL(clicked()), this, SLOT(open()));
     connect(m_gui.runButton, SIGNAL(clicked()), this, SLOT(run()));
     connect(m_gui.meshButton, SIGNAL(clicked()), this, SLOT(delaunay()));
     connect(m_gui.testButton, SIGNAL(clicked()), this, SLOT(test()));
     connect(m_gui.saveButton, SIGNAL(clicked()), this, SLOT(save()));
     
     return true;
}

bool Vectorize::open()
{
     QString fileName = QFileDialog::getOpenFileName(&m_gui, tr("Open Image File"), NULL, tr("Images (*.jpg *.png *.bmp)"));

     if(!fileName.isEmpty())
     {
          /* 将QSTRING 类型转换为char* */
          QByteArray temp = fileName.toLocal8Bit();
          const char* c_str = temp.data();

          if( m_data.initial(c_str) )
          {
               if( !m_shader.initial(m_data.m_source, m_data.m_edge) )
               {
                    QMessageBox::about(NULL, "Error", " Shader Initial Error");
               }
          }
          else
          {
               QMessageBox::about(NULL, "Error", "Vdata initial error");
          }
          // assert 断言：用于绝不可能的事，异常用于异常情况
          
     }
     cvShowImage("img", m_data.m_source);

     // open 的同时会导出一份obj
     common::export_to_obj(m_data.m_gray);
     return true;
}

bool Vectorize::run()
{
     if( ! m_shader.initial(m_data.m_source, m_data.m_edge) )
     {
          QMessageBox::about(NULL, "Error", "No data? ");
          return false;
     }
    
     m_shader.run(1);
     m_shader.quit();  
     return true;
}

bool
Vectorize::delaunay()
{
     m_shader.run(2);
     m_shader.quit();
     return true;
}
bool Vectorize::test()
{
     m_shader.run(4);
     m_shader.quit();
     return true;
}
bool Vectorize::save()
{
     if(m_data.m_dst == NULL)
     {
          QMessageBox::about(NULL, "Error", "No data? ");
          return false;
     }
     
      QString fileName = QFileDialog::getSaveFileName(&m_gui, tr("Save Image File"), NULL, tr("Images (*.jpg *.png *.bmp)"));

     if(!fileName.isEmpty())
     {
          /* 将QSTRING 类型转换为char* */
          QByteArray temp = fileName.toLocal8Bit();
          const char* c_str = temp.data();
          cvSaveImage(c_str, m_data.m_dst);
          return true;
     }
    
     return false;
}
/****************   class Vectorize .end ...... ******************/
