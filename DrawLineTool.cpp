// DrawLineTool.cpp: implementation of the CDrawLineTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "koaclient.h"
#include "DrawLineTool.h"
#include "TechAnalysisBasePic.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawLineTool::CDrawLineTool()
{
	memset(this,0,sizeof(*this));
}

CDrawLineTool::~CDrawLineTool()
{

}

void CDrawLineTool::MouseLBDown(CPoint point)
{
	m_pHistoryObj->GetDateFromPoint(point,m_BeginPoint.nDay,m_BeginPoint.fPrice);
	m_bSelected = TRUE;
	m_MoveSize= CSize(0,0);
}

void CDrawLineTool::MouseLBUp(CPoint point)
{
	m_pHistoryObj->GetDateFromPoint(point,m_EndPoint.nDay,m_EndPoint.fPrice);
}

void CDrawLineTool::Draw(CDC *pDC)
{
	if((m_EndPoint.nDay == 0)&&(m_EndPoint.fPrice<=0.00000f))
		return;
	if(0 == memcmp(&m_BeginPoint,&m_EndPoint,sizeof(SKeyPoint)))return;//两点相同
	
	CPoint beginpoint,endpoint;

	m_pHistoryObj->GetPointFromDate(m_BeginPoint.nDay,m_BeginPoint.fPrice,beginpoint);
	m_pHistoryObj->GetPointFromDate(m_EndPoint.nDay,m_EndPoint.fPrice,endpoint);


	
	if(m_bMoveState)
	{
		beginpoint += m_MoveSize;
		endpoint += m_MoveSize;
	}

	if(!m_DrawRect.PtInRect(beginpoint)||(!m_DrawRect.PtInRect(endpoint)))return;
	
	CPen LinePen(PS_SOLID,1,m_crLine);
	CPen * OldPen = pDC->SelectObject(&LinePen);
	switch(m_nState)
	{
	case LINE:
		DrawLine(pDC,beginpoint,endpoint);
		break;
	case RECTANGLE :
		DrawRectangle(pDC,beginpoint,endpoint);
		break;
	case PARALLEL:
		DrawParallel(pDC,beginpoint,endpoint);
		break;
	case GOLDLINE:
		DrawGoldLine(pDC,beginpoint,endpoint);
		break;
	case GANNLINE:
		DrawGannLine(pDC,beginpoint,endpoint);
		break;
	default:
		break;
	}
	//画出选择状态
	if(m_bSelected)
	{
		CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(WHITE_BRUSH);
		CPen* pOldPen = (CPen*)pDC->SelectStockObject(WHITE_PEN);
		pDC->Rectangle(beginpoint.x-3,beginpoint.y-3,
					   beginpoint.x+3,beginpoint.y+3);

		pDC->Rectangle(endpoint.x-3,endpoint.y-3,
						endpoint.x+3,endpoint.y+3);		
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
	pDC->SelectObject(OldPen);
}


int CDrawLineTool::CheckIsLine(CPoint point)
{
	CPoint ArrPoint[3];

	m_pHistoryObj->GetPointFromDate(m_BeginPoint.nDay,m_BeginPoint.fPrice,ArrPoint[0]);
	m_pHistoryObj->GetPointFromDate(m_EndPoint.nDay,m_EndPoint.fPrice,ArrPoint[1]);
	m_pHistoryObj->GetPointFromDate(m_ThirdPoint.nDay,m_ThirdPoint.fPrice,ArrPoint[2]);
	
	CRect FirstPointRect,EndPointRect,ThirdRect;
	FirstPointRect = CRect(ArrPoint[0].x-5,ArrPoint[0].y-5,
					   ArrPoint[0].x+5,ArrPoint[0].y+5);
	EndPointRect = CRect(ArrPoint[1].x-5,ArrPoint[1].y-5,
		ArrPoint[1].x+5,ArrPoint[1].y+5);
	ThirdRect = CRect(ArrPoint[2].x-5,ArrPoint[2].y-5,
					 ArrPoint[2].x+5,ArrPoint[2].y+5);

	CRect TotalRect(ArrPoint[0],ArrPoint[1]);
	TotalRect.NormalizeRect();
	m_bSelected = NOSELECT;//选择当前点
	if(FirstPointRect.PtInRect(point))
		m_bSelected = SELECTEDFIRSTPOINT;
	else if(EndPointRect.PtInRect(point))
		m_bSelected = SELECTEDENDPOINT;
	else if(ThirdRect.PtInRect(point))
		m_bSelected = SELECTTHIRDPOINT;
	else
	{
		if((m_nState == LINE)&&(abs(ArrPoint[0].x-ArrPoint[1].x)<5||abs(ArrPoint[0].y- ArrPoint[1].y)<5))
		{
			CRect rRect(ArrPoint[0].x -5,ArrPoint[0].y-5,ArrPoint[1].x+5,ArrPoint[1].y+5);
			if(rRect.PtInRect(point))
				m_bSelected = SELECTRECTANGLE;
		}
		else
			if(TotalRect.PtInRect(point))
				m_bSelected = SELECTRECTANGLE;
	}
	return m_bSelected;
}

void CDrawLineTool::DrawLine(CDC *pDC,CPoint beginPoint,CPoint endPoint)
{
	pDC->MoveTo(beginPoint);
	pDC->LineTo(endPoint);
}

void CDrawLineTool::DrawRectangle(CDC *pDC,CPoint beginPoint,CPoint endPoint)
{
	CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(beginPoint.x,beginPoint.y,endPoint.x,endPoint.y);
	pDC->SelectObject(oldBrush);
}

void CDrawLineTool::DrawParallel(CDC *pDC,CPoint beginPoint,CPoint endPoint)
{
	if((beginPoint == CPoint(0,0))
		||(endPoint == CPoint(0,0)))
		return;
	CPoint ThirdPoint,FourPoint;
	CPoint newpoint;
	
	int nDistance;
	if((m_ThirdPoint.nDay == 0)&&(m_ThirdPoint.fPrice <= 0.0000f))
	{
		nDistance = 40;
	}
	else
	{
		m_pHistoryObj->GetPointFromDate(m_ThirdPoint.nDay,m_ThirdPoint.fPrice,newpoint);//得到第三点坐标
		if(m_bMoveState)
		{
			newpoint += m_MoveSize;
		}
		
		float a,b,c,s;
		float area;
		a = sqrt(pow((float)(beginPoint.x -endPoint.x),2)+pow((float)(beginPoint.y-endPoint.y),2));
		b = sqrt(pow((float)(beginPoint.x -newpoint.x),2)+pow((float)(beginPoint.y-newpoint.y),2));
		c = sqrt(pow((float)(endPoint.x -newpoint.x),2)+pow((float)(endPoint.y-newpoint.y),2));
		s = (a+b+c)/2;
		area = sqrt(s*(s-a)*(s-b)*(s-c));//海伦公式
		nDistance = area*2/a;
		

		//////////////////////////////////////////////////////////////////////////
		CPoint linepoint;//开始结束线上的同向坐标
//		linepoint.y = newpoint.y;
//		if(endPoint.y!= beginPoint.y)
//			linepoint.x = beginPoint.x + (endPoint.x - beginPoint.x)*(linepoint.y - beginPoint.y  )/(endPoint.y - beginPoint.y );
//		else
//			linepoint.x = endPoint.x;
//
//		if(linepoint.x > newpoint.x)
//			nDistance = -nDistance;	
		linepoint.x = newpoint.x;
		if(endPoint.x == beginPoint.x)
			linepoint.y = endPoint.x;
		else
			linepoint.y = beginPoint.y +(endPoint.y - beginPoint.y)*(linepoint.x - beginPoint.x)/(endPoint.x -beginPoint.x);
		if(linepoint.y < newpoint.y)
			nDistance = -nDistance;		
	}


	if(beginPoint.x != endPoint.x)
	{
		float k=((float)(beginPoint.y-endPoint.y))/(beginPoint.x-endPoint.x);
		if( k!=0 )
		{
			//第三点的坐标
			ThirdPoint.x= nDistance*k /sqrt((double)( k*k+1 ) )+beginPoint.x ;
			ThirdPoint.y=(int)( -(ThirdPoint.x-beginPoint.x)/k+beginPoint.y );
			//第四点的坐标
			FourPoint.x =nDistance*k/sqrt((double)( k*k+1 ) )+endPoint.x;
			FourPoint.y =(int)( -(FourPoint.x-endPoint.x)/k+endPoint.y );
		}
		else if( k == 0)            //平行线第二条平行直线
		{
			ThirdPoint.x= beginPoint.x;
			ThirdPoint.y= beginPoint.y + nDistance;
			FourPoint.x = endPoint.x;
			FourPoint.y = endPoint.y + nDistance;
		}
	}
	else if(beginPoint.x==endPoint.x)   //直线处于垂直状态
	{
		ThirdPoint.x= beginPoint.x+ nDistance;
		ThirdPoint.y= beginPoint.y;
		FourPoint.x = endPoint.x+ nDistance;
		FourPoint.y = endPoint.y;
	}
	if((m_ThirdPoint.nDay == 0)&&(m_ThirdPoint.fPrice <= 0.00000f))
	{
		newpoint = ThirdPoint;
		SetThirdPoint(ThirdPoint);
	}

	//////////////////////////////////////////////////////////////////////////
	//画线
	CPoint point[4];
	getBorderPoint(beginPoint,endPoint,&point[0]);
	getBorderPoint(ThirdPoint,FourPoint,&point[2]);
	
	if(point[0] != point[1]&&(m_DrawRect.PtInRect(point[0])||m_DrawRect.PtInRect(point[1])))
	{
		pDC->MoveTo(point[0]);
		pDC->LineTo(point[1]);
	}
	if((point[2] != point[3])&&(m_DrawRect.PtInRect(point[2])||(m_DrawRect.PtInRect(point[3]))))
	{
		pDC->MoveTo(point[2]);
		pDC->LineTo(point[3]);
	}
	//////////////////////////////////////////////////////////////////////////
	//画出第三点的焦点
	if((m_bSelected)&&m_DrawRect.PtInRect(newpoint))
	{
		CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(WHITE_BRUSH);
		CPen* pOldPen = (CPen*)pDC->SelectStockObject(WHITE_PEN);
		pDC->Rectangle(newpoint.x-3,newpoint.y-3,
			newpoint.x+3,newpoint.y+3);	
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
}

void CDrawLineTool::DrawGoldLine(CDC *pDC,CPoint beginPoint,CPoint endPoint)
{
	CRect rcDraw;
	m_pHistoryObj->GetDrawRect(rcDraw);
	beginPoint.x = rcDraw.left;
	endPoint.x = rcDraw.right;

	CFont* pTextFont = CInterfaceManager::Instance()->GetSysFont(ID_HISTORYPICTITLEFONT);
	CFont* OldFont = pDC->SelectObject(pTextFont);

	int nMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF TextColor = CInterfaceManager::Instance()->GetSysColor(ID_FORMTITLE_COLOR);
	pDC->SetTextColor(TextColor); 
	CPen pnLine(PS_DOT,1,m_crLine);
	CPen *OldPn;
	int iDay = 0;
	float fPrice = 0.0;
	CString strTemp;

	pDC->MoveTo(beginPoint);
	pDC->LineTo(endPoint.x,beginPoint.y);//第一条线

	OldPn = pDC->SelectObject(&pnLine);
	int nSize = (endPoint.y - beginPoint.y)*0.618;
	pDC->MoveTo(beginPoint.x,beginPoint.y + nSize);
	pDC->LineTo(endPoint.x,beginPoint.y + nSize);//下侧黄金线
	m_pHistoryObj->GetDateFromPoint(CPoint(beginPoint.x+5,beginPoint.y + nSize),iDay,fPrice);
	strTemp.Format(_T("%.2f"),fPrice);
	strTemp = _T("61.8% ") + strTemp;
	pDC->TextOut(beginPoint.x+5,beginPoint.y + nSize - 12,strTemp);

	pDC->MoveTo(beginPoint.x,endPoint.y- nSize );
	pDC->LineTo(endPoint.x,endPoint.y - nSize);//上侧黄金线
	m_pHistoryObj->GetDateFromPoint(CPoint(beginPoint.x+5,endPoint.y- nSize),iDay,fPrice);
	strTemp.Format(_T("%.2f"),fPrice);
	strTemp = _T("38.2% ") + strTemp;
	pDC->TextOut(beginPoint.x+5,endPoint.y- nSize - 12,strTemp);

	pDC->MoveTo(beginPoint.x,(beginPoint.y+endPoint.y)/2);
	pDC->LineTo(endPoint.x,(beginPoint.y+endPoint.y)/2);//中间线
	m_pHistoryObj->GetDateFromPoint(CPoint(beginPoint.x+5,(beginPoint.y+endPoint.y)/2),iDay,fPrice);
	strTemp.Format(_T("%.2f"),fPrice);
	strTemp = _T("50% ") + strTemp;
	pDC->TextOut(beginPoint.x+5,(beginPoint.y+endPoint.y)/2 - 12,strTemp);

    pDC->SelectObject(OldPn);
	pDC->MoveTo(beginPoint.x,endPoint.y);
	pDC->LineTo(endPoint);//最后一条线
	m_pHistoryObj->GetDateFromPoint(CPoint(beginPoint.x+5,endPoint.y),iDay,fPrice);
	strTemp.Format(_T("%.2f"),fPrice);
	strTemp = _T("100% ") + strTemp;
	pDC->TextOut(beginPoint.x+5,endPoint.y - 12,strTemp);
}

void CDrawLineTool::DrawGannLine(CDC *pDC,CPoint beginPoint,CPoint endPoint)
{
	if(endPoint.x == beginPoint.x)return;
	if(endPoint.y == beginPoint.y)return;
	
	CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(beginPoint.x,beginPoint.y,endPoint.x,endPoint.y);
	pDC->SelectObject(oldBrush);

	pDC->MoveTo(beginPoint);
	pDC->LineTo(endPoint);

	int nXSize,nYSize;
	nXSize = (endPoint.x - beginPoint.x)/4;
	nYSize = (endPoint.y - beginPoint.y)/4;
	
	int nSignPoint;
	CSize newsize;
	newsize = endPoint- beginPoint;
	for(int i = 1;i<4;i++)
	{
		pDC->MoveTo(beginPoint.x,beginPoint.y+nYSize*i);
		pDC->LineTo(endPoint.x,beginPoint.y+nYSize*i);//水平线

		pDC->MoveTo(beginPoint);
		if(beginPoint.x < endPoint.x)
		{
			nSignPoint = beginPoint.y + (m_DrawRect.right- beginPoint.x)*i*nYSize/newsize.cx;
			if(nSignPoint>m_DrawRect.bottom)
				nSignPoint = m_DrawRect.bottom;
			if(nSignPoint<m_DrawRect.top)
				nSignPoint = m_DrawRect.top;
			pDC->LineTo(m_DrawRect.right,nSignPoint);
		}
		else
		{
			nSignPoint = beginPoint.y +(m_DrawRect.left - beginPoint.x)*i*nYSize/newsize.cx;
			if(nSignPoint>m_DrawRect.bottom)
				nSignPoint = m_DrawRect.bottom;
			if(nSignPoint<m_DrawRect.top)
				nSignPoint = m_DrawRect.top;
			pDC->LineTo(m_DrawRect.left,nSignPoint);
		}
		
		pDC->MoveTo(beginPoint.x+nXSize*i,beginPoint.y);
		pDC->LineTo(beginPoint.x+nXSize*i,endPoint.y);//垂直线

		pDC->MoveTo(beginPoint);
		if(beginPoint.y < endPoint.y)
		{
			nSignPoint = beginPoint.x + (m_DrawRect.bottom- beginPoint.y)*i*nXSize/newsize.cy;
			if(nSignPoint >m_DrawRect.right)
				nSignPoint = m_DrawRect.right;
			if(nSignPoint<m_DrawRect.left)
				nSignPoint= m_DrawRect.left;
			pDC->LineTo(nSignPoint,m_DrawRect.bottom);		
		}
		else
		{
			nSignPoint = beginPoint.x +(m_DrawRect.top-beginPoint.y)*i*nXSize/newsize.cy;
			if(nSignPoint >m_DrawRect.right)
				nSignPoint = m_DrawRect.right;
			if(nSignPoint<m_DrawRect.left)
				nSignPoint= m_DrawRect.left;
			pDC->LineTo(nSignPoint,m_DrawRect.top);
		}
	}
}

void CDrawLineTool::SetDrawMode(int nState)
{
	 m_nState = (LINETYPE)nState;
}

void CDrawLineTool::SetOffset(CSize offset)
{
//	beginPoint += offset;
//	endPoint += offset;
	m_bMoveState = TRUE;
	m_MoveSize +=offset;

}

void CDrawLineTool::SetSelectState(BOOL bSelected)
{
	m_bSelected = bSelected;
}

void CDrawLineTool::SetThirdPoint(CPoint point)
{
	m_pHistoryObj->GetDateFromPoint(point,m_ThirdPoint.nDay,m_ThirdPoint.fPrice);
}

void CDrawLineTool::getBorderPoint(CPoint Begin, CPoint End, CPoint* BordPoint)
{
	if(!m_DrawRect.IsRectNull())
	{
		if((Begin.x !=End.x)&&(Begin.y!=End.y))
		{
			int nHeight,nWidth;
			int nStep= 0;;
			nHeight=(Begin.y-End.y)*(m_DrawRect.left-Begin.x)/(Begin.x-End.x)+Begin.y;
			if(nHeight>=m_DrawRect.top&&nHeight<=m_DrawRect.bottom)//与左边框有交点
				BordPoint[nStep++]=CPoint(m_DrawRect.left,nHeight);
			
			nWidth=(Begin.x-End.x)*(m_DrawRect.top-Begin.y)/(Begin.y-End.y)+Begin.x;
			if(nWidth>=m_DrawRect.left&&nWidth<=m_DrawRect.right)//与上边框有交点
				BordPoint[nStep++]=CPoint(nWidth,m_DrawRect.top);
			if(nStep>= 2)return;

			nHeight=(Begin.y-End.y)*(m_DrawRect.right-Begin.x)/(Begin.x-End.x)+Begin.y;
			if(nHeight>=m_DrawRect.top&&nHeight<=m_DrawRect.bottom)//与右边框有交点
				BordPoint[nStep++]=CPoint(m_DrawRect.right,nHeight);
			if(nStep>= 2)return;
			
			nWidth=(Begin.x-End.x)*(m_DrawRect.bottom-Begin.y)/(Begin.y-End.y)+Begin.x;
			if(nWidth>=m_DrawRect.left&&nWidth<=m_DrawRect.right)//与下边框有交点
				BordPoint[nStep++]=CPoint(nWidth,m_DrawRect.bottom);
			if(nStep>= 2)return;
		}
		else if(Begin.x==End.x)
		{
			BordPoint[0] = CPoint(Begin.x,m_DrawRect.top);
			BordPoint[1] = CPoint(Begin.x,m_DrawRect.bottom);
		}
		else if(Begin.y==End.y) 
		{
			BordPoint[0] = CPoint(m_DrawRect.left,Begin.y);
			BordPoint[1] = CPoint(m_DrawRect.right,Begin.y);
		}
	}
}

void CDrawLineTool::SetParent(LPVOID pParent)
{
	m_pHistoryObj = (CTechAnalysisBasePic*)pParent;
	if(m_pHistoryObj)
		m_pHistoryObj->GetDrawRect(m_DrawRect);
}

void CDrawLineTool::GetDrawRect(CRect &rRect)
{
	CPoint Point[2];
	m_pHistoryObj->GetPointFromDate(m_BeginPoint.nDay,m_BeginPoint.fPrice,Point[0]);
	m_pHistoryObj->GetPointFromDate(m_EndPoint.nDay,m_EndPoint.fPrice,Point[1]);
	rRect = CRect(Point[0],Point[1]);
}

void CDrawLineTool::MoveEnd(CSize offset)
{
	m_bMoveState = FALSE;
	m_MoveSize +=offset;
	if(m_MoveSize == CSize(0,0))return;
	//////////////////////////////////////////////////////////////////////////
	//开始点位置
	CPoint point[2];
	m_pHistoryObj->GetPointFromDate(m_BeginPoint.nDay,m_BeginPoint.fPrice,point[0]);
	m_pHistoryObj->GetPointFromDate(m_EndPoint.nDay,m_EndPoint.fPrice,point[1]);
	point[1]+= m_MoveSize;
	point[0]+= m_MoveSize;
	offset = point[1]- point[0];
	CRect rRect;
	m_pHistoryObj->GetDrawRect(rRect);
	if(point[0].x < rRect.left)
	{
		point[0].x = rRect.left;
		point[1].x = point[0].x+ offset.cx;
	}
	if(point[0].y < rRect.top)
	{
		point[0].y = rRect.top;
		point[1].y = point[0].y+offset.cy;
	}
	if(point[1].x>rRect.right)
	{
		point[1].x = rRect.right;
		point[0].x = point[1].x - offset.cx;
	}
	if(point[1].y >rRect.bottom)
	{
		point[1].y = rRect.bottom;
		point[0].y = point[1].y - offset.cy;
	}

	m_pHistoryObj->GetDateFromPoint(point[0],m_BeginPoint.nDay,m_BeginPoint.fPrice);
	m_pHistoryObj->GetDateFromPoint(point[1],m_EndPoint.nDay,m_EndPoint.fPrice);

	if((m_ThirdPoint.nDay!= 0)&&(m_ThirdPoint.fPrice > 0.00000f))
	{
		m_pHistoryObj->GetPointFromDate(m_ThirdPoint.nDay,m_ThirdPoint.fPrice,point[0]);
		point[0]+=m_MoveSize;
		m_pHistoryObj->GetDateFromPoint(point[0],m_ThirdPoint.nDay,m_ThirdPoint.fPrice);
	}

	m_MoveSize= CSize(0,0);
}
