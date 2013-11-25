#include "worksheetdialog.h"

#include <QBoxLayout>
#include <QDialogButtonBox>

#include "worksheetwidget.h"

namespace JiraWorksheet
{

WorksheetDialog::WorksheetDialog(const QString& jiraHost, QWidget* parent)
	: QDialog(parent)
	, m_widget(new WorksheetWidget(jiraHost, this))
{
	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	connect(buttons, SIGNAL(accepted()), SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), SLOT(reject()));

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(m_widget);
	layout->addWidget(buttons);
	setLayout(layout);

	setWindowTitle("Worksheet");
}

time_t WorksheetDialog::total() const
{
	return m_widget->total();
}

void WorksheetDialog::setLogin(const QString& login)
{
	m_widget->setLogin(login);
}

void WorksheetDialog::setPassword(const QString& password)
{
	m_widget->setPassword(password);
}

} // namespace
