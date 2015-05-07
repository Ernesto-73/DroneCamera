
// CameraDlg.h : header file
//

#pragma once
#include "atltypes.h"
#include <opencv2/opencv.hpp>
#include "opencv2\highgui\highgui.hpp"

enum{GREY = 0, FLIP, AIM, AIMED, HIDE_CURSOR, IS_CAMERA_OPENED, NUM};
// CCameraDlg dialog
class CCameraDlg : public CDialogEx
{
// Construction
public:
	CCameraDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CAMERA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenCamera();
	void Draw(CDC * pDC);
	CRect m_canvas;
	cv::Mat m_img;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	cv::VideoCapture m_capture;
	int m_arrOptions[NUM];
	cv::Mat HanleIamge();
	afx_msg void OnBnClickedFlip();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedAim();
	CPoint m_point;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	int m_iSize;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
