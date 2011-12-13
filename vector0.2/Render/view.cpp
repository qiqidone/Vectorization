#include "view.h"
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QDialog>
#include <stdio.h>
#include <cassert>

/*******         RenderControler        *********/
RenderControler::RenderControler(QWidget* parent, RenderViewer* viewer)
     : QWidget(parent)
{
     m_viewer = viewer;
     layoutForDesktop();
}

void RenderControler::createCommonControls(QWidget* parent)
{
     // pointGroup负责绘制
     m_pointGroup = new QGroupBox(parent);
     m_pointGroup->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
     QPushButton *addPoint = new QPushButton(m_pointGroup);
     QPushButton *addLine = new QPushButton(m_pointGroup);
     QPushButton *movePoint = new QPushButton(m_pointGroup);
     QPushButton *moveLine = new QPushButton(m_pointGroup);
     QPushButton *deletePoint = new QPushButton(m_pointGroup);
     QPushButton *deleteLine = new QPushButton(m_pointGroup);
     QPushButton *setColorLeft = new QPushButton(m_pointGroup);
     QPushButton *setColorRight = new QPushButton(m_pointGroup);
     QPushButton *setSigmaLeft = new QPushButton(m_pointGroup);
     QPushButton *setSigmaRight = new QPushButton(m_pointGroup);
     m_pointGroup->setTitle(tr(" Point"));
     addPoint->setText(tr("ADD Point"));
     addLine->setText(tr("ADD  Line"));
     movePoint->setText(tr("Move Point"));
     moveLine->setText(tr("Move  Line"));
     deletePoint->setText(tr("Delete Point"));
     deleteLine->setText(tr("Delete  Line"));
     setColorLeft->setText(tr("Left Color"));
     setColorRight->setText(tr("Right Color"));
     setSigmaLeft->setText(tr("Left Sigma"));
     setSigmaRight->setText(tr("Right Sigma"));
     
     addPoint->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
     addLine->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
     movePoint->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
     moveLine->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
     deletePoint->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
     deleteLine->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
     setColorLeft->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
     setColorRight->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
     setSigmaLeft->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
     setSigmaRight->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
     
     // joinGroup负责lock和unlock
     m_joinGroup = new QGroupBox(parent);
     m_joinGroup->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
     QRadioButton *lockJoin = new QRadioButton(m_joinGroup);
     QRadioButton *unlockJoin = new QRadioButton(m_joinGroup);
     m_joinGroup->setTitle(tr("Lock Style"));
     lockJoin->setText(tr("Lock"));
     unlockJoin->setText(tr("unLock"));

     // styleGroup 负责显示风格设定
     m_styleGroup = new QGroupBox(parent);
     m_styleGroup->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
     QRadioButton *cpu = new QRadioButton(m_styleGroup);
     QRadioButton *gpu = new QRadioButton(m_styleGroup);
     m_styleGroup->setTitle(tr("Render Type"));
     cpu->setText(tr("Cpu"));
     gpu->setText(tr("Gpu"));
     
     // Layouts
     QGridLayout *pointGroupLayout = new QGridLayout(m_pointGroup);
     pointGroupLayout->addWidget(addPoint,0,0);
     pointGroupLayout->addWidget(addLine,0,1);
     pointGroupLayout->addWidget(movePoint,1,0);
     pointGroupLayout->addWidget(moveLine,1,1);
     pointGroupLayout->addWidget(deletePoint,2,0);
     pointGroupLayout->addWidget(deleteLine,2,1);
     pointGroupLayout->addWidget(setColorLeft,3,0);
     pointGroupLayout->addWidget(setColorRight,3,1);
     pointGroupLayout->addWidget(setSigmaLeft,4,0);
     pointGroupLayout->addWidget(setSigmaRight,4,1);
     
     QVBoxLayout *joinGroupLayout = new QVBoxLayout(m_joinGroup);
     joinGroupLayout->addWidget(lockJoin);
     joinGroupLayout->addWidget(unlockJoin);

     QVBoxLayout *styleGroupLayout = new QVBoxLayout(m_styleGroup);
     styleGroupLayout->addWidget(cpu);
     styleGroupLayout->addWidget(gpu);

     // Connenctions
     connect(addPoint, SIGNAL(clicked()), m_viewer, SLOT(setAddPoint()));
     connect(addLine, SIGNAL(clicked()), m_viewer, SLOT(setAddLine()));

     connect(moveLine, SIGNAL(clicked()), m_viewer, SLOT(setMoveLine()));
     connect(movePoint, SIGNAL(clicked()), m_viewer, SLOT(setMovePoint()));

     connect(deleteLine, SIGNAL(clicked()), m_viewer, SLOT(setDeleteLine()));
     connect(deletePoint, SIGNAL(clicked()), m_viewer, SLOT(setDeletePoint()));

     connect(setColorLeft, SIGNAL(clicked()), m_viewer, SLOT(setLineColorLeft()));
     connect(setColorRight, SIGNAL(clicked()), m_viewer, SLOT(setLineColorRight()));

     connect(setSigmaLeft, SIGNAL(clicked()), m_viewer, SLOT(setLineSigmaLeft()));
     connect(setSigmaRight, SIGNAL(clicked()), m_viewer, SLOT(setLineSigmaRight()));
     
     connect(lockJoin, SIGNAL(clicked()), m_viewer, SLOT(setLock()));
     connect(unlockJoin, SIGNAL(clicked()), m_viewer, SLOT(setUnlock()));
     connect(cpu, SIGNAL(clicked()), m_viewer, SLOT(setCPU()));
     connect(gpu, SIGNAL(clicked()), m_viewer, SLOT(setGPU()));
     
     // set the defaults:
     lockJoin->setChecked(true);
     cpu->setChecked(true);
}

