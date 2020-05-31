
// CMainVDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "HW2_2015253058_김경훈.h"
#include "CMainVDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define size 512
#define hist_size 256
#define source map[size*i+j]
#define Temp "temp.bmp"
#define uc unsigned char

int max = 0;
int hist[hist_size];
uc map[size * size];
uc map_temp[size * size];

BITMAPFILEHEADER hf;
BITMAPINFOHEADER hlnf;
RGBQUAD hRGB[256];

CMainVDlg::CMainVDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(Main_Dlg, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainVDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(Open_btn, &CMainVDlg::OpenFile)
	ON_BN_CLICKED(Org_btn, &CMainVDlg::ViewOrg)
	ON_BN_CLICKED(eq_btn, &CMainVDlg::EQ)
	ON_BN_CLICKED(Bcon_btn, &CMainVDlg::BCS)
	ON_BN_CLICKED(EIS_btn, &CMainVDlg::EIS)
END_MESSAGE_MAP()


// CMainVDlg 메시지 처리기

BOOL CMainVDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.


	


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}
void CMainVDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMainVDlg::OnQueryDragIcon(){return static_cast<HCURSOR>(m_hIcon);}

void CMainVDlg::OpenFile() //파일 불러오기
{
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif |모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal())
	{
		GetDlgItem(Org_btn)->ShowWindow(SW_SHOW);
		GetDlgItem(eq_btn)->ShowWindow(SW_SHOW);
		GetDlgItem(Bcon_btn)->ShowWindow(SW_SHOW);
		GetDlgItem(EIS_btn)->ShowWindow(SW_SHOW);
		Img_PATH = dlg.GetPathName();
		MessageBox(Img_PATH);
		char* path = (LPSTR)(LPCTSTR)Img_PATH;
		FILE* in;
		fopen_s(&in, path, "rb");

		fread(&hf, sizeof(BITMAPFILEHEADER), 1, in); //14바이트
		fread(&hlnf, sizeof(BITMAPINFOHEADER), 1, in); //40바이트
		fread(&hRGB, sizeof(RGBQUAD), 256, in);
		fread(&map, sizeof(uc), size * size, in);


		fclose(in);

		memcpy(map_temp, map, size * size);


	}
}

//View_hist() 히스토그램 표시하기
void CMainVDlg::View_hist() 
{
	CWnd* pWnd = (CWnd*)GetDlgItem(hist_con);
	CDC* dc = pWnd->GetDC();
	CStatic* staticSize = (CStatic*)GetDlgItem(hist_con);
	CRect rect;

	staticSize->GetClientRect(rect);

	Img_Viewer2.Destroy(); 
	Img_Viewer2.Load(Temp);
	Img_Viewer2.Draw(dc->m_hDC, 0, 0, Img_Viewer2.GetWidth(), Img_Viewer2.GetHeight());
}

//이미지 표시하기
void CMainVDlg::View(CString File) 
{
	CWnd* pWnd = (CWnd*)GetDlgItem(Pic_con);
	CDC* dc = pWnd->GetDC();
	CStatic* staticSize = (CStatic*)GetDlgItem(Pic_con);
	CRect rect;

	staticSize->GetClientRect(rect);

	Img_Viewer.Destroy(); 
	Img_Viewer.Load(File);
	Img_Viewer.Draw(dc->m_hDC, 0, 0, Img_Viewer.GetWidth(), Img_Viewer.GetHeight());
	Make_hist();

	memcpy(map_temp, map, size * size);
}

//ViewOrg이미지보기
void CMainVDlg::ViewOrg() 
{
	if (Img_PATH == _T(""))return;
	memcpy(map_temp, map, size * size);

	View(Img_PATH);

}

//EQ 평활화
void CMainVDlg::EQ() 
{
	FILE* out;
	fopen_s(&out, Temp, "wb");

	uc hist_s[hist_size];
	memset(hist_s, 0, hist_size);


	Data_set();
	const double max_bright = 255;
	const double pixel = (double)(size * size);
	int sum = 0;
	
	for (int i = 0; i < hist_size; i++)hist_s[i] = (int)(round(((double)(sum += hist[i]) / pixel) * max_bright));
	for (int i = 0; i < size; i++)for (int j = 0; j < size; j++)map_temp[i*size+j] = hist_s[source];

	hlnf.biWidth = size;
	hlnf.biHeight = size;

	fwrite(&hf, sizeof(uc), sizeof(BITMAPFILEHEADER), out);
	fwrite(&hlnf, sizeof(uc), sizeof(BITMAPINFOHEADER), out);
	fwrite(hRGB, sizeof(RGBQUAD), 256, out);
	fwrite(map_temp, sizeof(uc), size * size, out);

	fclose(out);
	View(Temp);

}

