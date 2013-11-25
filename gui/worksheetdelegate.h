#pragma once

#include <QStyledItemDelegate>

namespace JiraWorksheet
{

class WorksheetDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	explicit WorksheetDelegate(QObject *parent = NULL);
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	
};

} // namespace
