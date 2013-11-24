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

	connect(m_fetcher.data(), SIGNAL(finished(bool)), SLOT(finished()));
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

void WorksheetWidget::fetchWorksheet(const QDate& startDate, const QDate& endDate)
{
	m_fetcher->fetchWorksheet(startDate, endDate);
}

void WorksheetWidget::finished()
{
	if(WorksheetModel* model = qobject_cast<WorksheetModel*>(ui->tableView->model()))
		model->setWorkLog(m_fetcher->workLog());
}

} // namespace
