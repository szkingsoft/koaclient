#ifndef ____OUTPUTDEFINE___H_20040427_ANN___
#define ____OUTPUTDEFINE___H_20040427_ANN___

//������Ժ�
#define ENTERLONG 265
#define EXITLONG 266
#define ENTERSHORT 267
#define EXITSHORT 268
#define RETURN 269
#define COLORRED 270
#define COLORBLUE 271
#define COLORGREEN 272
#define COLORCYAN 273
#define COLORBLACK 274
#define COLORMAGENTA 275
#define COLORBROWN 276
#define COLORLIGHTGRAY 277
#define COLORDARKGRAY 278
#define COLORLIGHTBLUE 279
#define COLORLIGHTGREEN 280
#define COLORLIGHTCYAN 281
#define COLORLIGHTRED 282
#define COLORLIGHTMAGENTA 283
#define COLORYELLOW 284
#define COLORWHITE 285
#define DEFAULTKCOLOR 286
#define DEFAULT 287
#define VOLSTICK 288
#define COLORSTICK 289
#define CIRCLEDOT 290
#define NODRAW 291

enum	FORMU_TYPE
{
	F_ZB = 0,	//ָ�����͹�ʽ
	F_TJ = 1,	//����ѡ�����͹�ʽ
	F_JY = 2,	//����ϵͳ��ʽ
	F_WC = 3,	//���K�߹�ʽ
	F_FC = 4,	//�Զ��庯����ʽ
};

enum	DATA_TYPE
{
	    K_DATA,		//�õ�K������
		M_DATA,		//�õ���������
		HQ_DATA,	//�õ���������
		FN_DATA,	//�õ���������
};

typedef	float 	FPERIOD[12];
//ע��ԭ��������ͨ��ָ������
typedef void ( *FP_FUNC)();
//�ص�����:��ȡ��Ʊ���ݵ�
typedef long(CALLBACK * FP_DATAIO)(SCodeInfo stk,
								   DATA_TYPE nDType,void * pData,short nDataNum);
#endif