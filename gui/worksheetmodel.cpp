#include "worksheetmodel.h"

#include <QStringList>
#include <QFont>

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
	return m_data.isNull() ? 0 : (m_data->issues.size() + 1);
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
		{
			if(section == (rowCount() - 1))
				return "TOTAL";
			else if(const QSharedPointer<Issue> issue = m_data->issues.value(section))
				return issue->key;
		}
		break;

	case Qt::ToolTipRole:
		if(orientation == Qt::Vertical)
			if(const QSharedPointer<Issue> issue = m_data->issues.value(section))
				return issue->summary;
		break;

	case Qt::FontRole:
		if(orientation == Qt::Vertical && section == (rowCount()-1))
		{
			QFont font = QAbstractTableModel::headerData(section, orientation, role).value<QFont>();
			font.setBold(true);
			return font;
		}
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
		if(index.row() == (rowCount() - 1))
		{
			const QDate& date = m_data->startDate.addDays(index.column());

			time_t timeSpentInDay = 0;
			foreach(const QSharedPointer<Issue> issue, m_data->issues)
				foreach(const QSharedPointer<Entry> entry, issue->entries)
				{
					if(entry->timeUpdated.date() == date)
						timeSpentInDay += entry->timeSpent;
				}

			if(timeSpentInDay != 0)
				return Entry::formatTimeSpent(timeSpentInDay);
		}
		else if(const QSharedPointer<Issue> issue = m_data->issues.value(index.row()))
		{
			const QDate& date = m_data->startDate.addDays(index.column());

			time_t timeSpent = 0;
			foreach(const QSharedPointer<Entry> entry, issue->entries)
			{
				if(entry->timeUpdated.date() == date)
					timeSpent += entry->timeSpent;
			}

			if(timeSpent != 0)
				return Entry::formatTimeSpent(timeSpent);
		}

	} break;

	case Qt::ToolTipRole:
		if(const QSharedPointer<Issue> issue = m_data->issues.value(index.row()))
		{
			const QDate& date = m_data->startDate.addDays(index.column());

			QStringList comments;
			foreach(const QSharedPointer<Entry> entry, issue->entries)
			{
				if(entry->timeUpdated.date() == date)
					comments << (QChar(0x2022) + ' ' + entry->comment);
			}

			if(!comments.isEmpty())
				return comments.join("\n");
		}
		break;

	case TotalRole:
		return index.row() == (rowCount() - 1);

	}

	return QVariant();
}

}
