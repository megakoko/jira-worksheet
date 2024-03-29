#pragma once

#include <QDialog>
#include "../core/timetype.h"
#include "gui_global.h"

namespace JiraWorksheet
{

class WorksheetWidget;

class GUISHARED_EXPORT WorksheetDialog : public QDialog
{
	Q_OBJECT
public:
	explicit WorksheetDialog(const QString& jiraHost, QWidget *parent = 0);
	void setLogin(const QString& login);
	void setPassword(const QString& password);
	time_t total() const;

private:
	WorksheetWidget* m_widget;

};

} // namespace
