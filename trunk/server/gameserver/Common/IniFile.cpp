//////////////////////////////////////////////////////////////////////
//文件名称：IniFile.cpp
//功能描述：基础包类
//版本说明：IniFile API
//			
//编写作者：2017.09.06 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "IniFile.h"
#include "Common.h"
#include <stdlib.h>
#include <strings.h>

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////



CIniFile::CIniFile(const char * pFilename, const char * pSection)
{
	if(pFilename && strlen(pFilename) < INIWORDSIZE)
	{
		SafeCopy(m_bufFilename, pFilename, INIWORDSIZE);
	}
	else
	{
		m_bufFilename[0] = 0;
		printf("CIniFile::CIniFile() parameter error_1.\n");
	}

	if(pSection && strlen(pSection) < INIWORDSIZE)
	{
		SafeCopy(m_bufSection, pSection,INIWORDSIZE);
	}
	else
	{
		m_bufSection[0] = 0;
		printf("CIniFile::CIniFile() parameter error_2.\n");
	}
	//printf("CIniFile::GetString() parameter 1. m_bufFilename=[%s]\tm_bufSection=[%s]\n",m_bufFilename, m_bufSection);
}

CIniFile::~CIniFile()
{
}

void	CIniFile::SetSection(const char * pSect)
{
	if(pSect && strlen(pSect) < INIWORDSIZE)
	{
		SafeCopy(m_bufSection, pSect, INIWORDSIZE);
	}
	else
	{
		m_bufSection[0] = 0;
		printf("CIniFile::SetSection()  parameter error_3.\n");
	}
}

bool	CIniFile::GetString(char * bufString, const char * pKey, int nBufSize)
{
	if(bufString)
		bufString[0] = 0;
	else
	{
		printf("CIniFile::GetString() parameter error_4.\n");
		return false;
	}

	//printf("CIniFile::GetString() parameter 2.\n");

	int	nSectLen = strlen(m_bufSection);
	assert(nSectLen < INIWORDSIZE);
	int	nKeyLen = strlen(pKey);
	assert(nKeyLen < INIWORDSIZE);
	FILE * pFile = fopen(m_bufFilename, "r");
	//extern int errno;
	//printf("m_bufFilename=[%s]\tm_bufSection=[%s]\tpKey=[%s]\tnSectLen=[%d]\tnKeyLen=[%d]\terrno =[%d]\tres=[%s]\n", m_bufFilename, m_bufSection, pKey, nSectLen, nKeyLen, errno,strerror(errno));
	assert(pFile);
	if(nSectLen && nKeyLen && pFile)
	{
		//printf("CIniFile::GetString() parameter 3.\n");
		char buf[INIWORDSIZE+INIDATASIZE];
		do
		{
			fgets(buf, INIWORDSIZE+INIDATASIZE-1, pFile);
			buf[INIWORDSIZE+INIDATASIZE-1] = 0;
			//LOGDEBUG("CIniFile::GetString() parameter error. buf=[%s] m_bufSection=[%s] nSectLen=[%d] pKey=[%s]\n", buf, m_bufSection, nSectLen, pKey);
			if(buf[0] == '[' && buf[nSectLen+1] == ']' && strncasecmp(buf+1, m_bufSection, nSectLen) == 0)//windows:strnicmp -> linux:strncasecmp
			{
				//printf("CIniFile::GetString() parameter 4.\n");
				do
				{
					//printf("CIniFile::GetString() parameter 5.\n");
					fgets(buf, INIWORDSIZE+INIDATASIZE-1, pFile);
					buf[INIWORDSIZE+INIDATASIZE-1] = 0;
					if(buf[0] && buf[0] != ';' 
							&& (buf[nKeyLen] == '=' || buf[nKeyLen] == ' ' || buf[nKeyLen] == '\t')
							&& strncasecmp(buf, pKey, nKeyLen) == 0)
					{
						//printf("CIniFile::GetString() parameter 6.\n");
						int		pos = nKeyLen;
						while(buf[pos] == ' ' || buf[pos] == '\t')		// 去掉前空
							pos++;
						if(buf[pos] == '=')		// 去掉'='
							pos++;
						while(buf[pos] == ' ' || buf[pos] == '\t')		// 去掉前空
							pos++;
						strncpy(bufString, buf + pos, nBufSize-1);
						bufString[nBufSize-1] = 0;
						if(bufString[0] && bufString[strlen(bufString)-1] == '\n')		// fgets 会多取一个 '\r\n' 字符
							bufString[strlen(bufString)-1] = 0;
						while(bufString[0] && (bufString[strlen(bufString)-1] == ' '
												|| bufString[strlen(bufString)-1] == '\t'		// 去掉尾空
												|| bufString[strlen(bufString)-1] == '\r'))		// 去掉\r
							bufString[strlen(bufString)-1] = 0;
						if(pFile)
						{
							fclose(pFile);
						}

						//printf("len=%d\tpKey=%s\tbufString=%s\n",strlen(bufString), pKey, bufString);
						return true;
					}
				}while(!feof(pFile) && buf[0] != '[');
			}
			else
			{
				//LOGCATCH("CIniFile::GetString() parameter error. buf=[%s] m_bufSection=[%s] nSectLen=[%d] pKey=[%s]\n", buf, m_bufSection, nSectLen, pKey);
			}
		}while(!feof(pFile));
	}
	if(pFile)
	{
		fclose(pFile);
	}
	return false;
}

int		CIniFile::GetInt(const char * pKey)
{
	char buf[INIWORDSIZE];		//??
	if(GetString(buf, pKey, INIWORDSIZE))
		return atoi(buf);
	else
		return 0;
}


