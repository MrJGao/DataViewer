#include "dataviewer.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <QDomDocument>
#include "MeasureTool.h"

#include <qgsmapcanvas.h>
#include <qgslayertreeview.h>
#include <qgslayertreemodel.h>
#include <qgslayertreemapcanvasbridge.h>
#include <qgslayertreeviewdefaultactions.h>
#include <qgslayertree.h>
#include <qgsvectorlayer.h>
#include <qgsrasterlayer.h>
#include <qgsmaptoolzoom.h>
#include <qgsmaptoolpan.h>
#include <qgsrectangle.h>


DataViewer::DataViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// 初始化地图画布
	m_mapCanvas = new QgsMapCanvas();
	this->setCentralWidget(m_mapCanvas);

	// 初始化图层管理器
	m_layerTreeView = new QgsLayerTreeView(this);
	initLayerTreeView();

	// 初始化地图工具
	m_zoomInTool = new QgsMapToolZoom(m_mapCanvas, false);
	m_zoomOutTool = new QgsMapToolZoom(m_mapCanvas, true);
	m_panTool = new QgsMapToolPan(m_mapCanvas);
	m_measureLineTool = new MeasureTool(m_mapCanvas, false);
	m_measureAreaTool = new MeasureTool(m_mapCanvas, true);

	// 初始化书签窗口
	m_bookmarkDlg = new BookMarkDialog(m_mapCanvas);
}

DataViewer::~DataViewer()
{
	
}

void DataViewer::on_actionOpenProject_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, QStringLiteral("选择工程文件"),
		"", "QGIS project (*.qgs)");
	QFileInfo fi(filename);
	if (!fi.exists()) { return; }
	QgsProject::instance()->read(filename);
}

void DataViewer::on_actionSaveProject_triggered()
{
	QString filename = QFileDialog::getSaveFileName(this, QStringLiteral("工程保存为"),
		"", "QGIS project (*.qgs)");
	QFileInfo file(filename);
	QgsProject::instance()->write(filename);
}

void DataViewer::on_actionSaveAsProject_triggered()
{

}

void DataViewer::on_actionAddVectorData_triggered()
{
	QStringList layerPathList = QFileDialog::getOpenFileNames(this, QStringLiteral("选择矢量数据"), "", "shapefile (*.shp)");
	QList<QgsMapLayer*> layerList;
	for each (QString layerPath in layerPathList)
	{
		QFileInfo fi(layerPath);
		if (!fi.exists()) { return; }
		QString layerBaseName = fi.baseName(); // 图层名称

		QgsVectorLayer* vecLayer = new QgsVectorLayer(layerPath, layerBaseName);
		if (!vecLayer) { return; }
		if (!vecLayer->isValid())
		{
			QMessageBox::information(0, "", "layer is invalid");
			return;
		}
		layerList << vecLayer;
	}
	
	QgsProject::instance()->addMapLayers(layerList);
	m_mapCanvas->refresh();
}

void DataViewer::on_actionAddRasterData_triggered()
{
	QStringList layerPathList = QFileDialog::getOpenFileNames(this, QStringLiteral("选择栅格数据"), "", "Image (*.img *.tif *.tiff)");
	QList<QgsMapLayer*> layerList;
	for each (QString layerPath in layerPathList)
	{
		QFileInfo fi(layerPath);
		if (!fi.exists()) { return; }
		QString layerBaseName = fi.baseName(); // 图层名称

		QgsRasterLayer* rasterLayer = new QgsRasterLayer(layerPath, layerBaseName);
		if (!rasterLayer) { return; }
		if (!rasterLayer->isValid())
		{
			QMessageBox::information(0, "", "layer is invalid");
			return;
		}
		layerList << rasterLayer;
	}

	QgsProject::instance()->addMapLayers(layerList);
	m_mapCanvas->refresh();
}

void DataViewer::on_actionAddWmsLayer_triggered()
{

}

void DataViewer::on_actionAddWfsLayer_triggered()
{

}

void DataViewer::on_actionAddWcsLayer_triggered()
{

}

void DataViewer::on_actionZoomIn_triggered()
{
	m_mapCanvas->setMapTool(m_zoomInTool);
}

void DataViewer::on_actionZoomOut_triggered()
{
	m_mapCanvas->setMapTool(m_zoomOutTool);
}

void DataViewer::on_actionPan_triggered()
{
	m_mapCanvas->setMapTool(m_panTool);
}

void DataViewer::on_actionZoomActual_triggered()
{
	
}

void DataViewer::on_actionFullExtent_triggered()
{
	m_mapCanvas->zoomToFullExtent();
}

void DataViewer::on_actionMeasureLine_triggered()
{
	m_mapCanvas->setMapTool(m_measureLineTool);
}

void DataViewer::on_actionMeasureArea_triggered()
{
	m_mapCanvas->setMapTool(m_measureAreaTool);
}

void DataViewer::on_actionMeasureAngle_triggered()
{
}

