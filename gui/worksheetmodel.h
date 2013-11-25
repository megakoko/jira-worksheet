#pragma once

#include <QAbstractTableModel>
#include "../core/worklog.h"

namespace JiraWorksheet
{

class WorksheetModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	enum ExtraRoles
	{
		TotalRole = Qt::UserRole
	};

	explicit WorksheetModel(QObject *parent = 0);

	void setWorkLog(const QSharedPointer<WorkLog>& workLog);

	// Reimp
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	QVariant data(const QModelIndex &index, int role) const;

private:
	QSharedPointer<WorkLog> m_data;

};

} // namespace