void RenderControler::layoutForDesktop()
{
     QGroupBox *mainGroup = new QGroupBox(this);
     //mainGroup->setFixedWidth(180);
     mainGroup->setTitle(tr("Radial Point"));

     createCommonControls(mainGroup);

     // Render Function
     QComboBox* rayFunctionCombo = new QComboBox(mainGroup);
     rayFunctionCombo->insertItem(0, tr("Ray Render"));
     rayFunctionCombo->insertItem(1, tr("Field Render"));
     rayFunctionCombo->insertItem(2, tr("Diffusion Render"));
     // pen width
     QGroupBox* penWidthGroup = new QGroupBox(mainGroup);
     QSlider *penWidth = new QSlider(Qt::Horizontal, penWidthGroup);
     penWidth->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
     penWidthGroup->setTitle(tr("Ray Num"));
     penWidth->setRange(0, 100);
     // engine
     QPushButton *engineButton = new QPushButton(mainGroup);
     engineButton->setText(tr("Engine"));
     // engineButton->setCheckable(true);
     // show image
     QPushButton *showButton = new QPushButton(mainGroup);
     showButton->setText(tr("Show Image"));
     // showButton->setCheckable(true);
     // show txt
     QPushButton *textButton = new QPushButton(mainGroup);
     textButton->setText(tr("Show Data"));
     textButton->setCheckable(true);
     
     // about
     QPushButton *whatsThisButton = new QPushButton(mainGroup);
     whatsThisButton->setText(tr("What's This?"));
     whatsThisButton->setCheckable(true);

     // Layouts:
     QVBoxLayout *penWidthLayout = new QVBoxLayout(penWidthGroup);
     penWidthLayout->addWidget(penWidth);
     QVBoxLayout * mainLayout = new QVBoxLayout(this);
     mainLayout->addWidget(mainGroup);
     QVBoxLayout *mainGroupLayout = new QVBoxLayout(mainGroup);
     mainGroupLayout->setMargin(10);
     mainGroupLayout->addWidget(m_pointGroup);   
     mainGroupLayout->addWidget(m_joinGroup);    
     mainGroupLayout->addWidget(m_styleGroup);   
     mainGroupLayout->addWidget(rayFunctionCombo);
     mainGroupLayout->addWidget(penWidthGroup);  

     mainGroupLayout->addStretch();
     mainGroupLayout->addWidget(engineButton);
     mainGroupLayout->addWidget(showButton);
     mainGroupLayout->addWidget(textButton);
     
     mainGroupLayout->addWidget(whatsThisButton);

     //Connection
     connect(rayFunctionCombo, SIGNAL(currentIndexChanged(int)),
             m_viewer, SLOT(setRayFunction(int)));
     connect(penWidth, SIGNAL(valueChanged(int)),
             m_viewer, SLOT(setPenWidth(int)));
     connect(engineButton, SIGNAL(clicked()),
             m_viewer, SLOT(engineStart()));
     connect(showButton, SIGNAL(clicked()),
             m_viewer, SLOT(showImage()));
     connect(textButton, SIGNAL(clicked()),
             m_viewer, SLOT(showData()));
     
     // Set defaults
     penWidth->setValue(30);
}     


