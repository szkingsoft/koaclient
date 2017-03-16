
/////////////////////////////////////////////////////////////////////////
//编写人：李开程
//编写时间：2002-12-15
//实现功能：实现时间与数值的转换，进行时间的比较等功能
/////////////////////////////////////////////////////////////////////////
#ifndef _INC_TIMEVALUE_H
#define _INC_TIMEVALUE_H


class CTimeValue
{
protected:
	unsigned int	m_uiDate;
public:
CTimeValue()
{
}

~CTimeValue()
{
}
///////////////////////////////////////////////////////////////////////////
//设置今日日期，由年月日构成，例如：20020604
///////////////////////////////////////////////////////////////////////////
void SetCurrentDate(unsigned int	uiDate)
{
	m_uiDate=uiDate;
}

unsigned int MakeMinuteTime(unsigned short	usMinutes)
{//返回由月日时分组成的数字，用于分钟K线
	unsigned int	uiValue=(m_uiDate-m_uiDate/10000*10000);
	if(usMinutes/60>23)//时间延伸到了第二天
	{	usMinutes-=(24*60);
		return	(uiValue+1)*10000+usMinutes/60*100+usMinutes%60;}
	else
		return	uiValue*10000+usMinutes/60*100+usMinutes%60;
}

bool IsSameWeek(unsigned int uiDate)
{//比较今日和所给定日期是否在同一周内
	tm	stPrevTime,stNowTime;
	memset(&stPrevTime,0,sizeof(tm));
	stPrevTime.tm_year=uiDate/10000-1900;
	stPrevTime.tm_mon=uiDate%10000/100-1;
	stPrevTime.tm_mday=uiDate%100;
	time_t	stPrev=mktime(&stPrevTime);

	memset(&stNowTime,0,sizeof(tm));
	stNowTime.tm_year=m_uiDate/10000-1900;
	stNowTime.tm_mon=m_uiDate%10000/100-1;
	stNowTime.tm_mday=m_uiDate%100;
	time_t	stNow=mktime(&stNowTime);

	if(stNow>stPrev && difftime(stNow,stPrev)>=7*24*3600)
		return	false;
	else if(stPrev>stNow && difftime(stPrev,stNow)>=7*24*3600)
		return	false;
	if(stNowTime.tm_wday<stPrevTime.tm_wday && stNow>stPrev)
		return	false;
	else if(stNowTime.tm_wday>stPrevTime.tm_wday && stNow<stPrev)
		return	false;
	return	true;
}

bool IsSameMonth(unsigned int uiDate)
{//比较今日和所给定日期是否在同一月内
	unsigned char	ucNowMonth=m_uiDate%10000/100;
	unsigned char	ucPrevMonth=uiDate%10000/100;
	if(m_uiDate/10000 != uiDate/10000)
		return	false;
	if(ucNowMonth!=ucPrevMonth)
		return	false;
	return	true;
}
};
#endif