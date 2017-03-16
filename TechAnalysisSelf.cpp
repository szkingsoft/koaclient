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


//	这个注册函数，可以是指定类的静态函数
//
//计算引擎可以灵活的读取其他各种数据
//如果公式要用到其他数据，可以通过注册的回调函数 g_aCalcStack.pfunc ，得到数据。
//具体数据：可以自行约定，只要自己的函数能够读取到就可以。
//例如：如果将来要读取信息地雷!
long CALLBACK CallBackDataIO(SCodeInfo stk,DATA_TYPE nDType,void * pData,short nDataNum)
{
	switch(nDType )
	{
	case K_DATA:	//得到K线数据
	case M_DATA:	//得到分钟数据
	case HQ_DATA:	//得到行情数据
	case FN_DATA:	//得到财务数据
		break;
	}
	return 0;
}
//构造函数指定指标名。
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
		//指标默认参数
		for ( i=0;i<m_thCalc.GetRegParSize();++i )
		{
			m_pParamValue[i] = m_thCalc.GetRegParFloat(i);
		}
		//可以任意把参数存盘，然后取出
		//ReadTechCurveParam(MACD);//读取参数
	}

	SetTechType(CString(zbCode));
}

CTechAnalysisSelf::~CTechAnalysisSelf()
{

}

//函数功能：市面上公式中用的的画线方式转换成可画的方式。
//函数参数：nGSType指标类型值
//函数返回：指标类型值
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

//函数功能：根据技术指标的类型值得到指标名称
//函数参数：nType指标类型值
//函数返回：指标名称
CString CTechAnalysisSelf::GetAnalysistName(int nType)
{
	return CString(m_szKey);
	//return CTechAnalysisBasePic::GetAnalysistName(nType);
}

//函数功能：计算MACD指标数据
//函数参数：无
//函数返回：无
void CTechAnalysisSelf::CalculateValue()
{
	//////////////////////////////////////////////////////////////////////////
	//计算数据  0x7fffffff作为无效数据
	int		i,nc;
	if(m_nValueNum < 0)
		return;

	if ( m_thCalc.Calc((short)F_ZB,m_szKey,m_pKUnit,m_nValueNum) )
	{
		DestroyObj();//清除老数据
		
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
		
		InitDataBuffer(m_nValueNum);//初始化数据区
	
		CString title;
		///////////////////////////////////////
		//计算比率，防止溢出
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
//提高效率，全部用一条指令处理完成。
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