/**********************************************/

/*******         RenderViewer        *********/

/*********************************************/

RenderViewer::RenderViewer(QWidget *parent)
  : QWidget(parent)
{
     setAutoFillBackground(true);
     QPalette palette;
     palette.setColor(QPalette::Background, QColor(250,250,250,125));
     //palette.setBrush(QPalette::Background, QBrush(QPixmap(":/background.png")));
     setPalette(palette);

     // member
     m_activePoint = -1;
     m_resLabel = new QLabel();
     m_image = new QPixmap("image.ppm");
     
     initializePoints();
     update();
}

void RenderViewer::initializePoints()
{
     // m_point
     m_points.push_back( QPoint(250, 210) );

     // m_vector
     m_vectors.push_back( QPoint(100,0) );

     // m_leftColor
     m_leftColor.push_back( QColor(255,255,255) );
     // m_rightColor
     m_rightColor.push_back( QColor(255,255,255) );

     // sigma
     m_leftSigma.push_back(1.0);
     m_rightSigma.push_back(1.0);

     
     // vector map to point 
     for(int i = 0; i < m_vectors.size(); ++i)
     {
          m_vectorMapping.insert(i, i);
     }
     
     // finger Map hash table
     for(int i=0; i<512; ++i){
          for(int j=0; j<512; ++j){
               for(int k=0; k<m_points.size(); ++k){
                    // 范围10以内
                    if(abs( i-m_points.at(k).x() ) < 10 &&
                       abs( j-m_points.at(k).y() ) < 10 ){
                         m_fingerPointMapping.insert(i*j, k);
                         break;
                    }
               }
          }
     }
     //fprintf(stderr, "Hash table with size %d", m_fingerPointMapping.size());
     m_pointCount = m_points.size();
     m_pointSize = 6;


}

void RenderViewer::paintEvent(QPaintEvent *p)
{
     QPainter painter(this);
     painter.setRenderHint(QPainter::Antialiasing, true);

     draw(&painter);
}
    
