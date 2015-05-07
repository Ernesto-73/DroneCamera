
// CameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Camera.h"
#include "CameraDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCameraDlg dialog




CCameraDlg::CCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCameraDlg::IDD, pParent)
	, m_point(0)
	, m_iSize(100)
{
	this->m_canvas = CRect(10, 10, 650, 490);
	memset(m_arrOptions, 0, NUM * sizeof(int));
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCameraDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_CAMERA, &CCameraDlg::OnBnClickedOpenCamera)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_FLIP, &CCameraDlg::OnBnClickedFlip)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_AIM, &CCameraDlg::OnBnClickedAim)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CCameraDlg message handlers

BOOL CCameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	MoveWindow(0, 0, 1000, 600);
	CenterWindow();

	GetDlgItem(IDC_AIM)->EnableWindow(FALSE);
	GetDlgItem(IDC_FLIP)->EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCameraDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCameraDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CDC xDC;
		CBitmap xBMP;
		xDC.CreateCompatibleDC(&dc);
		xBMP.CreateCompatibleBitmap(&dc, m_canvas.Width(), m_canvas.Height());
		xDC.SelectObject(&xBMP);
		Draw(&xDC);

		CDC yDC;
		CBitmap yBMP;
		CRect r;
		GetClientRect(&r);
		yDC.CreateCompatibleDC(&dc);

		yBMP.CreateCompatibleBitmap(&dc,r.Width(),r.Height());
		yDC.SelectObject(&yBMP);

		yDC.FillSolidRect(r, GetSysColor(COLOR_3DFACE)); //Get the system color of dialog background

		yDC.BitBlt(m_canvas.left, m_canvas.top, m_canvas.Width(), m_canvas.Height(), &xDC, 0, 0, SRCCOPY);
		dc.BitBlt(0, 0,r.Width(), r.Height(), &yDC, 0, 0, SRCCOPY);

		xBMP.DeleteObject();
		xDC.DeleteDC();
		yBMP.DeleteObject();
		yDC.DeleteDC();
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCameraDlg::OnBnClickedOpenCamera()
{
	// TODO: Add your control notification handler code here
	if(!m_arrOptions[IS_CAMERA_OPENED])
	{
		m_capture.open(0);
		if(!m_capture.isOpened())
			return ;
		m_arrOptions[IS_CAMERA_OPENED] = 1;
		SetTimer(1, 0, NULL);
		SetDlgItemText(IDC_OPEN_CAMERA, _T("Close Camera"));
		GetDlgItem(IDC_AIM)->EnableWindow(TRUE);
		GetDlgItem(IDC_FLIP)->EnableWindow(TRUE);
	}
	else{
		KillTimer(1);
		m_arrOptions[IS_CAMERA_OPENED] = 0;
		m_capture.release();
		SetDlgItemText(IDC_OPEN_CAMERA, _T("Open Camera"));
		GetDlgItem(IDC_AIM)->EnableWindow(FALSE);
		GetDlgItem(IDC_FLIP)->EnableWindow(FALSE);
	}
	return ;
}


