#include "worksheetwidget.h"

#include "../core/datafetcher.h"
#include "worksheetmodel.h"

#include "ui_worksheetwidget.h"


namespace JiraWorksheet
{

WorksheetWidget::WorksheetWidget(const QString& jiraHost, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::WorksheetWidget)
	, m_fetcher(new DataFetcher(jiraHost))
{
	ui->setupUi(this);
	ui->tableView->setModel(new WorksheetModel(ui->tableView));

	const QDate& today = QDate::currentDate();
	ui->startDate->setDate(today);
	ui->endDate->setDate(today);

	ui->warningLabel->clear();
	ui->warningText->clear();
	ui->warningLabel->setPixmap(style()->standardIcon(QStyle::SP_MessageBoxWarning).pixmap(ui->warningLabel->height()*0.7));

	connect(m_fetcher.data(), SIGNAL(finished(bool)), SLOT(finished()));
	connect(ui->startDate, SIGNAL(dateChanged(QDate)), SLOT(fetchWorksheet()));
	connect(ui->endDate, SIGNAL(dateChanged(QDate)), SLOT(fetchWorksheet()));
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

void WorksheetWidget::fetchWorksheet()
{
	fetchWorksheet(ui->startDate->date(), ui->endDate->date());
}

void WorksheetWidget::fetchWorksheet(const QDate& startDate, const QDate& endDate)
{
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
}

void WorksheetWidget::setWarning(const QString& message)
{
	ui->warningLabel->setHidden(message.isNull());
	ui->warningText->setHidden(message.isNull());
	ui->warningText->setText(message);
}

} // namespace