void DataViewer::on_actionNewBookmark_triggered()
{
	bool ok;
	QString text = QInputDialog::getText(this, QStringLiteral("输入书签名称"), "", QLineEdit::Normal, "", &ok);
	if (ok && !text.isEmpty())
	{
		m_bookmarkDlg->add(text, m_mapCanvas->extent());
	}
}

void DataViewer::on_actionShowBookmarks_triggered()
{
	m_bookmarkDlg->show();
}

void DataViewer::on_actionLayerTreeControl_treggered()
{
	// 打开图层管理器
	if (ui.LayerTreeControl->isVisible())
	{
		ui.LayerTreeControl->setFocus();
	}
	else
	{
		ui.LayerTreeControl->show();
	}
}

void DataViewer::on_actionOverviewMap_triggered()
{
	// 打开鹰眼图
	if (ui.OverviewMap->isVisible())
	{
		ui.OverviewMap->setFocus();
	}
	else
	{
		ui.OverviewMap->show();
	}
}

void DataViewer::removeLayer()
{
	if (!m_layerTreeView) { return; }

	QModelIndexList indexes;
	while ((indexes = m_layerTreeView->selectionModel()->selectedRows()).size()) {
		m_layerTreeView->model()->removeRow(indexes.first().row());
	}
}

void DataViewer::initLayerTreeView()
{
	QgsLayerTreeModel* model = new QgsLayerTreeModel(QgsProject::instance()->layerTreeRoot(), this);
	qDebug() << QgsProject::instance()->layerTreeRoot();
	model->setFlag(QgsLayerTreeModel::AllowNodeReorder);
	model->setFlag(QgsLayerTreeModel::AllowNodeRename);
	model->setFlag(QgsLayerTreeModel::AllowNodeChangeVisibility);
	model->setFlag(QgsLayerTreeModel::ShowLegendAsTree);
	model->setFlag(QgsLayerTreeModel::UseEmbeddedWidgets);
	model->setAutoCollapseLegendNodes(10);
	m_layerTreeView->setModel(model);

	m_layerTreeCanvasBridge = new QgsLayerTreeMapCanvasBridge(QgsProject::instance()->layerTreeRoot(), m_mapCanvas, this);
	connect(QgsProject::instance(), SIGNAL(writeProject(QDomDocument&)), m_layerTreeCanvasBridge, SLOT(writeProject(QDomDocument&)));
	connect(QgsProject::instance(), SIGNAL(readProject(QDomDocument)), m_layerTreeCanvasBridge, SLOT(readProject(QDomDocument)));

	// 添加组命令
	QAction* actionAddGroup = new QAction(QStringLiteral("添加组"), this);
	actionAddGroup->setIcon(QIcon(QStringLiteral(":/images/Resources/mActionAddGroup.svg")));
	actionAddGroup->setToolTip(QStringLiteral("添加组"));
	connect(actionAddGroup, &QAction::triggered, m_layerTreeView->defaultActions(), &QgsLayerTreeViewDefaultActions::addGroup);

	// 扩展和收缩图层树
	QAction* actionExpandAll = new QAction(QStringLiteral("展开所有组"), this);
	actionExpandAll->setIcon(QIcon(QStringLiteral(":/images/Resources/mActionExpandTree.svg")));
	actionExpandAll->setToolTip(QStringLiteral("展开所有组"));
	connect(actionExpandAll, &QAction::triggered, m_layerTreeView, &QgsLayerTreeView::expandAllNodes);
	QAction* actionCollapseAll = new QAction(QStringLiteral("折叠所有组"), this);
	actionCollapseAll->setIcon(QIcon(QStringLiteral(":/images/Resources/mActionCollapseTree.svg")));
	actionCollapseAll->setToolTip(QStringLiteral("折叠所有组"));
	connect(actionCollapseAll, &QAction::triggered, m_layerTreeView, &QgsLayerTreeView::collapseAllNodes);

	// 移除图层
	QAction* actionRemoveLayer = new QAction(QStringLiteral("移除图层/组"));
	actionRemoveLayer->setIcon(QIcon(QStringLiteral(":/images/Resources/mActionRemoveLayer.svg")));
	actionRemoveLayer->setToolTip(QStringLiteral("移除图层/组"));
	connect(actionRemoveLayer, &QAction::triggered, this, &DataViewer::removeLayer);

	QToolBar* toolbar = new QToolBar();
	toolbar->setIconSize(QSize(16, 16));
	toolbar->addAction(actionAddGroup);
	toolbar->addAction(actionExpandAll);
	toolbar->addAction(actionCollapseAll);
	toolbar->addAction(actionRemoveLayer);

	QVBoxLayout* vBoxLayout = new QVBoxLayout();
	vBoxLayout->setMargin(0);
	vBoxLayout->setContentsMargins(0,0,0,0);
	vBoxLayout->setSpacing(0);
	vBoxLayout->addWidget(toolbar);
	vBoxLayout->addWidget(m_layerTreeView);

	QWidget* w = new QWidget;
	w->setLayout(vBoxLayout);
	this->ui.LayerTreeControl->setWidget(w);
}





