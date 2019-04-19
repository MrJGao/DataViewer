#include "dataviewer.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <QDomDocument>
#include "MeasureTool.h"

#include <qgsvectorlayer.h>
#include <qgsrasterlayer.h>
#include <qgsmaptoolzoom.h>
#include <qgsmaptoolpan.h>
#include <qgsrectangle.h>


DataViewer::DataViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_mapCanvas = new QgsMapCanvas();
	this->setCentralWidget(m_mapCanvas);

	m_zoomInTool = new QgsMapToolZoom(m_mapCanvas, false);
	m_zoomOutTool = new QgsMapToolZoom(m_mapCanvas, true);
	m_panTool = new QgsMapToolPan(m_mapCanvas);
	m_measureLineTool = new MeasureTool(m_mapCanvas, false);
	m_measureAreaTool = new MeasureTool(m_mapCanvas, true);

	m_bookmarkDlg = new BookMarkDialog(m_mapCanvas);
}

DataViewer::~DataViewer()
{
	
}

void DataViewer::on_actionOpenProject_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, QStringLiteral("选择工程文件"),
		"", "QGIS project (*.qgz)");
	QFileInfo fi(filename);
	if (!fi.exists()) { return; }
	QDomDocument domDoc(filename);
	m_mapCanvas->readProject(domDoc);
}

void DataViewer::on_actionSaveProject_triggered()
{
	QString filename = QFileDialog::getSaveFileName(this, QStringLiteral("工程保存为"),
		"", "QGIS project (*.qgz)");
	QFile file(filename);
	file.open(QIODevice::WriteOnly);
	file.close();
	QDomDocument domDoc(filename);
	m_mapCanvas->writeProject(domDoc);
}

void DataViewer::on_actionSaveAsProject_triggered()
{

}

void DataViewer::on_actionAddVectorData_triggered()
{
	QString layerPath = QFileDialog::getOpenFileName(this, QStringLiteral("选择矢量数据"), "", "shapefile (*.shp)");
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
	m_layerList << vecLayer;
	m_mapCanvas->setLayers(m_layerList);
	m_mapCanvas->setExtent(vecLayer->extent());
	m_mapCanvas->refresh();
}

void DataViewer::on_actionAddRasterData_triggered()
{
	QString layerPath = QFileDialog::getOpenFileName(this, QStringLiteral("选择栅格数据"), "", "Image (*.img *.tif *.tiff)");
	QFileInfo fi(layerPath);
	if (!fi.exists()) { return; }
	QString layerBaseName = fi.baseName(); // 图层名称

	QgsRasterLayer* vecLayer = new QgsRasterLayer(layerPath, layerBaseName);
	if (!vecLayer) { return; }
	if (!vecLayer->isValid())
	{
		QMessageBox::information(0, "", "layer is invalid");
		return;
	}
	m_layerList << vecLayer;
	m_mapCanvas->setLayers(m_layerList);
	m_mapCanvas->setExtent(vecLayer->extent());
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




