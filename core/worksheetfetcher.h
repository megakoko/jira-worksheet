#pragma once

#include <QObject>

#include "core_global.h"

class QNetworkAccessManager;
class QNetworkReply;
class QDate;

class CORESHARED_EXPORT WorksheetFetcher : public QObject
{
	Q_OBJECT
public:
	WorksheetFetcher(const QString& jiraHost, QObject* parent = NULL);

	void setLogin(const QString& login) { m_login = login; }
	void setPassword(const QString& password) { m_password = password; }

	void fetchWorksheet(const QDate& startDate, const QDate& endDate);

private:
	bool processJson(const QByteArray& json);

	QNetworkAccessManager* m_manager;

	QString m_login;
	QString m_password;

	QString m_host;

private slots:
	void processReply();

signals:
	void finished(bool ok);

};

