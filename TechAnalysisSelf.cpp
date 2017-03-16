 // TechAnalysisSelf.cpp: implementation of the CTechAnalysisSelf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "koaclient.h"
#include "BasePic.h"
#include "TechAnalysisSelf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//	���ע�ắ����������ָ����ľ�̬����
//
//��������������Ķ�ȡ������������
//�����ʽҪ�õ��������ݣ�����ͨ��ע��Ļص����� g_aCalcStack.pfunc ���õ����ݡ�
//�������ݣ���������Լ����ֻҪ�Լ��ĺ����ܹ���ȡ���Ϳ��ԡ�
//���磺�������Ҫ��ȡ��Ϣ����!
long CALLBACK CallBackDataIO(SCodeInfo stk,DATA_TYPE nDType,void * pData,short nDataNum)
{
	switch(nDType )
	{
	case K_DATA:	//�õ�K������
	case M_DATA:	//�õ���������
	case HQ_DATA:	//�õ���������
	case FN_DATA:	//�õ���������
		break;
	}
	return 0;
}
//���캯��ָ��ָ������
CTechAnalysisSelf::CTechAnalysisSelf(const char * zbCode) : CTechAnalysisBasePic()
{
	CString		title;
	strcpy(m_szKey,zbCode);
	m_thCalc.RegisterDataIO(CallBackDataIO);

	if ( m_thCalc.Calc(F_ZB,zbCode,NULL,m_nValueNum) )
	{
		int		i;
		for ( i=0;i<m_thCalc.GetOutVarSize();++i )
		{
			if ( i>0 )		title += ",";
			title += m_thCalc.GetOutVarWord(i);
		}
		title.MakeUpper();
		Create(title,m_thCalc.GetRegParSize(),m_thCalc.GetOutVarSize());
		//ָ��Ĭ�ϲ���
		for ( i=0;i<m_thCalc.GetRegParSize();++i )
		{
			m_pParamValue[i] = m_thCalc.GetRegParFloat(i);
		}
		//��������Ѳ������̣�Ȼ��ȡ��
		//ReadTechCurveParam(MACD);//��ȡ����
	}

	SetTechType(CString(zbCode));
}

CTechAnalysisSelf::~CTechAnalysisSelf()
{

}

//�������ܣ������Ϲ�ʽ���õĵĻ��߷�ʽת���ɿɻ��ķ�ʽ��
//����������nGSTypeָ������ֵ
//�������أ�ָ������ֵ
int		CTechAnalysisSelf::GetLineType(int nGSType)
{
	switch(nGSType)
	{
	case VOLSTICK:
		return CTechAnalysisBasePic::VOLUME;
	case CIRCLEDOT:	
		return CTechAnalysisBasePic::CIRCLELINE;
	case COLORSTICK:
		return CTechAnalysisBasePic::BARLINE;
	default:
		return CTechAnalysisBasePic::LINKLINE;
	}
}

//�������ܣ����ݼ���ָ�������ֵ�õ�ָ������
//����������nTypeָ������ֵ
//�������أ�ָ������
CString CTechAnalysisSelf::GetAnalysistName(int nType)
{
	return CString(m_szKey);
	//return CTechAnalysisBasePic::GetAnalysistName(nType);
}

//�������ܣ�����MACDָ������
//������������
//�������أ���
void CTechAnalysisSelf::CalculateValue()
{
	//////////////////////////////////////////////////////////////////////////
	//��������  0x7fffffff��Ϊ��Ч����
	int		i,nc;
	if(m_nValueNum < 0)
		return;

	if ( m_thCalc.Calc((short)F_ZB,m_szKey,m_pKUnit,m_nValueNum) )
	{
		DestroyObj();//���������
		
		if (m_nCurves < m_thCalc.GetOutVarSize() ||
			m_nParamNum < m_thCalc.GetRegParSize())
		{
			ResetParaBuffers(m_thCalc.GetRegParSize(), m_thCalc.GetOutVarSize());
		}
		else
		{
			m_nCurves = m_thCalc.GetOutVarSize();
			m_nParamNum = m_thCalc.GetRegParSize();
		}
		
		InitDataBuffer(m_nValueNum);//��ʼ��������
	
		CString title;
		///////////////////////////////////////
		//������ʣ���ֹ���
		int nDiv;
		nDiv=1;
		float fV,fMax;
		fMax  = 0;
		for ( nc =0;nc<m_nCurves;++nc )
		{
			for (i = 0; i < m_nValueNum; i++)
			{
				if ( !InValid(m_thCalc[nc][i]) )
				{
					fV = m_thCalc[nc][i];
					if(fMax < fV)
						fMax = fV;
				}
			}
		}
		if(fMax >1000000)
			nDiv = 10000;
		////////////////////////////////////////
		for ( nc =0;nc<m_nCurves;++nc )
		{
			for (i = 0; i < m_nValueNum; i++)
			{
				if ( InValid(m_thCalc[nc][i]) )
					m_ppCurveValue[nc][i] = 0x7fffffff;
				else
				{
					m_ppCurveValue[nc][i] = m_thCalc[nc][i];
					m_ppCurveValue[nc][i] /= nDiv;
				}
			}
			m_pLineType[nc] = GetLineType(m_thCalc.GetOutVarProperty(nc));

			if ( nc>0 )		title += ",";
			title += m_thCalc.GetOutVarWord(nc);
		}

		for ( i=0;i<m_thCalc.GetRegParSize();++i )
		{
			m_pParamValue[i] = m_thCalc.GetRegParFloat(i);
		}
		title.MakeUpper();
		UpdateIndexParamInfo(title);
	}
}
//���Ч�ʣ�ȫ����һ��ָ�����ɡ�
BOOL    InValid(float  x)
{
	int	ret = memcmp(&x,INVALIDF,sizeof(float));
	return (ret==0);
}

BOOL CTechAnalysisSelf::UpdateData(LPVOID pNewData)
{
	BOOL bRet;
	bRet = CTechAnalysisBasePic::UpdateData(pNewData);
	if((0 == strcmp(m_szKey,"MA"))||(0 == strcmp(m_szKey,"BOLL")))
	{
		m_nDecimal = ((CMulQuantitySequence*)pNewData)->m_lMax;
	}
	return bRet;
}

void CTechAnalysisSelf::SetVertZoomRaw(float fRaw)
{
	m_fVertZoomRaw = fRaw;
	CalcStation();
}
