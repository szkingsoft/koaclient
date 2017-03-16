// AnalisysObj.cpp: implementation of the CAnalisysObj class.

#include "stdafx.h"
#include "AnalisysObj.h"
#include "mainfrm.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnalisysObj::CAnalisysObj()
{
	m_nDrawMode = RealObj;
	m_pCodeInfo = NULL;
	m_ArrDrawMode.RemoveAll();
	m_ArrDrawMode2.RemoveAll();
	m_ArrDrawMode2.Add(m_nDrawMode);
//	m_PriceVolObj.SetDisplayMode();
}

CAnalisysObj::~CAnalisysObj()
{
	m_ArrDrawMode.RemoveAll();
	m_ArrDrawMode2.RemoveAll();
}


//函数功能：设置证券信息
//函数参数：pCodeInfo 证券信息
//函数返回：无
void CAnalisysObj::SetStockInfo(const SCodeInfo *pCodeInfo)
{
	if(pCodeInfo == NULL)
		return;

	m_pCodeInfo = pCodeInfo;
	
	m_PriceVolObj.SetStockInfo(pCodeInfo);
	m_HisTechObj.SetStockInfo(pCodeInfo);
	
	m_ArrDrawMode.RemoveAll();

	if(TechObj != m_nDrawMode) //历史分析		
		SetAnalisysMode(0);// 其他情况下都设置显示状态为“分时走势”
	else 
		SetAnalisysMode(1);	
}

//函数功能：计算子区域大小
//函数参数：无
//函数返回：无
void CAnalisysObj::RecalSubObjSize(int iRightW)
{
	CRect rtRect;

	//类型控制栏
	rtRect = CRect(m_DrawRect.left,m_DrawRect.bottom-16,m_DrawRect.right,m_DrawRect.bottom);
	m_TypeCtlPic.MoveTo(rtRect);

	//分时走势图
	rtRect.bottom = rtRect.top;
	rtRect.top = m_DrawRect.top;
	//rtRect.right -= 2;
	m_PriceVolObj.MoveTo(rtRect);

	//历史分析图
	m_HisTechObj.MoveTo(rtRect);

}

