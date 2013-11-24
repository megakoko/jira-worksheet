#include "worksheetmodel.h"

namespace JiraWorksheet
{

WorksheetModel::WorksheetModel(QObject* parent)
	: QAbstractTableModel(parent)
{
}

void WorksheetModel::setWorkLog(const QSharedPointer<WorkLog>& workLog)
{
	beginResetModel();
	m_data = workLog;
	endResetModel();
}

int WorksheetModel::rowCount(const QModelIndex&) const
{
	return m_data.isNull() ? 0 : m_data->issues.size();
}

int WorksheetModel::columnCount(const QModelIndex&) const
{
	return m_data.isNull() ? 0 : (m_data->startDate.daysTo(m_data->endDate) + 1);
}

QVariant WorksheetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(m_data.isNull())
		return QVariant();

	switch(role)
	{
	case Qt::DisplayRole:
		if(orientation == Qt::Horizontal)
			return m_data->startDate.addDays(section);
		else
			return m_data->issues.at(section)->key;
		break;

	case Qt::ToolTipRole:
		if(orientation == Qt::Vertical)
			return m_data->issues.at(section)->summary;
		break;

	}

	return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant WorksheetModel::data(const QModelIndex& index, int role) const
{
	if(m_data.isNull() || !index.isValid())
		return QVariant();

	switch(role)
	{
	case Qt::DisplayRole: {
		const QSharedPointer<Issue> issue = m_data->issues.at(index.row());

		const QDate& date = m_data->startDate.addDays(index.column());

		qlonglong timeSpent = 0;
		foreach(const QSharedPointer<Entry> entry, issue->entries)
		{
			if(entry->timeUpdated.date() == date)
				timeSpent += entry->timeSpent;
		}

		if(timeSpent == 0)
			return QVariant();
		else
			return Entry::formatTimeSpent(timeSpent);
	} break;

	case Qt::TextAlignmentRole:
		return QVariant(Qt::AlignRight | Qt::AlignVCenter);

	}

	return QVariant();
}

}
