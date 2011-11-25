#ifndef _VIEW_H_
#define _VIEW_H_

#include <QtGui>
#include "radial.h"

enum EditStates
{
     DO_NOTHING,
     ADD_POINT,
     ADD_LINE,
     MOVE_POINT,
     MOVE_LINE,
     DELETE_POINT,
     DELETE_LINE,
     SET_LEFT_COLOR,
     SET_RIGHT_COLOR,
     SET_LEFT_SIGMA,
     SET_RIGHT_SIGMA,
     LOCKED,
};

enum RenderTypes
{
     CPU,
     GPU,
};


static const char* STATE_NAME[] = {
     "DO_NOTHING",
     "ADD_POINT",
     "ADD_LINE",
     "MOVE_POINT",
     "MOVE_LINE",
     "DELETE_POINT",
     "DELETE_LINE",
     "SET_LEFT_COLOR",
     "SET_RIGHT_COLOR",
     "SET_LEFT_SIGMA",
     "SET_RIGHT_SIGMA",
     "LOCKED"
};

static const char* RENDER_TYPE[] = {
     "CPU",
     "GPU"     
};

struct QRadialLine
{
     QPointF *point;
     QLine *line;
     QColor *left_color;
     QColor *right_color;
};


using Radial::RadialImage;
class RenderViewer : public QWidget
{
     Q_OBJECT
public:
     RenderViewer(QWidget* parent);
     
     void draw(QPainter* painter);
     void mousePressEvent(QMouseEvent* e);
     void mouseMoveEvent(QMouseEvent* e);
     void mouseReleaseEvent(QMouseEvent* e);
     //void timeEvent(QTimerEvent* e){}

     QSize sizeHint() const { return QSize(512, 512); }
     void setRealPenWidth(qreal penWidth) { m_penWidth = penWidth; update(); }
     void chooseColor();
     void chooseSigma();
     void deletePoint(int point);
     void deleteLine(int line);
     
protected:
     void paintEvent(QPaintEvent* p);
        
public slots:
     void setPenWidth(int penWidth) { m_penWidth = penWidth / 10.0; update(); }
     void setAddPoint() { m_state = ADD_POINT; update(); }
     void setAddLine(){ m_state = ADD_LINE; update(); }
     void setMovePoint(){ m_state = MOVE_POINT; update(); }
     void setMoveLine(){ m_state = MOVE_LINE; update(); }
     void setDeletePoint(){ m_state = DELETE_POINT; update(); }
     void setDeleteLine(){ m_state = DELETE_LINE; update(); }
     void setLock(){ m_state = LOCKED; update(); }
     void setUnlock(){ m_state = DO_NOTHING; update(); }
     void setCPU(){ m_render.renderType = CPU; update(); }
     void setGPU(){ m_render.renderType = GPU; update(); }
     void setLineColorLeft() { m_state = SET_LEFT_COLOR; update(); }
     void setLineColorRight() { m_state = SET_RIGHT_COLOR; update(); }
     void setLineSigmaLeft() { m_state = SET_LEFT_SIGMA; update(); }
     void setLineSigmaRight() { m_state = SET_RIGHT_SIGMA; update(); }
     void engineStart();
     void showImage();
     void showData();
     
private:
     void initializePoints();
     void updatePoints();

     qreal m_penWidth;
     int m_pointCount;
     int m_pointSize;
     int m_activePoint;
     int m_activeLine;
     QVector<QPointF> m_points; /* start point */
     QVector<QPointF> m_vectors; /* end point */
     QVector<QRadialLine> m_lines;
     QVector<QColor> m_leftColor;
     QVector<QColor> m_rightColor;
     QVector<qreal> m_leftSigma;
     QVector<qreal> m_rightSigma;
     Qt::PenStyle m_penStytle;

     QPoint m_mousePress;
     QPoint m_lineStart;
     QPoint m_lineEnd;
     int m_state;               /* add point or line */
     bool m_mouseDrag;          /* is draging or not */

     RadialImage m_render;
     QLabel *m_resLabel;
     QPixmap *m_image;
     QHash<int, int> m_vectorMapping;
     QHash<int, int> m_fingerPointMapping;
};

class RenderControler : public QWidget
{
     Q_OBJECT
public:
     RenderControler(QWidget* parent, RenderViewer* viewer);

signals:
     void okPressed();
     void quitPressed();

private:
     RenderViewer* m_viewer;

     QGroupBox* m_pointGroup;   /* edit */
     QGroupBox *m_joinGroup;    /*  */
     QGroupBox *m_styleGroup;   /*  */

     void createCommonControls(QWidget* parent);
     void layoutForDesktop();
};

class RenderWidget : public QWidget
{
     Q_OBJECT
public:
     RenderWidget(QWidget* parent=NULL);
     void setStyle(QStyle *style);

     void mousePressEvent(QMouseEvent* e){ printf("father\n");}
private:
     RenderControler *m_controler;
     RenderViewer *m_viewer;

/* private slots: */
/*      void showControls(); */
/*      void hideControls(); */
};

#endif /* _VIEW_H_ */
