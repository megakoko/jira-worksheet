#include "worksheetdelegate.h"
#include "worksheetmodel.h"

namespace JiraWorksheet
{

WorksheetDelegate::WorksheetDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}

void WorksheetDelegate::paint(QPainter* painter, const QStyleOptionViewItem& ___option, const QModelIndex& index) const
{
	QStyleOptionViewItem option = ___option;

	option.displayAlignment = (Qt::AlignRight | Qt::AlignVCenter);

	if(index.data(WorksheetModel::TotalRole).toBool())
		option.font.setBold(true);

	QStyledItemDelegate::paint(painter, option, index);
}

} // namespace
