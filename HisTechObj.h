// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_HISTECHOBJ_409F3AB302ED_INCLUDED
#define _INC_HISTECHOBJ_409F3AB302ED_INCLUDED

#include "BaseDrawObj.h"
#include "techcreator.h"
#include "InfoTable.h"
#include "distractstattable.h"

#define	TP_FIRST	1    //������ʷ�����������͵�һ��
#define	TP_NEXT		2    //������ʷ�����������ͳ���һ��֮�������
#define	TP_END		3    //�Ѿ�������������ʷ����

//Function:������������
//Author:  gongxiaofeng
class CHisTechObj: public CBaseDrawObj
{
public:
	void SetDrawLineCallBcakFun();
	void SetCurSymbol(CString strSymbolCode);
public:	
	enum REQUESTDATATYPE
	{
		NORMALREQUEST,//��������
		LEFTMOVEREQUEST,//��������
		CTRLLEFTMOVEREQUEST,//Ctrl+��������
		FIRSTGETDATA,
		ZOOMINREQUEST
	};
	
	enum DISTACTSTATSTATE
	{
		BEGINSTAT,//��ʼͳ��
		FIRSTCLICK,//��һ�ε��
		SECONDCLICK,//�ڶ��ε��
		ENDSTAT//����ͳ��
	};//����ͳ��״̬
 	//////////////////////////////////////////////////////////////////////////

	CHisTechObj();
	~CHisTechObj(); 
	/////////////////////////////////////////////////////////////////////////
	//���������������
	//�����������С
	virtual void RecalSubObjSize(int iRightW = 50);
	//������Ʒ��Ϣ
	void SetStockInfo(const SCodeInfo* pCodeInfo);
	//����֤ȯ�����������Ϣ����ͼ
	void SetStockNameToPic();
	//////////////////////////////////////////////////////////////////////////
    //��������
	void RequestData();
	void RequestData_ZYQ();
	/////////////////////////////////////////////////////////////////////////
	//��������
	virtual BOOL UpdateData(LPVOID pData,  int nDataSize);
	/////////////////////////////////////////////////////////////////////////
	//��ͼ
	virtual BOOL Draw(CDC* pDC);
	/////////////////////////////////////////////////////////////////////////	
	//�������ܣ���Ӧ�˵���Ϣ
	BOOL OnUserCmd(UINT iSel);
	BOOL OnUpdateUserCmd(CCmdUI *pCmdUI);
	
	// ��ʼ��K����������
	void InitCircleType();
	
	// ��ǰK�����������Ƿ�Ϊ��ʼ״̬
	BOOL IsInitCircleType();
	int GetCurKLinePriodID(){return m_nCurKLinePriodID;}

public://��Ϣ��Ӧ
	BOOL TreatDayDataArrived(const char* pHisotyData);//�������ߵ�����Ϣ
	BOOL TreatDayDataArrived_ZYQ(const char* pHisotyData);//�������ߵ�����Ϣ
	void SetFormulaCommand(char* pCommand);//����ָ������
	void ResetTechData();//����ָ��״̬
	void DestoryData();//ɾ������
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ���Ӧ�����¼� 
	virtual BOOL OnChar(int nChar);	 
	//�����Ӧ
	virtual BOOL HitTest(CPoint point);
	BOOL MouseLBtnUp(CPoint point);
	BOOL MouseLBtnDown(CPoint point);
	BOOL MouseLBtnDbClick(CPoint point);
	BOOL MouseMove(CPoint point);
	BOOL MouseRBtnDown(LPVOID lpWnd,CPoint point);
	BOOL MouseRBtnUp(CPoint point);

	CTechCreator* GetPic(int nPic) {return m_pHisCurvePic+nPic;};
private:
	//////////////////////////////////////////////////////////////////////////
	//Ctrl ����
	void CtrlRightMove();
	//Ctrl ����
	void CtrlLeftMove();
	//����
	void RightMove();
	//����
	void LeftMove();
 	//����K�߿��
	void AdjustKUnitWidth();
	//����ˮƽ�����
	void SetHoriPoint();
	//��ͼ�Ŵ�
	void ZoomOutData();
	//��ͼ��С
	void ZoomInData();

	//////////////////////////////////////////////////////////////////////////	
	//���ݵ�ǰλ�ø���ͼ��
	void TransCurStationToPic();	
	//�������ݵ���ͼ
	void TransKDataToPic();
	//������ʷ��ͼ����ͼ�����������
	//////////////////////////////////////////////////////////////////////////
	//����ʮ�ֹ��
	void DrawCrossCursor(CDC* pDC); 	
	//����ʱ������
	void DrawTimeRect(CDC* pDC);	
	// �������µ����ݸ���k�ߵĶ�Ӧֵ
	void UpdateKLineMinPriceByNowData(float &rfOldValue,float fNewValue);

	void ReAssignPicNum(int nNewPicNum);
	void CalcDistractInfo();
	void DrawDistractLine(CDC* pDC);

	BOOL MakeDayData(StuRecvQuoteInfo* pQuote);
	const SHistoryUnit* GetCurrentKUnit();
	void DeleteIndex();
	
    // �����ǰK�������Ǵ����յģ�����Ҫ���������K�ߺϲ�����ʷK����ȥ
	BOOL CombineTodayKLine(unsigned char usKLinePriod,const SHistoryUnit*  pTodayKUnit,
					   SHistoryUnit* pPreHisKUnit);

