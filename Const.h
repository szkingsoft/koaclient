/////////////////////////////////////////////////////////////////////////
//ʵ�ֹ��ܣ��������������Ҫ�ĳ���
//////////////////////////////////////////////////////////////////////////
#ifndef INC_CONST_H
#define INC_CONST_H
#include "quoteconst.h"

#define MENUCOLUMN_MAXNUM 18 //�˵����������
// �Ҽ��˵���װ������
const  unsigned int POPSUBMENU_INDEX_PRICEVOLOBJ = 3;   // ��ʱ����
const  unsigned int POPSUBMENU_INDEX_HISTECHOBJ = 0;    //��������
const  unsigned int POPSUBMENU_INDEX_TECHINDEX = 1;     //����ָ��
const  unsigned int POPSUBMENU_INDEX_REPORTTABLE = 2;   //���ۣ����У���

const   unsigned char   PRICE_NORMAL = 0; // ����

////////////////////////////////////////////////////////////////////////
//�������Ͷ���
const   unsigned short PT_NONE       =0x0001;//������
const unsigned int PRICEREPORT= 1;	//�����б�
const unsigned int PRICEDETAIL = 2; //������Ϣ



//���ɷ�����ͼ������(���Obj����)
const   unsigned int RealObj          =0x0010;  //ʵʱ����Obj
const   unsigned int TechObj          =0x0020;  //��ʷ����Obj
//RealObj��ģ��Pic����
const   unsigned int PRICEVOLPIC      =0x0000;  //ֻ�гɽ��ۡ��ɽ���ͼ��

////////////////////////////////////////////////////////////////////////
//�Զ���
////////////////////////////////////////////////////////////////////////
const int INITVIEWINFO     = 0;//��ʼ����ͼ��Ϣ
const int CURRENTSTOCKCODE = 2;//��ǰ֤ȯ����
const int INITBLKCONST     = 4;//������ƺͼ�ƶ��ձ�



///////////////////////////////////////////////////////////////////////

const int QUOTELINKSUCCESS	 = 1;
const int QUOTELINKFAIL		 = 2;
const int QUOTELINKDOWN		 = 3;
const int QUOTELOGINSUCCESS  = 4;
const int QUOTELOGINFALIED   = 5;
const int QUOTEOTHERERROR	 = 6;
const int QUOTEOTHERSUCCESS  = 7;
const int TRADELINKSUCCESS	 = 8;
const int TRADELINKFAILED	 = 9;
const int TRADELINKDOWN		 = 10;
const int TRADELOGINSUCCESS  = 11;
const int TRADELOGINFAILED   = 12;



//////////////////////////////////////////////////////////////////////////
//�����������
//////////////////////////////////////////////////////////////////////////
const int INDEX_MAX = 99999;   //ҵ���������ֵ

const	BYTE	INVALIDF[8] = {0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
#endif