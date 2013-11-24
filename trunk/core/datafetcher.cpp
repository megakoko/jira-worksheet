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

namespace JiraWorksheet
{

DataFetcher::DataFetcher(const QString &jiraHost, QObject* parent)
	: QObject(parent)
	, m_manager(new QNetworkAccessManager(this))
	, m_host(jiraHost)
{

}

void DataFetcher::fetchWorksheet(const QDate &startDate, const QDate &endDate)
{
	// Cleanup
	m_workLog.clear();

	QUrl url;
	url.setScheme("https");
	url.setHost(m_host);
	url.setPath("rest/timesheet-gadget/1.0/raw-timesheet.json");
	url.addQueryItem("userId", m_login);

	static const QLocale locale(QLocale::English, QLocale::UnitedKingdom);
	static const QString jiraDateFormat("dd/MMM/yyyy");
	url.addQueryItem("startDate", locale.toString(startDate, jiraDateFormat));
	url.addQueryItem("endDate", locale.toString(endDate, jiraDateFormat));

	qDebug() << url;

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
	if (!reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/json"))
		replyIsValid = false;
	else
	{
		replyIsValid = processJson(reply->readAll());
	}


	qDebug() << "REPLY IS VALID =" << replyIsValid;
	emit finished(replyIsValid);
}

bool DataFetcher::processJson(const QByteArray& json)
{
	bool jsonIsValid = true;


	QJson::Parser parser;
	QVariant map = parser.parse(json, &jsonIsValid);

	m_workLog = QSharedPointer<WorkLog>(new WorkLog(map.toMap()));

	return jsonIsValid;
}

} // namespace
