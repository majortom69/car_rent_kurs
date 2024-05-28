#include "QtWidgetsApplication1.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    /*AVLTree tree;*/
    QApplication a(argc, argv);
    QtWidgetsApplication1 w;
    
    
    w.show();
    return a.exec();
}