void RenderViewer::draw(QPainter *painter)
{
     if(m_points.isEmpty())
          initializePoints();
     painter->setRenderHint(QPainter::Antialiasing);

     QPalette pal = palette();
     painter->setPen(Qt::NoPen);

     if(1){
          // draw the points
          // fprintf(stderr, "Paint Points\n");
          painter->setPen( QColor(50, 100, 120, 200) );
          painter->setBrush(QColor(200, 200, 210, 120));
          for(int i=0; i<m_points.size(); ++i){
               QPointF pos = m_points.at(i);
               painter->drawEllipse(pos,m_pointSize, m_pointSize);
          }
          // active point
          if(m_activePoint >= 0 && m_activePoint < m_points.size())
          {
               painter->setPen( QColor(150, 10, 10, 200) );
               painter->setBrush( QColor(0, 0, 0, 120) );
               QPointF pos = m_points.at(m_activePoint);
               painter->drawEllipse(pos, m_pointSize, m_pointSize);
          }     

          // draw line & end point
          // fprintf(stderr, "Paint Lines\n");
          for(int i=0; i<m_vectors.size(); ++i){
               int start =  m_vectorMapping.find(i).value(); 
               QPointF pos_start = m_points.at(start);
               QPointF pos_end = m_vectors.at(i) + pos_start;
               painter->setPen( QColor(0, 0, 0, 120) );
               painter->setBrush(QColor(0, 0, 0, 120));
               painter->drawEllipse(pos_end, m_pointSize/2, m_pointSize/2);
               painter->setPen( QColor(200, 50, 50, 200) );
               painter->drawLine(pos_start, pos_end);
          }
          // temp line @ add line
          if(m_state == ADD_LINE)
               painter->drawLine(m_lineStart, m_lineEnd);
          // active line
          if(m_activeLine >= 0 && m_activeLine < m_vectors.size()){
               int start =  m_vectorMapping.find(m_activeLine).value();
               QPointF pos_start = m_points.at(start);
               QPointF pos_end = m_vectors.at(m_activeLine) + pos_start;
               painter->setPen( QColor(0, 200, 0, 120) );
               painter->setBrush(QColor(0, 0, 200, 120));
               painter->drawEllipse(pos_end, m_pointSize/2, m_pointSize/2);
               
               painter->setPen( QColor(200, 250, 50, 200) );
               painter->drawLine(pos_start, pos_end);
          }
          
     }

          
}
void
RenderViewer::mousePressEvent(QMouseEvent* e)
{
     fprintf(stderr, "Mouse Press ; State = %s\n",STATE_NAME[m_state]);
     
     m_activeLine = -1;
     m_activePoint = -1;
     qreal distance = -1;
     
     // find the nearest point
     for(int i=0; i<m_points.size(); ++i){
          qreal d = QLineF(e->pos(), m_points.at(i)).length();
          if((distance < 0 && d < m_pointSize) || d < distance){
               distance = d;
               m_activePoint = i;
          }
     }
     // 最近的端点 active line
     if(m_activePoint == -1) 
     {
          qreal dis = -1;
          for(int i=0; i<m_vectors.size(); ++i)
          {
               QPointF end = m_points[ m_vectorMapping[i] ] + m_vectors[i];
               qreal d = QLineF(e->pos(), end).length();
               if( (dis<0 && d<m_pointSize) || d < dis)
               {
                    dis = d;
                    m_activeLine = i;
               }
          }

     }
     // with QHash 每次变化都要更新 对静态的对象好用
     // QHash<int, int>::const_iterator it =
     //      m_fingerPointMapping.find(e->pos().x() * e->pos().y());
     // if( it != m_fingerPointMapping.end() )
     // {
     //      printf("find in hash\n");
     //      m_activePoint = it.value();
     // }


     if(m_activePoint == -1 && m_state == ADD_POINT){
          QPoint p = e->pos();
          m_points.push_back(p);
          m_state = DO_NOTHING;
          update();
     }
     
     if(m_activePoint == -1 && m_state == ADD_LINE){
          QPoint p = e->pos();
          printf("Select no point!\n");
     }
     
     if(m_activePoint != -1 && m_state == MOVE_POINT){
          fprintf(stderr, "Point %d seleted\n", m_activePoint);
          m_mouseDrag = true;
          mouseMoveEvent(e);
     }
     if(m_activeLine != -1 && m_state == MOVE_LINE){
          mouseMoveEvent(e);
     }
     // delete line | point
     if(m_activeLine != -1 && m_state == DELETE_LINE ){
          deleteLine(m_activeLine);
     }
     if(m_activePoint != -1 && m_state == DELETE_POINT){
          deletePoint(m_activePoint);
     }

     // set Color
     if(m_activeLine != -1 && (m_state == SET_LEFT_COLOR || m_state == SET_RIGHT_COLOR)){
          // 设置颜色 并且线选中
          //printf("choose color\n");
          chooseColor();
     }
          // set Sigma
     if(m_activeLine != -1 && (m_state == SET_LEFT_SIGMA || m_state == SET_RIGHT_SIGMA)){
          // 设置颜色 并且线选中
          //printf("choose color\n");
          chooseSigma();
     }
     m_mousePress = e->pos();
}

void
RenderViewer::mouseMoveEvent(QMouseEvent *e)
{
     // fprintf(stderr, "Mouse Move\n");
     // moved more then 10 pixels -> dragging
     if(m_state == MOVE_POINT &&
        QPoint(m_mousePress - e->pos()).manhattanLength() > 10)
          m_mouseDrag = true;

     // printf("Avtive Point: %d\n", m_activePoint);
     if(m_mouseDrag == true &&
        m_state == MOVE_POINT &&
        m_activePoint >= 0 &&
        m_activePoint < m_points.size()){
          printf("%d %d\n", e->pos().x(), e->pos().y());
          m_points[m_activePoint] = e->pos();
          update();
     }

     if(m_state == MOVE_LINE &&
          m_activeLine >=0 &&
        m_activeLine < m_vectors.size()){
          QPointF start = m_points[ m_vectorMapping[m_activeLine] ];
          m_vectors[m_activeLine] = e->pos() - start;
          update();
     }
     if(m_state == ADD_LINE &&
        m_activePoint >=0 &&
        m_activePoint < m_points.size())
     {
          m_lineStart = m_points.at(m_activePoint).toPoint();
          m_lineEnd = e->pos();
          
          update();
     }
}

