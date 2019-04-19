#include "dataviewer.h"
#include <QtWidgets/QApplication>

#include <qgsapplication.h>



int main(int argc, char *argv[])
{
	QgsApplication a(argc, argv, true);
	QgsApplication::setPrefixPath("C:/OSGeo4W64/apps/qgis-dev", true); // !!!这个路径参数需要自行修改
	QgsApplication::initQgis();
	DataViewer w;
	w.show();
	return a.exec();
}
