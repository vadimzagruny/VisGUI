#include "stdafx.h"
#include "Logger.h"
#include "LoggerImpl.h"

CLogger::CLogger(CLoggerImpl *pLoggerImpl) :m_pLoggerImpl(pLoggerImpl)
{
}


CLogger::~CLogger()
{
	delete m_pLoggerImpl;
}



CFileLogger::~CFileLogger()
{
}
//CFileLogger
CFileLogger::CFileLogger(string & file_name) : CLogger(
#ifdef MT
	new CMT_LoggerImpl()
#else
	new CST_LoggerImpl()
#endif
	), file(file_name)
{ }

void CFileLogger::log(string & str)
{
	m_pLoggerImpl->file_log(file, str);
}

//ConsoleLogger
CConsoleLogger::CConsoleLogger() : CLogger(
#ifdef MT
	new CMT_LoggerImpl()
#else
	new CST_LoggerImpl()
#endif
	)
{ }

void CConsoleLogger::log(string & str)
{
	m_pLoggerImpl->console_log(str);
}


//SocketLogger
CSocketLogger::CSocketLogger(string & remote_host,
	int remote_port) : CLogger(
#ifdef MT
	new CMT_LoggerImpl()
#else
	new CST_LoggerImpl()
#endif
	), host(remote_host), port(remote_port)
{ }

void CSocketLogger::log(string & str)
{
	m_pLoggerImpl->socket_log(host, port, str);
}
