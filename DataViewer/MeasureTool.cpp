#include "MeasureTool.h"
#include <qsize.h>
#include <qtextdocument.h>
#include <qstring.h>

#include <qgsrubberband.h>
#include <qgswkbtypes.h>
#include <qgsdistancearea.h>
#include <qgsmapmouseevent.h>
#include <qgstextannotation.h>
#include <qgsannotation.h>
#include <qgsproject.h>
#include <qgsannotationmanager.h>
#include <qgsmapcanvasannotationitem.h>
#include <qgsunittypes.h>
#include <qgsmapcanvas.h>

MeasureTool::MeasureTool(QgsMapCanvas* canvas, bool measureArea)
	: QgsMapTool(canvas)
	, m_measureArea(measureArea)
{
	// 设置Rubber Band的样式
	m_rubberBand = new QgsRubberBand(canvas, m_measureArea ? QgsWkbTypes::PolygonGeometry : QgsWkbTypes::LineGeometry);
	m_rubberBand->setColor(QColor(255, 150, 0, 100));
	m_rubberBand->setWidth(3);
	m_rubberBand->setLineStyle(Qt::PenStyle::DashDotDotLine);

	m_rubberBand->setFillColor(QColor(15, 150, 120, 100));
	m_rubberBand->setBrushStyle(Qt::BrushStyle::CrossPattern);

	// 设置Rubber Band Point的样式
	m_rubberBandPoints = new QgsRubberBand(canvas, QgsWkbTypes::PointGeometry);
	m_rubberBandPoints->setIcon(QgsRubberBand::ICON_CIRCLE);
	m_rubberBandPoints->setIconSize(10);
	m_rubberBandPoints->setColor(QColor(255, 150, 0, 150));
}

MeasureTool::~MeasureTool()
{
}

void MeasureTool::restart()
{
	m_points.clear();
	m_rubberBand->reset(m_measureArea ? QgsWkbTypes::PolygonGeometry : QgsWkbTypes::LineGeometry);
	m_rubberBandPoints->reset(QgsWkbTypes::PointGeometry);

	m_done = true;

	// 删除量测结果
	for each (QgsMapCanvasAnnotationItem* item in this->canvas()->annotationItems())
	{
		delete item;
	}
}

void MeasureTool::measure()
{
	int numPoints = m_points.size();
	if (m_measureArea && numPoints >= 3)
	{
		// 计算多边形面积
		double area = m_distanceArea.measurePolygon(m_points);
		// 将面积转换为平方米
		area = m_distanceArea.convertAreaMeasurement(area, QgsUnitTypes::AreaSquareMeters);
		// 格式化量测结果字符串
		QString aresStr = QgsDistanceArea::formatArea(area, 2, QgsUnitTypes::AreaSquareMeters);
		
		// 将计算结果显示在图形中
		QgsTextAnnotation* annotation = new QgsTextAnnotation();
		QgsPointXY mapPos = m_points[0];
		annotation->setMapPosition(mapPos);
		annotation->setFrameSize(QSizeF(200, 100));
		QTextDocument text;
		annotation->setDocument(&QTextDocument(aresStr));
		QgsMapCanvasAnnotationItem* canvasItem = new QgsMapCanvasAnnotationItem(annotation, this->canvas());
		canvasItem->show();
	}
	else if (!m_measureArea && numPoints >= 1)
	{
		// 量测当前长度
		double length = m_distanceArea.measureLine(m_points);
		// 将长度转换为米
		length = m_distanceArea.convertLengthMeasurement(length, QgsUnitTypes::DistanceMeters);
		// 格式化量测结果字符串
		QString lengthStr = QgsDistanceArea::formatDistance(length, 2, QgsUnitTypes::DistanceMeters);

		// 将计算结果显示在图形中
		QgsTextAnnotation* annotation = new QgsTextAnnotation();
		QgsPointXY mapPos = m_points[m_points.size()-1];
		annotation->setMapPosition(mapPos);
		annotation->setFrameSize(QSizeF(200, 100));
		QTextDocument text;
		annotation->setDocument(&QTextDocument(lengthStr));
		QgsMapCanvasAnnotationItem* canvasItem = new QgsMapCanvasAnnotationItem(annotation, this->canvas());
		canvasItem->show();
	}
}

void MeasureTool::canvasMoveEvent(QgsMapMouseEvent * e)
{
	QgsPointXY point = e->snapPoint();
	if (!m_done)
	{
		m_rubberBand->movePoint(point);
	}
}

void MeasureTool::canvasPressEvent(QgsMapMouseEvent * e)
{
}

void MeasureTool::canvasReleaseEvent(QgsMapMouseEvent * e)
{
	QgsPointXY point = e->snapPoint();
	if (m_done)
	{
		restart();
	}
	if (e->button() == Qt::RightButton)
	{
		m_done = true;
		m_rubberBand->removeLastPoint();
	}
	else if (e->button() == Qt::LeftButton)
	{
		m_done = false;
		addPoint(point);
	}
}

void MeasureTool::activate()
{
	this->setCursor(Qt::CustomCursor);
}

void MeasureTool::deactivate()
{
}

void MeasureTool::keyPressEvent(QKeyEvent* e)
{
}

void MeasureTool::addPoint(const QgsPointXY& point)
{
	if (!m_points.isEmpty() && m_points.last() == point)
	{
		return;
	}
	m_points.append(QgsPointXY(point));
	m_rubberBand->addPoint(point);
	m_rubberBandPoints->addPoint(point);
	
	if (!m_done)
	{
		measure();
	}
}



