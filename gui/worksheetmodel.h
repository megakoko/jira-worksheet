#pragma once

#include <QAbstractTableModel>
#include "../core/worklog.h"

namespace JiraWorksheet
{

class WorksheetModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	explicit WorksheetModel(QObject *parent = 0);

	void setWorkLog(const QSharedPointer<WorkLog>& workLog);

	// Reimp
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	QVariant data(const QModelIndex &index, int role) const;

private:
	QSharedPointer<WorkLog> m_data;

};

} // namespace
