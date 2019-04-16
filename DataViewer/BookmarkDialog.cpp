#include "BookmarkDialog.h"
#include <QStringListModel>

#include "qgsmapcanvas.h"

BookMarkDialog::BookMarkDialog(QgsMapCanvas* mapCanvas = NULL)
	: m_mapCanvas(mapCanvas)
{
	ui.setupUi(this);

	mStringListModel = new QStringListModel(this);
	ui.listView->setModel(mStringListModel);

	// 连接列表更新信号和具体更新操作函数
	connect(this, SIGNAL(listUpdate()), this, SLOT(update()));
}

BookMarkDialog::~BookMarkDialog()
{
	
}

void BookMarkDialog::add(QString name, QgsRectangle rect)
{
	// 添加书签之后，发出更新书签列表信号，表示需要更新列表
	m_bookmarks.insert(name, rect);
	emit listUpdate();
}

void BookMarkDialog::on_zoomToButton_clicked()
{
	// 获取列表选中的书签名称，以此找到对应的地图视图范围矩形，并跳转到该范围
	QModelIndexList selIndexList = ui.listView->selectionModel()->selectedIndexes();
	if (selIndexList.count() <= 0) return;
	QgsRectangle rect = m_bookmarks[mStringListModel->data(selIndexList[0]).toString()];
	m_mapCanvas->setExtent(rect, false);
	m_mapCanvas->refresh();
}

void BookMarkDialog::on_deleteButton_clicked()
{
	// 获取列表选中的书签名称，并删掉该选中项
	QModelIndexList selIndexList = ui.listView->selectionModel()->selectedIndexes();
	if (selIndexList.count() <= 0) return;
	m_bookmarks.remove(mStringListModel->data(selIndexList[0]).toString());
	
	emit listUpdate();
}

void BookMarkDialog::update()
{
	mStringListModel->setStringList(m_bookmarks.keys());
}

