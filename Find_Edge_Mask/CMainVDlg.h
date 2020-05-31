
// CMainVDlg.h: 헤더 파일
//
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
#pragma warning (disable:26451)

#define size 512
#define hist_size 256
#define source map[size*i+j]
#define Temp "temp.bmp"
#define uc unsigned char
#define Thresh 150
#pragma once

#define uc unsigned char
// CMainVDlg 대화 상자
class CMainVDlg : public CDialogEx
{
public:
	CMainVDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = Main_Dlg };
#endif

protected:

	HICON m_hIcon;
	CString Img_PATH=_T("");
	CImage Img_Viewer;
	CImage Img_Viewer2;
	CEdit MSE;
	int SNR;
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	float Gaussian(float sd);//Gausian Noise
	void View(CString File); //이미지 출력
	void View_hist();		 //히스토그램 출력
	void Make_hist();		 //히스토그램 만들기
	void Data_set();		 //데이터 원본으로
	void Horizontal_Mirroring();//Horizontal_Mirroring
	void Vertical_Mirroring();//Vertical_Mirroring
	void Change_MSE();		 //MSE 값 생성
	afx_msg void OpenFile(); //이미지 열기
	afx_msg void ViewOrg();  //이미지 보이기
	afx_msg void Roberts_Mask();//Roberts Mask
	afx_msg void Prewit_Mask();	//Prewit Mask
	afx_msg void Sobel_Mask();//Sobel Mask 
	afx_msg void Stochastic_Mask();//5x5 Stochastic gradient
	afx_msg void Make_Noise();//Make Gausian Noise
	afx_msg void Medain_Filter();//Median Filter
	afx_msg void Reset();//Make Original
	afx_msg void Low_Pass_Filter();//Low-pass Filter
	afx_msg void SNR_8();//Change SNR 8
	afx_msg void SNR_9();//Change SNR 9
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
