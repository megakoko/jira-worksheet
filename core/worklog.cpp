#include "worklog.h"

#include <QVariant>
#include <QDebug>

namespace JiraWorksheet
{

Entry::Entry(const QString& comment, const qlonglong& timeSpent,
			 const qlonglong& timeUpdated, const qlonglong& timeCreated,
			 const QString& authorUpdated, const QString& authorCreated)
	: comment(comment)
	, timeSpent(timeSpent)
	, timeUpdated(QDateTime::fromMSecsSinceEpoch(timeUpdated))
	, timeCreated(QDateTime::fromMSecsSinceEpoch(timeCreated))
	, authorUpdated(authorUpdated)
	, authorCreated(authorCreated)
{
}

QString Entry::formatTimeSpent(const qlonglong timeSpent)
{
	const qlonglong mins = (timeSpent / 60) % 60;
	const qlonglong hrs = timeSpent / 3600;

	QString str = QString("%1m").arg(mins);
	if (hrs > 0)
		str.prepend(QString("%1h ").arg(hrs));

	return str;
}

QDebug operator<<(QDebug dbg, const Entry& entry)
{
	dbg.nospace() << "  Timesheet entry: " << qPrintable(Entry::formatTimeSpent(entry.timeSpent)) << " - " << qPrintable(entry.comment);
	return dbg.maybeSpace();
}

Issue::Issue(const QString &key, const QString &summary, const QList<QSharedPointer<Entry> > &entries)
	: key(key)
	, summary(summary)
	, entries(entries)
{
}

QDebug operator<<(QDebug dbg, const Issue& issue)
{
	dbg.nospace() << " Issue " << qPrintable(issue.key) << " - " << qPrintable(issue.summary) << "\n";
	foreach(const QSharedPointer<Entry>& e, issue.entries)
		dbg.nospace() << *e << "\n";

	return dbg.maybeSpace();
}

WorkLog::WorkLog(const QVariantMap &map)
	: startDate(QDateTime::fromMSecsSinceEpoch(map.value("startDate").toLongLong()).date())
	, endDate(QDateTime::fromMSecsSinceEpoch(map.value("endDate").toLongLong()).date().addDays(-1))
	, issues(getIssues(map))
{
}

QList<QSharedPointer<JiraWorksheet::Issue> > WorkLog::getIssues(const QVariantMap& workLogMap)
{
	QList<QSharedPointer<Issue> > issues;
	foreach(const QVariant& issueVariant, workLogMap.value("worklog").toList())
	{
		const QVariantMap& issue = issueVariant.toMap();

		QList<QSharedPointer<Entry> > entries;
		foreach(const QVariant& entryVariant, issue.value("entries").toList())
		{
			const QVariantMap& entry = entryVariant.toMap();

			entries << QSharedPointer<Entry>(new Entry(entry.value("comment").toString(),
													   entry.value("timeSpent").toLongLong(),
													   entry.value("updated").toLongLong(),
													   entry.value("created").toLongLong(),
													   entry.value("updateAuthor").toString(),
													   entry.value("author").toString()));
		}

		issues << QSharedPointer<Issue>(new Issue(issue.value("key").toString(),
												  issue.value("summary").toString(),
												  entries));
	}

	return issues;
}

QDebug operator<<(QDebug dbg, const WorkLog& log)
{
	dbg.nospace() << "Worklog from " << qPrintable(log.startDate.toString()) << " to " << qPrintable(log.endDate.toString()) << "\n";
	foreach(const QSharedPointer<Issue>& i, log.issues)
		dbg.nospace() << *i;

	return dbg.maybeSpace();
}

} // namespace
