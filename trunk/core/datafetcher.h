#pragma once

#include <QObject>

#include "core_global.h"

class QNetworkAccessManager;
class QNetworkReply;
class QDate;

template<class T> class QSharedPointer;

#include "worklog.h"

namespace JiraWorksheet
{

struct WorkLog;

class CredentialsProvider;

class CORESHARED_EXPORT DataFetcher : public QObject
{
	Q_OBJECT
public:
	DataFetcher(const QString& jiraHost, QObject* parent = NULL);

	void setLogin(const QString& login) { m_login = login; }
	void setPassword(const QString& password) { m_password = password; }

	void fetchWorksheet(const QDate& startDate, const QDate& endDate);

	QSharedPointer<WorkLog> workLog() const { return m_workLog; }

	QString lastError() const { return m_lastError; }

	// DataFetcher does not take ownership of the object.
	void setCredentialsProvider(CredentialsProvider* provider) { m_credentialsProvider = provider; }

private:
	QNetworkAccessManager* m_manager;

	QString m_login;
	QString m_password;

	QString m_host;

	QSharedPointer<WorkLog> m_workLog;

	QString m_lastError;

	CredentialsProvider* m_credentialsProvider;

private slots:
	void processReply();

signals:
	void finished(bool ok);

};

} // namespace
