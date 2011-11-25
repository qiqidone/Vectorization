#include "radial.h"
#include "view.h"
#include <QtGui/QApplication>

using namespace Radial;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    RadialImage renderer(512, 512);
//    renderer.run();
    //exit(0);
    RenderWidget render;
    render.show();
	
	return a.exec();
}
