////////////////////////////////////////////////////////////////
//文件名称：TimeOut.cpp
//功能描述：时间接口。
//版本说明：CTimeOut API
//			
//编写作者：2018.03.30 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////

#include "TimeOut.h"

//////////////////////////////////////////////////////////////////////
// CTimeOut
//////////////////////////////////////////////////////////////////////
// 带时间积累的定时时钟
template<int I>
bool CTimeOut_<I>::ToNextTick(int nSecs)
{
	if(IsTimeOut(nSecs))
	{
		if(time(NULL) >= m_tUpdateTime + nSecs*2)
			return Update(),true;
		else
			return (m_tUpdateTime+=nSecs),true;
	}
	else
		return false;
}

//////////////////////////////////////////////////////////////////////
// CTimeOutMS
//////////////////////////////////////////////////////////////////////
// 带时间积累的定时时钟
template<int I>
bool CTimeOutMS_<I>::ToNextTick(int nMilliSecs)
{
	if(IsTimeOut(nMilliSecs))
	{
		if((clock()/1000) >= m_tUpdateTime + nMilliSecs*2)
			return Update(),true;
		else
			return (m_tUpdateTime+=nMilliSecs),true;
	}
	else
		return false;
}

template<int I>
int CTimeOutMS_<I>::GetRemain()
{
	if(m_nInterval < 0) 
		return m_nInterval; 
	else 
		return m_tUpdateTime ? ::CutRange(m_nInterval - (int)(clock()/1000-(int)m_tUpdateTime), 0, m_nInterval) : 0; 
}
