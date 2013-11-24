#include "datafetcher.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QUrl>
#include <QLocale>
#include <QDebug>
#include <QDateTime>

#include <QJson/Parser>

#include "worklog.h"
#include "credentialsprovider.h"

namespace JiraWorksheet
{

DataFetcher::DataFetcher(const QString &jiraHost, QObject* parent)
	: QObject(parent)
	, m_manager(new QNetworkAccessManager(this))
	, m_host(jiraHost)
	, m_credentialsProvider(NULL)
{

}

void DataFetcher::fetchWorksheet(const QDate &startDate, const QDate &endDate)
{
	// Cleanup
	m_workLog.clear();
	m_lastError.clear();

	if(m_credentialsProvider != NULL && (m_login.isNull() || m_password.isNull()))
	{
		QString newLogin, newPassword;
		if(m_credentialsProvider->getCredentials(&newLogin, &newPassword))
		{
			m_login = newLogin;
			m_password = newPassword;
		}
	}

	QUrl url;
	url.setScheme("https");
	url.setHost(m_host);
	url.setPath("rest/timesheet-gadget/1.0/raw-timesheet.json");
	url.addQueryItem("userId", m_login);

	static const QLocale locale(QLocale::English, QLocale::UnitedKingdom);
	static const QString jiraDateFormat("dd/MMM/yyyy");
	url.addQueryItem("startDate", locale.toString(startDate, jiraDateFormat));
	url.addQueryItem("endDate", locale.toString(endDate, jiraDateFormat));

	const QByteArray authorization = "Basic " + QString("%1:%2").arg(m_login).arg(m_password).toUtf8().toBase64();


	QNetworkRequest req;
	req.setUrl(url);
	req.setRawHeader("Authorization", authorization);
	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	QNetworkReply* reply = m_manager->get(req);
	connect(reply, SIGNAL(finished()), SLOT(processReply()));
}


void DataFetcher::processReply()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply == NULL)
		return;

	reply->deleteLater();


	bool replyIsValid = true;
	if(reply->error() != QNetworkReply::NoError)
	{
		replyIsValid = false;
		m_lastError = reply->errorString();

		// If we couldn't pass authentication...
		if(reply->error() == QNetworkReply::AuthenticationRequiredError)
		{
			m_login.clear();
			m_password.clear();
		}
	}
	else if(!reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/json"))
	{
		replyIsValid = false;
		m_lastError = "Invalid response received from Jira. User login and password might be incorrect.";
	}
	else
	{
		QJson::Parser parser;
		QVariant map = parser.parse(reply->readAll(), &replyIsValid);

		m_workLog = QSharedPointer<WorkLog>(new WorkLog(map.toMap()));

		if(!replyIsValid)
			m_lastError = "Invalid JSON received.";
	}

	Q_ASSERT(replyIsValid || !m_lastError.isNull());
	if(!replyIsValid && m_lastError.isNull())
		m_lastError = "Unknown error";

	emit finished(replyIsValid);
}

} // namespace