//Make_hist 히스토그램 만든후 출력
void CMainVDlg::Make_hist()
{
	FILE* out;
	fopen_s(&out, Temp, "wb");
	
	uc histogram[hist_size * hist_size];
	memset(histogram, 0, hist_size * hist_size);
	memset(hist, 0, hist_size * sizeof(int));
	max = 0;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			hist[map_temp[i*size+j]]++;
			if (max < hist[map_temp[i * size + j]])max = hist[map_temp[i * size + j]];
		}

	for (int i = 0; i < hist_size; i++)
		for (int j = 0; j < hist_size; j++)
			histogram[j * hist_size + i] = j < (int)(hist[i]*(255.0/max) )? 0 : 255;
	
	hlnf.biWidth = hist_size;
	hlnf.biHeight = hist_size;

	fwrite(&hf, sizeof(uc), sizeof(BITMAPFILEHEADER), out);
	fwrite(&hlnf, sizeof(uc), sizeof(BITMAPINFOHEADER), out);
	fwrite(hRGB, sizeof(RGBQUAD), 256, out);
	fwrite(histogram, sizeof(uc), hist_size * hist_size, out);
	fclose(out);
	View_hist();
	memcpy(map_temp, map, size * size);
}

//Data_set 데이터 초기화
void CMainVDlg::Data_set()
{
	memcpy(map_temp, map, size * size);
	memset(hist, 0, hist_size * sizeof(int));
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			hist[map_temp[i * size + j]]++;
			if (max < hist[map_temp[i * size + j]])max = hist[map_temp[i * size + j]];
		}

}

//BCS 기본 명암대비 스트레칭
void CMainVDlg::BCS()
{
	FILE* out;
	fopen_s(&out, Temp, "wb");

	uc hist_s[hist_size];
	memset(hist_s, 0, hist_size);


	const double max_bright = 255;
	const double pixel = (double)(size * size);
	int sum = 0;
	
	Data_set();
	int max = 0;
	int min = hist_size - 1;
	
	for (int i = 1; i < hist_size; i++)
	{
		if (max < i && hist[i] > 0)max = i;
		if (min > i && hist[i] > 0)min = i;
	}
	for (int i = 0; i < size; i++)for (int j = 0; j < size; j++)
		map_temp[i*size+j] = (map_temp[i * size + j] - min) * 255 / (max - min);



	hlnf.biWidth = size;
	hlnf.biHeight = size;

	fwrite(&hf, sizeof(uc), sizeof(BITMAPFILEHEADER), out);
	fwrite(&hlnf, sizeof(uc), sizeof(BITMAPINFOHEADER), out);
	fwrite(hRGB, sizeof(RGBQUAD), 256, out);
	fwrite(map_temp, sizeof(uc), size * size, out);

	fclose(out);
	View(Temp);

}

//EIS 엔드인 탐색
void CMainVDlg::EIS() 
{
	FILE* out;
	fopen_s(&out, Temp, "wb");

	uc hist_s[hist_size];
	memset(hist_s, 0, hist_size);

	const double max_bright = 255;
	const double pixel = (double)(size * size);
	Data_set();
	int max = 190;
	int min = 50;

	for (int i = 0; i < size; i++)for (int j = 0; j < size; j++)
	{
		if (map_temp[i*size+j] < min)map_temp[i * size + j] = 0;
		else if (map_temp[i * size + j] > max)map_temp[i * size + j] = 255;
		else map_temp[i * size + j] = (map_temp[i * size + j] - min) * 255 / (max - min);
	}

	hlnf.biWidth = size;
	hlnf.biHeight = size;

	fwrite(&hf, sizeof(uc), sizeof(BITMAPFILEHEADER), out);
	fwrite(&hlnf, sizeof(uc), sizeof(BITMAPINFOHEADER), out);
	fwrite(hRGB, sizeof(RGBQUAD), 256, out);
	fwrite(map_temp, sizeof(uc), size * size, out);

	fclose(out);
	View(Temp);
}
