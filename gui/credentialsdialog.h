#pragma once

#include <QDialog>

namespace Ui {
class CredentialsDialog;
}

namespace JiraWorksheet
{

class CredentialsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CredentialsDialog(QWidget *parent = 0);
	~CredentialsDialog();

	QString login() const;
	QString password() const;
	
private:
	QScopedPointer<Ui::CredentialsDialog> ui;

private slots:
	void updateButtons();

};

} // namespace