void RenderViewer::mouseReleaseEvent(QMouseEvent *e)
{
     fprintf(stderr, "Mouse Release ; State = %s end\n",STATE_NAME[m_state]);
     if(m_state == ADD_LINE)
     {
          m_vectors.push_back( m_lineEnd-m_lineStart );
          m_vectorMapping.insert(m_vectors.size()-1, m_activePoint);

          m_leftColor.push_back( QColor(250,0,0) );
          m_rightColor.push_back( QColor(250,0,0) );

          m_leftSigma.push_back(1.0);
          m_rightSigma.push_back(1.0);
     }
     else if(m_state == SET_LEFT_COLOR || m_state == SET_RIGHT_COLOR)
     {
          return;
     }
     else if(m_state == SET_LEFT_SIGMA || m_state == SET_RIGHT_SIGMA)
     {
          return;
     }
     m_activePoint = -1;
     m_activeLine = -1;
     m_state = DO_NOTHING;

     update();
}


void RenderViewer::chooseColor()
{
     // 
     QColor color = QColorDialog::getColor(Qt::red, this);
     if(m_state == SET_LEFT_COLOR){
          m_leftColor[m_activeLine] = color;
          printf("%d\n", m_activeLine);
     }
     else if(m_state == SET_RIGHT_COLOR){
          m_rightColor[m_activeLine] = color;
          printf("%d\n", m_activeLine);
     }
     else{
          fprintf(stderr, "Error: How can you get here %s\nAnswer: Mouse released. and I'm here.\n", __FUNCTION__);
     }
     
     printf("Color :%d %d %d \n", color.red(), color.green(), color.blue());
}

void RenderViewer::chooseSigma()
{
     CancellationDialog cd;
     QDoubleSpinBox *sb = cd.m_pthreshold;
     sb->setRange(0.0, 10.0);
     sb->setValue(0.0);
     sb->setSingleStep(0.01);
     cd.exec();
     qreal sigma = sb->value();
     fprintf(stderr, "get sigma %.2f\n", sigma);
     if(m_state == SET_LEFT_SIGMA){
          m_leftSigma[m_activeLine] = sigma;
//          printf("%d\n", m_activeLine);
     }
     else if(m_state == SET_RIGHT_SIGMA){
          m_rightSigma[m_activeLine] = sigma;
//          printf("%d\n", m_activeLine);
     }
     else{
          fprintf(stderr, "Error: How can you get here %s\nAnswer: Mouse released. and I'm here.\n", __FUNCTION__);
     }
     
     printf("Sigma : %.2f \n", sigma);

}

void RenderViewer::deletePoint(int point){
     if(point < 0)
          return ;

     for(int i = 0; i < m_vectorMapping.size(); ++i)
     {
          if(m_vectorMapping[i] == m_activePoint)
          {
               m_leftColor.remove(i);
               m_rightColor.remove(i);
               m_leftSigma.remove(i);
               m_rightSigma.remove(i);
               m_vectors.remove(i);
               m_vectorMapping.remove(i);
          }
               
     }

     m_points.remove(m_activePoint);

     assert(m_vectors.size() == m_vectorMapping.size());
     assert(m_vectors.size() == m_leftColor.size());
     assert(m_vectors.size() == m_leftSigma.size());
//      assert(m_vectors.size() == m_points.size());

     update();
}

