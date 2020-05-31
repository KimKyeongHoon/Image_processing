
// CMainVDlg.h: 헤더 파일
//

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
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	void View(CString File); //이미지 출력
	void View_hist();		 //히스토그램 출력
	void Make_hist();		 //히스토그램 만들기
	void Data_set();		 //데이터 원본으로
	afx_msg void OpenFile(); //이미지 열기
	afx_msg void ViewOrg();  //이미지 보이기
	afx_msg void EQ();		 //평활화
	afx_msg void BCS();		 //기본 명암대비 스트레칭
	afx_msg void EIS();		 //엔드인 탐색
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

};
