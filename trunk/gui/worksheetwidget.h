#pragma once

#include <QWidget>
#include <QDate>
#include "gui_global.h"
#include "../core/credentialsprovider.h"
#include "../core/timetype.h"

namespace Ui {
class WorksheetWidget;
}

namespace JiraWorksheet
{

class DataFetcher;

class GUISHARED_EXPORT WorksheetWidget : public QWidget, public CredentialsProvider
{
	Q_OBJECT
	
public:
	explicit WorksheetWidget(const QString& jiraHost, QWidget *parent = 0);
	~WorksheetWidget();

	void setLogin(const QString& login);
	void setPassword(const QString& password);

	// Reimp
	bool getCredentials(QString *login, QString *password);

	time_t total() const;

public slots:
	void fetchWorksheet();
	void fetchWorksheet(const QDate& startDate, const QDate& endDate);
	
private:
	void setWarning(const QString& message);

	QScopedPointer<Ui::WorksheetWidget> ui;

	QScopedPointer<DataFetcher> m_fetcher;

private slots:
	void finished();

};

} // namespace