void RenderViewer::deleteLine(int line){
     if(line < 0) return ;

     m_vectors.remove(m_activeLine);
     m_vectorMapping.remove(m_activeLine);
     m_leftColor.remove(m_activeLine);
     m_rightColor.remove(m_activeLine);
     m_leftSigma.remove(m_activeLine);
     m_rightSigma.remove(m_activeLine);
     
     assert(m_vectors.size() == m_vectorMapping.size());
     assert(m_vectors.size() == m_leftColor.size());
     assert(m_vectors.size() == m_leftSigma.size());

     update();
}
void RenderViewer::engineStart()
{
     // output file
     FILE *f = fopen("data.txt", "w");
     fprintf(f, "# test\n");

     fprintf(f, "c %d %d\n", m_rayNum, m_rayFuntion);
     for(int i=0; i<m_points.size(); ++i)
     {
          fprintf(f, "v %d %.2f %.2f\n",
                  i+1,
                  m_points[i].x(),
                  512 - m_points[i].y());
     }

     // 文件里前n个是起点
     int n = m_points.size()+1;
     QHash<int, int>::const_iterator it = m_vectorMapping.begin();
     for(; it != m_vectorMapping.end(); ++it)
     {
          QPointF end = m_vectors[it.key()] + m_points[it.value()];
          fprintf(f, "v %d %.2f %.2f\n",
                  n++,
                  end.x(),
                  512 - end.y());
     }

     n = m_points.size() + 1;
     int m = 1;
     for(it=m_vectorMapping.begin(); it != m_vectorMapping.end(); ++it)
     {
          // 
          fprintf(f, "l %d %d %d %d %d %d %d %d %d %d %d %.2f %.2f\n",
                  m++,
                  it.value()+1,
                  it.key()+n,
                  m_leftColor[it.key()].red(),
                  m_leftColor[it.key()].green(),
                  m_leftColor[it.key()].blue(),
                  m_leftColor[it.key()].alpha(),
                  m_rightColor[it.key()].red(),
                  m_rightColor[it.key()].green(),
                  m_rightColor[it.key()].blue(),
                  m_rightColor[it.key()].alpha(),
                  m_leftSigma[it.key()],
                  m_rightSigma[it.key()]);
              
     }

     fprintf(f, "# end\n");
     fclose(f);

     fprintf(stderr, "Render start.\n");
     m_render.run();
     fprintf(stderr, "\rRender end.\n");

     // show
     m_image->load("image.ppm");
     m_resLabel->setPixmap(*m_image);
     m_resLabel->show();
}     

void RenderViewer::showImage()
{
     m_image->load("image.ppm");
     m_resLabel->setPixmap(*m_image);
     m_resLabel->show();

}

void RenderViewer::showData()
{
     QMessageBox msgBox;
     msgBox.setText("The document has been modified.");
     msgBox.exec();
}     

/*******         RenderWidget        *********/
RenderWidget::RenderWidget(QWidget* parent)
     : QWidget(parent)
{
     setWindowTitle(tr("Radial Render"));

     // Widget construction
     m_viewer = new RenderViewer(this);
     m_viewer->setFixedSize( QSize(512, 512) );
     m_controler = new RenderControler(this, m_viewer); // parent = NULL?
     m_controler->setFixedWidth(m_controler->sizeHint().width());

     // Layout
     QHBoxLayout *layout = new QHBoxLayout(this);
     layout->addWidget(m_controler);
     layout->addWidget(m_viewer);
    

     // connection
     connect(m_controler, SIGNAL(quitPressed()),
             QApplication::instance(), SLOT(quit()));
}


/********** SpinBox Dialog **********/
CancellationDialog::CancellationDialog(QWidget *parent)
:QDialog(parent)
{
	label = new QLabel(tr("Threshold:"));
	m_pthreshold = new QDoubleSpinBox;
	m_pthreshold->setDecimals(3);

	QSpacerItem *spacerItem = new QSpacerItem(131, 31, QSizePolicy::Minimum, QSizePolicy::Expanding);


	okButton = new QPushButton(tr("OK"));
	cancelButton = new QPushButton(tr("Close"));

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(label);
	layout->addWidget(m_pthreshold);


	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->addItem(spacerItem);
	hlayout->addWidget(okButton);
	hlayout->addWidget(cancelButton);

	QVBoxLayout *vlayout = new QVBoxLayout;
	vlayout->addLayout(layout);
	vlayout->addLayout(hlayout);

	setLayout(vlayout);

	setWindowTitle(tr("Select a eigenvector"));
	setFixedHeight(sizeHint().height());
}            