//函数功能：传递行情数据
//函数参数：pData 数据缓冲区指针
//函数返回：TRUE/FALSE
BOOL CAnalisysObj::UpdateData(LPVOID pData,int nDataSize)
{
	if(NULL  == m_pCodeInfo)return FALSE;
	SNormaHead* pDataHead = (SNormaHead*)pData;
	if(pDataHead->m_usType == HISKDATA||pDataHead->m_usType == HISKDATA_ZYQ)
	{
		return  m_HisTechObj.UpdateData(pData,0);
	}
	else if((pDataHead->m_usType == STKTRACEDATA)||(pDataHead->m_usType == STKMINUTEDATA))
	{
		return  m_PriceVolObj.UpdateData(pData);
	}
	else if(pDataHead->m_usType == PUSH_REALDATA)
	{
		StuRecvQuoteInfo* pQuote = (StuRecvQuoteInfo*)(pDataHead+1);
		//int nRecord = 0;
		//int nHqstruLen = 0;
		//nHqstruLen = sizeof(StuRecvQuoteInfo);
		//nRecord = (nDataSize-sizeof(SNormaHead))/nHqstruLen;
		char szTmp[100];
		SNormaHead* pSendData = (SNormaHead*)szTmp;
		pSendData->m_usSize = 1;
		pSendData->m_usType = PUSH_REALDATA;
		for(int i=0;i<pDataHead->m_usSize;i++)
		{
			StuRecvQuoteInfo* pQuote1 = (StuRecvQuoteInfo*)(pQuote+i);
			if(0 ==strcmp(pQuote1->sCode.szCode,m_pCodeInfo->szCode))
			{
				memcpy(pSendData+1,pQuote1,sizeof(StuRecvQuoteInfo));
				if(m_nDrawMode == RealObj)
					m_PriceVolObj.UpdateData(pSendData);
				else
					m_HisTechObj.UpdateData(pSendData,0);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//函数功能：测试点击位置
//函数参数：point 鼠标点击点
//函数返回：TRUE - 点中；FALSE - 未点中
BOOL CAnalisysObj::HitTest(CPoint point)
{
	return m_DrawRect.PtInRect(point);
}

//函数功能：画图
//函数参数：pDC 设备环境
//函数返回：TRUE/FALSE
BOOL CAnalisysObj::Draw(CDC* pDC)
{
	if(NULL == m_pCodeInfo)return FALSE;
	switch(m_nDrawMode)
	{
		case RealObj: //分时走势
			m_PriceVolObj.Draw(pDC);
			break;
		case TechObj:			//历史分析
			m_HisTechObj.Draw(pDC);
			break;
		default:
			break;
	}
	//类型控制栏
	m_TypeCtlPic.Draw(pDC);
	return TRUE;
}

//函数功能：初始化环境
//函数参数：无
//函数返回：无
void CAnalisysObj::Initialization()
{
	//插入单元文本
	m_TypeCtlPic.ClearAllTab();
	if(CPublic::Instance()->IsMinuteDataMode())
	{
		m_TypeCtlPic.InsertUnit(CLanguageInterface::Instance()->GetStr(20001),true);
	}
	else
	{
		m_TypeCtlPic.InsertUnit(CLanguageInterface::Instance()->GetStr(20003),true);
	}
	m_TypeCtlPic.InsertUnit(CLanguageInterface::Instance()->GetStr(20002));
}

//函数功能：键盘响应函数
//函数参数：nChar 按键ID
//函数返回：TRUE/FALSE
BOOL CAnalisysObj::OnChar(int nChar)
{
	BOOL bResult;
	bResult = FALSE; 
	//判断是否是F1~F12功能键
	int nSel;
 	switch(nChar)
	{
		case VK_F1:
			{
				nSel = 2;
				bResult = TRUE;
			}
			break;
		case VK_F2:
			{
				nSel = 3;
				bResult = TRUE;
			}
			break;
		case VK_TAB:
			{
				nSel = m_nDrawMode / 0x0010 - 1;
				{
					nSel = 0;
					m_nDrawMode = RealObj;
				}
				
				m_TypeCtlPic.OnSelectFocus(nSel);
				bResult = TRUE;
			}
			break;
		default:
			break;
	}
	//若是F1、F2、F5则改变实时分析视图
	if(bResult)
	{
		SetModeAndFocus(nSel);
		return bResult;
	}
	//其他键盘事件处理
 	switch(m_nDrawMode)
	{
		case RealObj: //分时走势
			bResult = m_PriceVolObj.OnChar(nChar);
			break;
		case TechObj: //历史分析
			bResult = m_HisTechObj.OnChar(nChar);
			break;
		default:
			break;
	}
 	return bResult;
}

// 响应系统菜单、工具栏的命令
BOOL CAnalisysObj::OnUserCmd(int nSel)	
{
	switch( m_nDrawMode) 
	{
	case RealObj:
		{
			return m_PriceVolObj.OnUserCmd(nSel);			
		}
		break;			
	case TechObj:
		return m_HisTechObj.OnUserCmd(nSel);		
	default:
	    return FALSE;
	}	

	return TRUE;
}

BOOL CAnalisysObj::OnUpdateUserCmd(CCmdUI *pCmdUI)
{
	switch(m_nDrawMode) 
	{
	case RealObj:
		{
			switch(pCmdUI->m_nID) 
			{
			case ID_TECHANALY_1MINLINE://1分钟线
			case ID_TECHANALY_5MINLINE: //5分钟线
			case ID_TECHANALY_10MINLINE: //10分钟线
			case ID_TECHANALY_15MINLINE: //15分钟线
			case ID_TECHANALY_30MINLINE: //30分钟线
			case ID_TECHANALY_60MINLINE: //60分钟线
			case ID_TECHANALY_2HOURS: //2小时线
			case ID_TECHANALY_4HOURS: //4小时线
			case ID_TECHANALY_DAYLINE: //日线
			case ID_TECHANALY_WEEKLINE: //周线
			case ID_TECHANALY_MONLINE: //月线
			case ID_TECHANALY_TOTAL:
			case ID_INTELLIG_FUNCTIONMANAGE:
				{
					pCmdUI->Enable(FALSE);
				}
				return TRUE;
			default:
			    return m_PriceVolObj.OnUpdateUserCmd(pCmdUI);
			}			
		}
		break;
	case TechObj:
		{
			switch(pCmdUI->m_nID)
			{
			case  ID_TECHANALY_TOTAL:
			case  ID_TECHANALY_ZOOMOUT:
			case  ID_TECHANALY_ZOOMIN:
				{
					pCmdUI->Enable(TRUE);
					m_HisTechObj.OnUpdateUserCmd(pCmdUI);
				}
				break;
			default:
				return m_HisTechObj.OnUpdateUserCmd(pCmdUI);				
			}
		}
		break;
	default:
	    return FALSE;
	}	

	return TRUE;
}

//函数功能：判断画图模式是否在数组中
//函数参数：nDrawMode 画图模式
//函数返回：TRUE 存在；FALSE 不存在
BOOL CAnalisysObj::IsExistDrawMode(int nDrawMode)
{
	int iRec;
	for(iRec = 0; iRec < m_ArrDrawMode.GetSize(); iRec++)
	{
		if(nDrawMode == m_ArrDrawMode.GetAt(iRec))
			return TRUE;
	}
	return FALSE;
}

//函数功能: 设置画图模式
//函数参数: nDrawMode 画图模式
//函数返回：无
void CAnalisysObj::SetAnalisysMode(int nDrawMode,BOOL bRequstData)
{
	m_PriceVolObj.ActivateObj(FALSE);
	m_HisTechObj.ActivateObj(FALSE);
	bool bNeedInit = m_nSel == nDrawMode ? false : true;
	m_nSel = nDrawMode;

	switch(nDrawMode)
	{
		case 0:  //分时走势
			m_PriceVolObj.ActivateObj();
			m_nDrawMode = RealObj;
			if((!IsExistDrawMode(RealObj))&&bRequstData)
			{
				m_PriceVolObj.RequestData();
				m_ArrDrawMode.Add(RealObj);
			}		
			break;
		case 1:  //历史分析
			{
				int nLastDrawMode = m_nDrawMode;
				m_nDrawMode = TechObj;
				m_HisTechObj.ActivateObj();

				if(bNeedInit)
				{
					if(IsExistDrawMode(TechObj))
					{
						if( m_HisTechObj.IsInitCircleType())
							break;
						m_HisTechObj.InitCircleType();
					}
					else
					{
						// 从其他状态切换为技术分析时，初始技术分析k线周期，保证显示“日”线
						if( nLastDrawMode != TechObj)
							m_HisTechObj.InitCircleType();
					}		
				}	
				
				m_HisTechObj.SetStockInfo(m_pCodeInfo);
				if(bRequstData)
				{
					m_HisTechObj.RequestData();
					m_ArrDrawMode.Add(TechObj);
				}
			}	 
			break;
		default:
			break;
	}
	if(nDrawMode != m_TypeCtlPic.GetSelTabID())
	{
		m_TypeCtlPic.OnSelectFocus(nDrawMode);
	}
	SetDrawMode(m_nDrawMode);
}

//函数功能: 响应鼠标按键消息
//函数参数：鼠标点击点
//函数返回：TRUE/FALSE
BOOL CAnalisysObj::MouseLBtnDown(CPoint point)
{
	//响应类型控制栏
	if(m_TypeCtlPic.MouseDown(point))
	{
		//设置画图模式
		SetAnalisysMode(m_TypeCtlPic.GetSelTabID());
		return 2;
	}
	BOOL bResult;
	bResult = FALSE;
	switch(m_nDrawMode)
	{
		case RealObj: //分时走势
			bResult = m_PriceVolObj.MouseLBtnDown(point);  
			break;
		case TechObj: //历史分析	
			bResult = m_HisTechObj.MouseLBtnDown(point); 
			break;
		default:
			break;
	}
	return bResult;
}

BOOL CAnalisysObj::MouseLBtnUp(CPoint point)
{
	BOOL bResult;
	bResult = FALSE;
	switch(m_nDrawMode)
	{
		case RealObj: //分时走势
			bResult = m_PriceVolObj.MouseLBtnUp(point);  
			break;
		case TechObj: //历史分析	
			bResult = m_HisTechObj.MouseLBtnUp(point); 
			break;
		default:
			break;
	}
	return bResult;
}

BOOL CAnalisysObj::MouseMove(CPoint point)
{
	BOOL bResult;
	bResult = FALSE;
	switch(m_nDrawMode)
	{
		case RealObj: //分时走势
			bResult = m_PriceVolObj.MouseMove(point);  
			break;
		case TechObj: //历史分析
			bResult = m_HisTechObj.MouseMove(point); 
			break;
		default:
			break;
	}
	return bResult;
}


BOOL CAnalisysObj::MouseLBtnDbClick(CPoint point)
{
	BOOL bResult;
	bResult = FALSE;
	switch(m_nDrawMode)
	{
		case TechObj:   //历史分析
			if(m_HisTechObj.HitTest(point))
				bResult = m_HisTechObj.MouseLBtnDbClick(point); 
			break;
		case RealObj:
			if( m_PriceVolObj.HitTest(point))
				bResult = m_PriceVolObj.MouseLBtnDbClick(point);
			break;
		default:
			break;
	}
	return bResult;
}

//函数功能：响应鼠标右键点击消息
//函数参数：lpWnd - 父窗口指针；point - 鼠标点击点
//函数返回：TRUE/FALSE
BOOL CAnalisysObj::MouseRBtnDown(LPVOID lpWnd,CPoint point)
{
	switch(m_nDrawMode)
	{
	case TechObj: //历史分析
		m_HisTechObj.MouseRBtnDown(lpWnd,point);
		break;
	case RealObj: //实时走势
		m_PriceVolObj.MouseRBtnDown(lpWnd,point);
		break;
	default:
		break;
	}
	return TRUE;
}

//函数功能：设置画图模式并设置类型控制栏焦点
void CAnalisysObj::SetModeAndFocus(int nSel,BOOL bRequestData)
{
	if(m_nSel ==nSel)return;
	
	if( bRequestData)
	{
	    SetAnalisysMode(nSel);
	}
	m_TypeCtlPic.OnSelectFocus(nSel);
}

//函数功能：返回前一个画图模式
//函数返回：前一个画图模式
int CAnalisysObj::GetPrevDrawMode()
{
	int iRec;
	int nMode;
	iRec = m_ArrDrawMode2.GetSize();
	if(iRec > 1)
	{
		nMode = m_ArrDrawMode2[iRec-2];
		m_ArrDrawMode2.RemoveAt(iRec-1);
	}
	else
	{
		nMode = -1;
	}
	return nMode;
}

//函数功能：设置画图模式
void CAnalisysObj::SetDrawMode(int nDrawMode)
{
	int nSize;
	nSize = m_ArrDrawMode2.GetSize();
	if(nDrawMode == m_ArrDrawMode2[nSize-1])
		return;
	m_ArrDrawMode2.Add(nDrawMode);
	//只保留最近10个画图模式
	if(nSize > 10)
		m_ArrDrawMode2.RemoveAt(0);
}

//函数功能：重新初始化表格
void CAnalisysObj::ReInitTable()
{
}

void CAnalisysObj::SetFormulaCommand(char *pCommand)
{
	if(TechObj == m_nDrawMode)
	{
		m_HisTechObj.SetFormulaCommand(pCommand);
	}
}

BOOL CAnalisysObj::MouseRBtnUp(CPoint point)
{
	if(TechObj == m_nDrawMode)
	{
		return m_HisTechObj.MouseRBtnUp(point);
	}
	return FALSE;
}
void CAnalisysObj::SetType(int iType)
{
	m_iType = iType;
	m_PriceVolObj.SetType(iType);
	m_HisTechObj.SetType(iType);
}
void CAnalisysObj::SetDrawLineCallBcakFun()
{
	m_HisTechObj.SetDrawLineCallBcakFun();
	if (m_pCodeInfo)
	{
		m_HisTechObj.SetCurSymbol(m_pCodeInfo->szCode);
	}
}