	// ����K������Ϊ������ʾ����
	void AdjustKLineData2Valid(SHistoryUnit& rNowKUnit,const SHistoryUnit* pAllKUnitList,int nNowKUnitIndex);
	
    // ����ָ��״̬����
	BOOL SaveIndexStatusParam();
	// װ��ָ��״̬����
	BOOL LoadIndexStatusParam();
	
	// ��ȡ��ǰʮ�ֹ������λ�õļ۸�
	BOOL GetCurCrossCursorPosValue(CString &rstrFormateValue,float &rfValue);

	//�ӷ��������ܵ����ݻ��������ݱ���////////////////////////////////////////
	const SCodeInfo*	m_pCodeInfo;			 //��Ʒ��Ϣ
	int					m_nCircleInfo;			 //������Ϣ	
	SHistoryUnit*		m_pRecvKUnit;			 //�Ѵӷ����������������ʷ����ָ��
	SHistoryUnit*		m_pDoneKunit;			 //�������K�����ںϲ�K�ߺʹ����Ȩʹ��
	SHistoryUnit*		m_pTranslateKUnit;		 //������K��
	BOOL				m_bTranslateIsPoint;		 //�����Ƿ�Ϊָ��
	int					m_nValueNum;			     //�Ѵӷ����������������ʷ���ݸ���	

	short				m_sRequestStatus;		 //��������״̬���ֵ�һ�κ͵�һ���Ժ�����	
	short				m_sLastSize;			     //���һ������K�����ݸ���
	REQUESTDATATYPE     m_nLeftMoveRequestStatus;	//����״̬
	
	int m_nDrawLineState;			//����״̬
	
	//��ʷ��ͼ״̬�ı����
	////////////////////////////////////////////////////
	int		   m_nDataBegin;			 //���ݿ�ʼλ
	int		   m_nDataEnd;				 //���ݽ���λ
	int		   m_nDataOffset;			 //����ƫ����
	int		   m_nKUnitWidth;			 //��ǰK�߿��
	float	   m_fCurrentWidth;			 //��ǰK�߷ָ��ȣ�m_nKUnitWidth + k��֮��ļ�϶��	
	int	       m_nRecsOfUnit;			 //һ��K���ϰ�����K������
	int        m_nCanShowMaxKUnits;      //��ǰ����ʾ�����K������

	//��ʷ������ʾ����////////////////////////////////////////////////////////
	CRect	   m_TimeRect;			      //ʱ������
	int	       m_nTotalPicNum;		      //ͼ������
 	CRect	   m_TipInfoRect;			  //��Ϣ������
	CRect	   m_KLineRect;			      //K������
	BOOL	   m_bMouseMove;			  //����ƶ�	
	BOOL	   m_bLeftButtonDown;	      //����������	
	CPoint	   m_DrapPoint;				  //�϶���
	int		   m_nVertStation;			  //��ǰ��괹ֱλ��
	int		   m_nHoriStation;			  //��ǰ���ˮƽλ��
	int		   m_nNowShowDate;			  //��ǰ����ʾ���� add by gongxiaofeng at 2004-7-23
	int*       m_pHoriPoint;			  //ˮƽ������λ��	
	int        m_nHoriPoint;              //ˮƽ�������
	int        m_nMaxParameret;	          //����ָ������е����ֵ
	int		   m_nParameret;			  //��ǰƫ��λ��ֵ
	
	//�Ӷ���//////////////////////////////////////////////////////////////////
	CTechCreator*     m_pHisCurvePic;     //��ʷ����
	CInfoTable        m_InfoTable;		  //��Ӧ���ڵ�K��������ʾ���

 	DWORD								m_dwShowStyle;//��ʾ����
	int									m_nCurrentPicNum;//ͼ������
//	BOOL								m_bRequestedData;//��������״̬
	//����ͳ����
	CDistractStatTable* m_pDistractStatTable;//ͳ�Ʊ��
	DISTACTSTATSTATE					m_nDistractStatState;//����ͳ��״̬
	int									m_nFirstClickState;//��һ�ε��λ��
	int									m_nSecondClickState;//�ڶ��ε��λ��		
	int									m_nCurrentTechIndex;
private://INI�ļ���д��
	CString		m_strIndexParamPath;  //ָ�������ļ�·��
	int			m_nCurKLinePriodID;//��ǰK���������Ͷ�Ӧ����ԴID
	BOOL		m_bDrawCross;//�Ƿ񻭳�ʮ����
	BOOL		m_bMouseInRect;//����Ƿ��ڵ�ǰ����
	int			m_nDrawState;	//����״̬
	BOOL		m_bChipDistribPic;//�ƶ�������ʾ״̬

	int			m_nMemoryStation;//��ǰ����λ��

private://����K����
	void CtrlDownZoom();
	void CtrlUpZoom();
	int			m_nRequestSize;//�����С
	int			m_nMemorySize;//�����е������С
	BOOL		m_bAddDataEnd;//���������Ƿ����
private:
	int FindStationFromDate(int nDate);
	void DrawSelectArea(CDC* pDC);//����ѡ������

	CRect m_rectSelect;
	BOOL m_bDrawSelect;
	//////////////////////////////////////////////////////////////////////////
	//add by david gong at 20140806
	CString	m_strDefaultIndex;//Ĭ��ָ��

	BOOL m_bMousePress;
	CPoint m_ptCurPress;
};

#endif /* _INC_HISTECHOBJ_409F3AB302ED_INCLUDED */
