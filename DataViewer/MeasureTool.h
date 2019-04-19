#pragma once
#include "qgsmaptool.h"

#include <qvector.h>

#include <qgsmaptool.h>
#include <qgspointxy.h>
#include <qgsdistancearea.h>

class QgsMapCanvas;
class QgsMeasureDialog;
class QgsRubberBand;
class QgsSnapIndicator;

// 量测工具类
class MeasureTool : public QgsMapTool
{
	Q_OBJECT

public:
	MeasureTool(QgsMapCanvas* canvas, bool measureArea);
	~MeasureTool();

	// 标识当前状态是量测距离还是量测面积
	bool measureArea() const { return m_measureArea; }

	// 标识是否完成绘制
	bool done() const { return m_done; }

	// 添加新点
	void addPoint(const QgsPointXY& point);

	QVector<QgsPointXY> points() const { return m_points; }

	// 重绘
	void restart();

	// 执行量测
	void measure();

	// 重写工具的交互方法
	void canvasMoveEvent(QgsMapMouseEvent *e) override;
	void canvasPressEvent(QgsMapMouseEvent *e) override;
	void canvasReleaseEvent(QgsMapMouseEvent *e) override;
	void activate() override;
	void deactivate() override;
	void keyPressEvent(QKeyEvent *e) override;
	
protected:
	// 标识是否是量测面积，false表示量测长度
	bool m_measureArea = false;
	// 标识量测绘制是否完成
	bool m_done = false;

	// 用于存储用户绘制的点
	QVector<QgsPointXY> m_points;

	// 临时几何图形
	QgsRubberBand* m_rubberBand = nullptr;
	QgsRubberBand* m_rubberBandPoints = nullptr;

	// 用于计算长度和面积
	QgsDistanceArea m_distanceArea;
};

