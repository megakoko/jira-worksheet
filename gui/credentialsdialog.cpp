#include "credentialsdialog.h"
#include "ui_credentialsdialog.h"

#include <QPushButton>

namespace JiraWorksheet
{

CredentialsDialog::CredentialsDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::CredentialsDialog)
{
	ui->setupUi(this);

	connect(ui->login, SIGNAL(textChanged(QString)), SLOT(updateButtons()));
	connect(ui->password, SIGNAL(textChanged(QString)), SLOT(updateButtons()));

	updateButtons();
}

CredentialsDialog::~CredentialsDialog()
{
}

QString CredentialsDialog::login() const
{
	return ui->login->text().isEmpty() ? QString::null : ui->login->text();
}

QString CredentialsDialog::password() const
{
	return ui->password->text().isEmpty() ? QString::null : ui->password->text();
}

void CredentialsDialog::updateButtons()
{
	if(QPushButton* okBtn = ui->buttonBox->button(QDialogButtonBox::Ok))
		okBtn->setDisabled(login().isNull() || password().isNull());
}

} // namespace