void CCameraDlg::Draw(CDC * pDC)
{
	CRect r;
	GetClientRect(&r);
	pDC->FillSolidRect(&r,RGB(200,200,200));

	if(!m_arrOptions[IS_CAMERA_OPENED])
		return ;

	if(m_img.empty())
		return ;

	cv::Mat img = HanleIamge();
	HDC hDCDst = pDC->GetSafeHdc();
	cv::Size winSize(m_canvas.Width(), m_canvas.Height() );
	cv::Mat cvImgTmp(winSize, CV_8UC3);
	if (img.size() != winSize)
		cv::resize(img, cvImgTmp, winSize);
	else
		cvImgTmp = img;

	// Rotate the image
	cv::flip(cvImgTmp,cvImgTmp,0);
	// Initialize the BITMAPINFO structure

	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 24;
	bitInfo.bmiHeader.biWidth = winSize.width;
	bitInfo.bmiHeader.biHeight = winSize.height;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;

	StretchDIBits(hDCDst, 0, 0,
		winSize.width, winSize.height, 0, 0,
		winSize.width, winSize.height,
		cvImgTmp.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);

	CPen pen(PS_SOLID, 2, RGB(50, 50, 50));
	CPen *oPen = pDC->SelectObject(&pen);
	
	CBrush *brush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush *oBrush = pDC->SelectObject(brush);

	if(m_arrOptions[AIM])
	{
		if(m_arrOptions[AIMED])
		{
			pen.DeleteObject();
			pen.CreatePen(PS_SOLID, 2, RGB(0, 200, 0));
			pDC->SelectObject(&pen);
		}
		pDC->MoveTo(m_point.x, m_point.y - static_cast<int>(1.5 * m_iSize));
		pDC->LineTo(m_point.x, m_point.y + static_cast<int>(1.5 * m_iSize));
		pDC->MoveTo(m_point.x - static_cast<int>(1.5 * m_iSize), m_point.y);
		pDC->LineTo(m_point.x + static_cast<int>(1.5 * m_iSize), m_point.y);

		CRect rec_out(m_point.x - m_iSize, m_point.y - m_iSize, m_point.x + m_iSize, m_point.y + m_iSize);
		CRect rec_in(m_point.x - m_iSize / 2, m_point.y - m_iSize / 2, m_point.x + m_iSize /2, m_point.y + m_iSize / 2);
		CRect rec_heart(m_point.x - 8, m_point.y - 8, m_point.x + 8, m_point.y + 8);

		pDC->Ellipse(&rec_in);
		pDC->Ellipse(&rec_out);
		
		pen.DeleteObject();
		pen.CreatePen(PS_SOLID, 2, RGB(200, 0, 0));
		pDC->SelectObject(&pen);

		CBrush b(RGB(200, 0, 0));
		pDC->SelectObject(&b);

		pDC->Ellipse(&rec_heart);
	}

	pDC->SelectObject(&oPen);
	pDC->SelectObject(&oBrush);
	ReleaseDC( pDC );
}


BOOL CCameraDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CCameraDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	m_capture.read(m_img);
	InvalidateRect(&m_canvas);
	CDialogEx::OnTimer(nIDEvent);
}


cv::Mat CCameraDlg::HanleIamge()
{
	cv::Mat img;
	m_img.copyTo(img);
	if(m_arrOptions[GREY] == 1)
	{
		
	}
	if(m_arrOptions[FLIP] == 1)
	{
		cv::flip(img, img, 0);
	}
	return img;
}


void CCameraDlg::OnBnClickedFlip()
{
	// TODO: Add your control notification handler code here
	m_arrOptions[FLIP] = m_arrOptions[FLIP] == 0 ? 1 :0;
}


void CCameraDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(!m_arrOptions[AIM])
		return ;

	if(!m_arrOptions[AIMED])
		m_point = point;

	CPoint tl = m_canvas.TopLeft();
	CPoint br = m_canvas.BottomRight();
	if(point.x > tl.x && point.y > tl.y && point.x < br.x && point.y < br.y)
	{	
		if(!m_arrOptions[HIDE_CURSOR])
		{
			m_arrOptions[HIDE_CURSOR] = 1;
			ShowCursor(FALSE);
		}
	}
	else{
		if(m_arrOptions[HIDE_CURSOR])
		{
			m_arrOptions[HIDE_CURSOR] = 0;
			ShowCursor(TRUE);
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CCameraDlg::OnBnClickedAim()
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_arrOptions[AIM] = 1;
	GetDlgItem(IDC_AIM)->EnableWindow(FALSE);
}

void CCameraDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(!m_arrOptions[AIM])
		return ;

	CPoint tl = m_canvas.TopLeft();
	CPoint br = m_canvas.BottomRight();
	if(point.x > tl.x && point.y > tl.y && point.x < br.x && point.y < br.y)
		m_arrOptions[AIMED] = 1;

	CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL CCameraDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(!m_arrOptions[AIM])
		return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);

	CPoint tl = m_canvas.TopLeft();
	CPoint br = m_canvas.BottomRight();

	if(zDelta > 0)
		m_iSize += 10;
	else
		m_iSize -= 10;

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CCameraDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(!m_arrOptions[AIM])
		return ;

	Sleep(1000);
	GetDlgItem(IDC_AIM)->EnableWindow(TRUE);
	m_arrOptions[AIM] = 0;
	m_arrOptions[AIMED] = 0;
	m_arrOptions[HIDE_CURSOR] = 0;
	ShowCursor(TRUE);
	CDialogEx::OnLButtonUp(nFlags, point);
}
