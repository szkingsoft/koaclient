
/////////////////////////////////////////////////////////////////////////
//��д�ˣ����
//��дʱ�䣺2002-12-15
//ʵ�ֹ��ܣ�ʵ��ʱ������ֵ��ת��������ʱ��ıȽϵȹ���
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
//���ý������ڣ��������չ��ɣ����磺20020604
///////////////////////////////////////////////////////////////////////////
void SetCurrentDate(unsigned int	uiDate)
{
	m_uiDate=uiDate;
}

unsigned int MakeMinuteTime(unsigned short	usMinutes)
{//����������ʱ����ɵ����֣����ڷ���K��
	unsigned int	uiValue=(m_uiDate-m_uiDate/10000*10000);
	if(usMinutes/60>23)//ʱ�����쵽�˵ڶ���
	{	usMinutes-=(24*60);
		return	(uiValue+1)*10000+usMinutes/60*100+usMinutes%60;}
	else
		return	uiValue*10000+usMinutes/60*100+usMinutes%60;
}

bool IsSameWeek(unsigned int uiDate)
{//�ȽϽ��պ������������Ƿ���ͬһ����
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
{//�ȽϽ��պ������������Ƿ���ͬһ����
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