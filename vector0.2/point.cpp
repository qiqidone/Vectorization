#include"point.h"

double VPoint::getElement(int i)
{
     // 从1到3 对应xyz
     switch(i)
     {
     case 1:
          return x;
     case 2:
          return y;
     case 3:
          return z;
     default:
          cout <<  __FUNCTION__ << "\t" << "Error i" << endl;
          return 0.0;
     }
}
void VPoint::setElement(int i, double val)
{
     switch(i)
     {
     case 1:
          x = val;
          break;
     case 2:
          y = val;
          break;
     case 3:
          z = val;
          break;
     default:
          cout <<  __FUNCTION__ << "\t" << "Error i" << endl;
          break;
     }
}




void VPoints::push_back(VPoint r, VPoint g, VPoint b)
{
     r_points.push_back(r);
     g_points.push_back(g);
     b_points.push_back(b);
}

void VPoints::print(ostream& out)
     {
          vector< VPoint >::iterator it;
          int size = r_points.size();
          for (int i = 0; i < size; ++i)
          {
               out << r_points[i].z << endl;
               out << g_points[i].z << endl;
               out << b_points[i].z << endl;               
          }
     }
