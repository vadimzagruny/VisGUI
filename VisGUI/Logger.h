#pragma once

// ќпережающее объ€вление
class CLoggerImpl;

class CLogger
{
public:
	CLogger(CLoggerImpl *pLoggerImpl);
	virtual ~CLogger();

	virtual void log(string & str) = 0;

protected:

	CLoggerImpl *m_pLoggerImpl;

};


class CFileLogger : public CLogger
{
public:
	CFileLogger(string & file_name);
	~CFileLogger();
	void log(string & str);

private:
	string file;
};

class CConsoleLogger : public CLogger
{
public:
	CConsoleLogger();
	void log(string & str);
};

class CSocketLogger : public CLogger
{
public:
	CSocketLogger(string & remote_host, int remote_port);
	void log(string & str);
private:
	string host;
	int    port;
};