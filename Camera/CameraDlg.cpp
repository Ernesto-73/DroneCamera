
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
{
	this->m_canvas = CRect(10, 10, 650, 490);
	memset(m_arrOptions, 0, sizeof(int));
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
	m_capture.open(0);
	if(!m_capture.isOpened())
		return ;
	double rate= m_capture.get(CV_CAP_PROP_FPS);

	SetTimer(1, 0, NULL);
	return ;
}


void CCameraDlg::Draw(CDC * pDC)
{
	CRect r;
	GetClientRect(&r);
	pDC->FillSolidRect(&r,RGB(200,200,200));
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

	ReleaseDC( pDC );
}


BOOL CCameraDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
//	return CDialogEx::OnEraseBkgnd(pDC);
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
	/*
	if(!m_arrOptions[FLIP])
	{
		GetDlgItem(IDC_FLIP)->SetWindowText(_T("Unflip"));
		m_arrOptions[FLIP] = 1;
	}
	else
	{
		GetDlgItem(IDC_FLIP)->SetWindowText(_T("Flip"));
		m_arrOptions[FLIP] = 0;
	}
	*/
}
