#include "worksheetwidget.h"

#include <QTimer>

#include "../core/datafetcher.h"
#include "worksheetmodel.h"
#include "credentialsdialog.h"
#include "worksheetdelegate.h"

#include "ui_worksheetwidget.h"


namespace JiraWorksheet
{

WorksheetWidget::WorksheetWidget(const QString& jiraHost, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::WorksheetWidget)
	, m_fetcher(new DataFetcher(jiraHost, this))
	, m_fetchTimer(new QTimer(this))
{
	ui->setupUi(this);
	ui->tableView->setModel(new WorksheetModel(ui->tableView));
	ui->tableView->setItemDelegate(new WorksheetDelegate(this));

	const QDate& today = QDate::currentDate();
	ui->startDate->setDate(today);
	ui->endDate->setDate(today);

	ui->totalTimeLabel->clear();
	ui->warningContainer->hide();
	ui->warningLabel->clear();
	ui->warningText->clear();
	ui->warningLabel->setPixmap(style()->standardIcon(QStyle::SP_MessageBoxWarning).pixmap(ui->warningLabel->height()*0.7));

	m_fetcher->setCredentialsProvider(this);

	m_fetchTimer->setInterval(500);

	connect(m_fetcher, SIGNAL(finished(bool)), SLOT(finished()));
	connect(ui->startDate, SIGNAL(dateChanged(QDate)), m_fetchTimer, SLOT(start()));
	connect(ui->endDate, SIGNAL(dateChanged(QDate)), m_fetchTimer, SLOT(start()));
	connect(m_fetchTimer, SIGNAL(timeout()), SLOT(fetchWorksheet()));
}

WorksheetWidget::~WorksheetWidget()
{
}

void WorksheetWidget::setLogin(const QString& login)
{
	m_fetcher->setLogin(login);
}

void WorksheetWidget::setPassword(const QString& password)
{
	m_fetcher->setPassword(password);
}

bool WorksheetWidget::getCredentials(QString *login, QString *password)
{
	bool result = false;

	if(login != NULL && password != NULL)
	{
		CredentialsDialog d(this);
		if(d.exec() == QDialog::Accepted)
		{
			result = true;
			*login = d.login();
			*password = d.password();
		}
	}

	return result;
}

void WorksheetWidget::fetchWorksheet()
{
	if(sender() == m_fetchTimer)
		m_fetchTimer->stop();

	fetchWorksheet(ui->startDate->date(), ui->endDate->date());
}

void WorksheetWidget::fetchWorksheet(const QDate& startDate, const QDate& endDate)
{
	disconnect(ui->startDate, SIGNAL(dateChanged(QDate)), m_fetchTimer, SLOT(start()));
	disconnect(ui->endDate, SIGNAL(dateChanged(QDate)), m_fetchTimer, SLOT(start()));

	ui->startDate->setDate(startDate);
	ui->endDate->setDate(endDate);

	connect(ui->startDate, SIGNAL(dateChanged(QDate)), m_fetchTimer, SLOT(start()));
	connect(ui->endDate, SIGNAL(dateChanged(QDate)), m_fetchTimer, SLOT(start()));



	m_fetcher->fetchWorksheet(startDate, endDate);
	if(startDate > endDate)
		setWarning("Start date is greater than end date");
	else
		setWarning(QString::null);
}

void WorksheetWidget::finished()
{
	if(WorksheetModel* model = qobject_cast<WorksheetModel*>(ui->tableView->model()))
		model->setWorkLog(m_fetcher->workLog());

	if(!m_fetcher->lastError().isNull())
		setWarning(m_fetcher->lastError());

	ui->totalTimeLabel->setText("Total time spent: " + Entry::formatTimeSpent(total()));
}

void WorksheetWidget::setWarning(const QString& message)
{
	ui->warningContainer->setHidden(message.isNull());
	ui->warningText->setText(message);
}

time_t WorksheetWidget::total() const
{
	if(QSharedPointer<WorkLog> worklog = m_fetcher->workLog())
		return worklog->total();

	return 0;
}

} // namespace
