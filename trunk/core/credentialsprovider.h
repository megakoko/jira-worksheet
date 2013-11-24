#pragma once

class QString;

namespace JiraWorksheet
{

class CredentialsProvider
{
public:
	virtual ~CredentialsProvider() { }
	virtual bool getCredentials(QString* login, QString* password) = 0;
};

} // namespace
