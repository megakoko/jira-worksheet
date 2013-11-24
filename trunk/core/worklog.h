#pragma once

#include "core_global.h"

#include <QString>
#include <QDateTime>

#include "timetype.h"

template<class K, class T> class QMap;
typedef QMap<QString, QVariant> QVariantMap;
template<class T> class QList;


namespace JiraWorksheet
{

struct CORESHARED_EXPORT Entry
{
public:
	Entry(const QString& comment, const time_t& timeSpent,
		  const time_t& timeUpdated, const time_t& timeCreated,
		  const QString& authorUpdated, const QString& authorCreated);

	static QString formatTimeSpent(const time_t timeSpent);

	const QString comment;
	const time_t timeSpent;

	const QDateTime timeUpdated;
	const QDateTime timeCreated;

	const QString authorUpdated;
	const QString authorCreated;
};

QDebug operator<<(QDebug dbg, const Entry& entry);


struct CORESHARED_EXPORT Issue
{
public:
	Issue(const QString& key, const QString& summary, const QList<QSharedPointer<Entry> >& entries);

	const QString key;
	const QString summary;
	const QList<QSharedPointer<Entry> > entries;
};

QDebug operator<<(QDebug dbg, const Issue& issue);



struct CORESHARED_EXPORT WorkLog
{
public:
	WorkLog(const QVariantMap& map);

	const QDate startDate;
	const QDate endDate;
	const QList<QSharedPointer<Issue> > issues;

	time_t total();

private:
	static QList<QSharedPointer<Issue> > getIssues(const QVariantMap& workLogMap);
	time_t totalTime;
};

QDebug operator<<(QDebug dbg, const WorkLog& log);

} // namespace
