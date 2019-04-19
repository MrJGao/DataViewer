#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#define M_PI_2 3.1415926

#include <QtWidgets/QMainWindow>
#include "ui_dataviewer.h"
#include "BookmarkDialog.h"

#include <qgsmapcanvas.h>

class QgsMapTool;

// 数据查看器主窗口
class DataViewer : public QMainWindow
{
	Q_OBJECT

public:
	DataViewer(QWidget *parent = 0);
	~DataViewer();

public slots:
	// 打开工程
	void on_actionOpenProject_triggered();
	// 保存工程
	void on_actionSaveProject_triggered();
	// 另存工程
	void on_actionSaveAsProject_triggered();

	// 添加矢量数据
	void on_actionAddVectorData_triggered();
	// 添加栅格数据
	void on_actionAddRasterData_triggered();
	// 添加WMS图层
	void on_actionAddWmsLayer_triggered();
	// 添加WFS图层
	void on_actionAddWfsLayer_triggered();
	// 添加WCS图层
	void on_actionAddWcsLayer_triggered();

	// 使用地图放大工具
	void on_actionZoomIn_triggered();
	// 使用地图缩小工具
	void on_actionZoomOut_triggered();
	// 使用地图漫游工具
	void on_actionPan_triggered();
	// 缩放地图到实际比例
	void on_actionZoomActual_triggered();
	// 缩放地图到全图范围
	void on_actionFullExtent_triggered();
	// 地图量测工具
	void on_actionMeasureLine_triggered();
	void on_actionMeasureArea_triggered();
	void on_actionMeasureAngle_triggered();

	// 创建新的地图书签
	void on_actionNewBookmark_triggered();
	// 显示地图书签管理器
	void on_actionShowBookmarks_triggered();

	// 图层管理器
	void on_actionLayerTreeControl_treggered();
	// 鹰眼图
	void on_actionOverviewMap_triggered();

private:
	Ui::DataViewerClass ui;

	// 地图画布
	QgsMapCanvas* m_mapCanvas;
	// 图层列表
	QList<QgsMapLayer*> m_layerList;

	// 地图缩放工具
	QgsMapTool* m_zoomInTool;
	QgsMapTool* m_zoomOutTool;
	// 地图浏览工具
	QgsMapTool* m_panTool;

	// 地图量测工具
	QgsMapTool* m_measureLineTool;
	QgsMapTool* m_measureAreaTool;

	// 书签窗体
	BookMarkDialog* m_bookmarkDlg;
};



#endif // DATAVIEWER_H
