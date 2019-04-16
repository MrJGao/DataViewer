#pragma once
#include <qdialog.h>
#include "ui_bookmark.h"
#include <QListView>
#include <QStringListModel>

class QgsMapCanvas;
class QgsRectangle;

// 书签管理器类
class BookMarkDialog : public QDialog
{
	Q_OBJECT
public:
	BookMarkDialog(QgsMapCanvas* mapCanvas);
	~BookMarkDialog();

	// 添加书签
	void add(QString name, QgsRectangle rect);

signals:
	// 列表更新信号
	void listUpdate();

protected slots:
	// 更新书签列表
	void update();
	
	void on_zoomToButton_clicked();
	void on_deleteButton_clicked();

private:
	Ui::BookMarkDialog ui;

	QgsMapCanvas* m_mapCanvas = NULL;
	// 书签存储数据结构。用QMap存储，将书签名称和对应的地图视图矩形相关联
	QMap<QString, QgsRectangle> m_bookmarks;
	// 书签列表的数据模型
	QStringListModel* mStringListModel = NULL;
};

