
/////////////////////////////////////////////////////////////////////////
//ʵ�ֹ��ܣ������˳���������Ҫʹ�õĸ��ֽṹ��
//
//���м۸�Ҫ����1000
//���гɽ�������ԪΪ��λ
/////////////////////////////////////////////////////////////////////////
#ifndef INC_STRUCT_H
#define INC_STRUCT_H
#include "quotestruct.h"
///////////////////////////////////////////////////////////////
#pragma pack(1)
struct SHistoryUnit
{
	unsigned int		m_uiDate;
	float	m_fOpenPrice;
	float	m_fMaxPrice;
	float	m_fMinPrice;
	float	m_fClosePrice;
	unsigned	int		m_uiVolume;
	unsigned	int		m_uiSum;//�ɽ����
};//�����ڴ���ʷ����


//����֤ȯ�ɽ����ݣ����ڻ��۸��ߺͳɽ�����
struct	SDynamicMinsUnit//�Է���Ϊ��λ����
{
	unsigned int		m_uiTime;//�����һ�ο���ʱ��
	float	m_fNewPrice;
};
#pragma pack()
#endif