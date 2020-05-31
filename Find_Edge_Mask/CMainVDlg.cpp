
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


int hist[hist_size];
uc map[size * size];
uc map_temp[size * size];
uc map_copy[size * size];
uc map_Original[size * size];

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
	DDX_Control(pDX, IDC_EDIT1, MSE);
}

BEGIN_MESSAGE_MAP(CMainVDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(Open_btn, &CMainVDlg::OpenFile)
	ON_BN_CLICKED(Func1, &CMainVDlg::ViewOrg)
	ON_BN_CLICKED(Func2, &CMainVDlg::Roberts_Mask)
	ON_BN_CLICKED(Func3, &CMainVDlg::Prewit_Mask)
	ON_BN_CLICKED(Func4, &CMainVDlg::Sobel_Mask)
	ON_BN_CLICKED(Func5, &CMainVDlg::Stochastic_Mask)
	ON_BN_CLICKED(IDC_BUTTON1, &CMainVDlg::Make_Noise)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainVDlg::Medain_Filter)
	ON_BN_CLICKED(IDC_BUTTON3, &CMainVDlg::Reset)
	ON_BN_CLICKED(IDC_BUTTON4, &CMainVDlg::Low_Pass_Filter)
	ON_BN_CLICKED(SNR1, &CMainVDlg::SNR_8)
	ON_BN_CLICKED(SNR2, &CMainVDlg::SNR_9)
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
HCURSOR CMainVDlg::OnQueryDragIcon(){return static_cast<HCURSOR>(m_hIcon);}
//파일 불러오기
void CMainVDlg::OpenFile() //파일 불러오기
{
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.RAW*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.raw;*.bmp;*.jpg;*.gif |모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal())
	{
		GetDlgItem(Func1)->ShowWindow(SW_SHOW);
		GetDlgItem(Func2)->ShowWindow(SW_SHOW);
		GetDlgItem(Func3)->ShowWindow(SW_SHOW);
		GetDlgItem(Func4)->ShowWindow(SW_SHOW);
		GetDlgItem(Func5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON4)->ShowWindow(SW_SHOW);
		
		Img_PATH = dlg.GetPathName();
		MessageBox(Img_PATH);
		char* path = (LPSTR)(LPCTSTR)Img_PATH;
		FILE* in;
		fopen_s(&in, path, "rb");
		if(Img_PATH.Find(".raw") != std::string::npos)
		{ 
			hf.bfType = 0x4d42;
			hf.bfSize = 0x00040436;
			hf.bfReserved1 = 0x0000;
			hf.bfReserved2 = 0x0000;
			hf.bfOffBits = 0x00000436;
			hlnf.biSize = 0x00000028;
			hlnf.biWidth = 0x00000200;
			hlnf.biHeight = 0x00000200;
			hlnf.biPlanes = 0x0001;
			hlnf.biBitCount = 0x0008;
			hlnf.biCompression = 0x00000000;
			hlnf.biSizeImage = 0x00040000;
			hlnf.biXPelsPerMeter = 0x00000200;
			hlnf.biYPelsPerMeter = 0x00000200;
			hlnf.biClrUsed = 0x00000100;
			hlnf.biClrImportant = 0x00000000;
			for (int i = 0; i < 256; i++)hRGB[i].rgbRed = hRGB[i].rgbGreen = hRGB[i].rgbBlue = i;
			fread(map, sizeof(uc),size * size, in);
			Horizontal_Mirroring();
			Vertical_Mirroring();
			fclose(in);
		}
		else
		{

			fread(&hf, sizeof(BITMAPFILEHEADER), 1, in); //14바이트
			fread(&hlnf, sizeof(BITMAPINFOHEADER), 1, in); //40바이트
			fread(&hRGB, sizeof(RGBQUAD), 256, in);
			fread(&map, sizeof(uc), size * size, in);

			fclose(in);
		}
		memcpy(map_temp, map, size * size);
		memcpy(map_Original, map, size * size);

	}
}
//View_hist() 히스토그램 표시하기
void CMainVDlg::View_hist() //View_hist() 히스토그램 표시하기
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
//Make_hist 히스토그램 만든후 출력
void CMainVDlg::Make_hist()
{
	uc* histogram;
	FILE* out;
	fopen_s(&out, Temp, "wb");

	histogram= new uc[hist_size * hist_size];
	memset(histogram, 0, hist_size * hist_size);
	memset(hist, 0, hist_size * sizeof(int));
	int max = 0;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			hist[map_temp[i * size + j]]++;
			if (max < hist[map_temp[i * size + j]])max = hist[map_temp[i * size + j]];
		}

	for (int i = 0; i < hist_size; i++)
		for (int j = 0; j < hist_size; j++)
			histogram[j * hist_size + i] = j < (int)(hist[i] * (255.0 / max)) ? 0 : 255;

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
	FILE* out;
	fopen_s(&out, Temp, "wb");

	uc hist_s[hist_size];
	memset(hist_s, 0, hist_size);


	Data_set();

	hlnf.biWidth = size;
	hlnf.biHeight = size;

	fwrite(&hf, sizeof(uc), sizeof(BITMAPFILEHEADER), out);
	fwrite(&hlnf, sizeof(uc), sizeof(BITMAPINFOHEADER), out);
	fwrite(hRGB, sizeof(RGBQUAD), 256, out);
	fwrite(map_temp, sizeof(uc), size * size, out);

	fclose(out);
	View(Temp);

}
//Data_set 데이터 초기화
void CMainVDlg::Data_set()//Data_set 데이터 초기화
{
	memcpy(map_temp, map, size * size);	//원본 map값 복사해서 사용
	memset(hist, 0, hist_size * sizeof(int));
}
//Roberts Mask
void CMainVDlg::Roberts_Mask()//Roberts Mask 
{
	FILE* out;
	fopen_s(&out, Temp, "wb");

	const int Gx[3][3] = { 0,0,-1,0,1,0,0,0,0 };
	const int Gy[3][3] = { -1,0,0,0,1,0,0,0,0};

	Data_set();
	memcpy(map_copy, map_temp, size * size);

	for (int i = 1; i < size - 1; i++)for (int j = 1; j < size - 1; j++)
	{
		double gx = 0, gy = 0;
		for (int n = 0; n < 3; n++)
		{
			for (int m = 0; m < 3; m++)
			{
				gx += Gx[n][m] * map_copy[(i + n - 1) * size + (j + m - 1)];
				gy += Gy[n][m] * map_copy[(i + n - 1) * size + (j + m - 1)];
			}
		}

		double result = (double)(sqrt(gx*gx+gy*gy));
	//	result = (result > Thresh ? 255 : 0);
		map_temp[i * size + j] = (uc)result;
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
//Prewit Mask
void CMainVDlg::Prewit_Mask()//Prewit Mask
{
	FILE* out;
	fopen_s(&out, Temp, "wb");
	const int Gx[3][3] = { -1,0,1,-1,0,1,-1,0,1 };
	const int Gy[3][3] = { 1,1,1,0,0,0,-1,-1,-1 };

	Data_set();
	memcpy(map_copy, map_temp, size * size);

	for (int i = 1; i < size - 1; i++)for (int j = 1; j < size - 1; j++)
	{
		double gx = 0, gy = 0;
		for (int n = 0; n < 3; n++)
		{
			for (int m = 0; m < 3; m++)
			{
				gx += Gx[n][m] * map_copy[(i + n - 1) * size + (j + m - 1)];
				gy += Gy[n][m] * map_copy[(i + n - 1) * size + (j + m - 1)];
			}
		}
		//gx /= 3, gy /= 3;
		double result = (double)sqrt(gx * gx + gy * gy);
		result = (result > Thresh ? 255 : 0);
		map_temp[i * size + j] = (uc)result;
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
//Sobel Mask
void CMainVDlg::Sobel_Mask()//Sobel Mask 
{
	FILE* out;
	fopen_s(&out, Temp, "wb");

	const int Gx[3][3] = { -1,0,1,-2,0,2,-1,0,1 };
	const int Gy[3][3] = { 1,2,1,0,0,0,-1,-2,-1 };

	Data_set();
	memcpy(map_copy, map_temp, size * size);

	for (int i = 1; i < size - 1; i++)for (int j = 1; j < size - 1; j++)
	{
		double gx = 0, gy = 0;
		for (int n = 0; n < 3; n++)
		{
			for (int m = 0; m < 3; m++)
			{
				gx += Gx[n][m] * map_copy[(i + n - 1) * size + (j + m - 1)];
				gy += Gy[n][m] * map_copy[(i + n - 1) * size + (j + m - 1)];
			}
		}
		//gx /= 4, gy /= 4;
		double result = (double)sqrt(gx * gx + gy * gy);
		result = (result > Thresh ? 255 : 0);
		map_temp[i * size + j] = (uc)result;
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
//5x5 Stochastic gradient
void CMainVDlg::Stochastic_Mask()//5x5 Stochastic gradient
{
	FILE* out;
	fopen_s(&out, Temp, "wb");

	const double Gx[5][5] =
	{
		{ -0.267, -0.364, 0, 0.364, 0.267 },
		{ -0.373, -0.562, 0, 0.562, 0.373 },
		{ -0.463, -1.000, 0, 1.000, 0.463 },
		{ -0.373, -0.562, 0, 0.562, 0.373 },
		{ -0.267, -0.364, 0, 0.364, 0.267 }
	};
	const double Gy[5][5] =
	{
		{ -0.267, -0.373, -0.463, -0.373, -0.267 },
		{ -0.364, -0.562, -1.000, -0.562, -0.364 },
		{ 0,0,0,0,0 },
		{ 0.364, 0.562, 1.000, 0.562, 0.364 },
		{ 0.267, 0.373, 0.463, 0.373, 0.267 },
	};

	Data_set();
	memcpy(map_copy, map_temp, size * size);

	for (int i = 1; i < size - 1; i++)for (int j = 1; j < size - 1; j++)
	{
		double gx = 0, gy = 0;
		for (int n = 0; n < 5; n++)
		{
			for (int m = 0; m < 5; m++)
			{
				gx += Gx[n][m] * map_copy[(i + n - 1) * size + (j + m - 1)];
				gy += Gy[n][m] * map_copy[(i + n - 1) * size + (j + m - 1)];
			}
		}

		double result = (double)sqrt(gx * gx + gy * gy);
		result = (result > Thresh ? 255 : 0);
		map_temp[i * size + j] = (uc)result;
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
//Make Gausian Noise
void CMainVDlg::Make_Noise()//Make Gausian Noise
{
	Data_set();
	float EX_2 = 0, EX = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			EX_2 += map[i * size + j] * map[i * size + j];
			EX += map[i * size + j];
		}
	}
	EX_2 /= (size * size);
	EX = (EX / (size * size)) * (EX / (size * size));
	float variance = EX_2 - EX;
	float sigma = (float)sqrt(variance / pow(10.0, ((double)SNR / 10)));
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			float s = map[i * size + j] + Gaussian(sigma);
			map[i * size + j] = (uc)(s > 255 ? 255 : (s < 0 ? 0 : s));
		}
	}
	if (IsDlgButtonChecked(Func1))ViewOrg();
	else if (IsDlgButtonChecked(Func2))Roberts_Mask();
	else if (IsDlgButtonChecked(Func3))Prewit_Mask();
	else if (IsDlgButtonChecked(Func4))Sobel_Mask();
	else if (IsDlgButtonChecked(Func5))Stochastic_Mask();
}
//Return Gausian Noise
float CMainVDlg::Gaussian(float sd)//Return Gausian Noise
{
	static int ready = 0;
	static float gstore;
	float v1, v2, r, fac, gaus;
	int r1, r2;

	if (ready == 0) {
		do {
			r1 = rand();
			r2 = rand();
			v1 = (float)(2. * ((float)r1 / (float)RAND_MAX - 0.5));
			v2 = (float)(2. * ((float)r2 / (float)RAND_MAX - 0.5));
			r = v1 * v1 + v2 * v2;
		} while (r > 1.0);
		fac = (float)sqrt((double)(-2 * log(r) / r));
		gstore = v1 * fac;
		gaus = v2 * fac;
		ready = 1;
	}
	else {
		ready = 0;
		gaus = gstore;
	}
	return (gaus * sd);
}
//Median Filter
void CMainVDlg::Medain_Filter()//Median Filter
{
	FILE* out;
	fopen_s(&out, Temp, "wb");

	Data_set();
	memcpy(map_copy, map_temp, size * size);
	
	for (int i = 1; i < size - 1; i++)for (int j = 1; j < size - 1; j++)
	{
		double* arr = new double[9];
		int nn = 0;
		for (int n = 0; n < 3; n++)
		{
			for (int m = 0; m < 3; m++)
			{
				arr[nn++] = map_copy[(i + n - 1) * size + (j + m - 1)];
			}
		}
		sort(arr, arr + 9);
		map[i * size + j] = (uc)arr[4];
	}

	hlnf.biWidth = size;
	hlnf.biHeight = size;

	fwrite(&hf, sizeof(uc), sizeof(BITMAPFILEHEADER), out);
	fwrite(&hlnf, sizeof(uc), sizeof(BITMAPINFOHEADER), out);
	fwrite(hRGB, sizeof(RGBQUAD), 256, out);
	fwrite(map, sizeof(uc), size * size, out);

	fclose(out);
	View(Temp);

	if (IsDlgButtonChecked(Func1))ViewOrg();
	else if (IsDlgButtonChecked(Func2))Roberts_Mask();
	else if (IsDlgButtonChecked(Func3))Prewit_Mask();
	else if (IsDlgButtonChecked(Func4))Sobel_Mask();
	else if (IsDlgButtonChecked(Func5))Stochastic_Mask();

	Change_MSE();
}
//Make Original
void CMainVDlg::Reset()//Make Original
{
	memcpy(map,map_Original, size * size);
	if (IsDlgButtonChecked(Func1))ViewOrg();
	else if (IsDlgButtonChecked(Func2))Roberts_Mask();
	else if (IsDlgButtonChecked(Func3))Prewit_Mask();
	else if (IsDlgButtonChecked(Func4))Sobel_Mask();
	else if (IsDlgButtonChecked(Func5))Stochastic_Mask();
}
//Low-pass Filter
void CMainVDlg::Low_Pass_Filter()//Low-pass Filter
{
	FILE* out;
	fopen_s(&out, Temp, "wb");
	Data_set();
	for (int i = 1; i < size - 1; i++)for (int j = 1; j < size - 1; j++)
	{
		double gx = 0;
		for (int n = 0; n < 3; n++)
		{
			for (int m = 0; m < 3; m++)
			{
				gx += map_temp[(i + n - 1) * size + (j + m - 1)];
				}
		}
		gx /= 9;
		map[i * size + j] = (uc)gx;
	}

	hlnf.biWidth = size;
	hlnf.biHeight = size;

	fwrite(&hf, sizeof(uc), sizeof(BITMAPFILEHEADER), out);
	fwrite(&hlnf, sizeof(uc), sizeof(BITMAPINFOHEADER), out);
	fwrite(hRGB, sizeof(RGBQUAD), 256, out);
	fwrite(map, sizeof(uc), size * size, out);

	fclose(out);
	View(Temp);

	if (IsDlgButtonChecked(Func1))ViewOrg();
	else if (IsDlgButtonChecked(Func2))Roberts_Mask();
	else if (IsDlgButtonChecked(Func3))Prewit_Mask();
	else if (IsDlgButtonChecked(Func4))Sobel_Mask();
	else if (IsDlgButtonChecked(Func5))Stochastic_Mask();

	Change_MSE();
}
//Horizontal_Mirroring
void CMainVDlg::Horizontal_Mirroring()//Horizontal_Mirroring
{
	Data_set();
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			map[i * size + j] = map_temp[i * size + (size - 1 - j)];
		}
	}
}
//Vertical_Mirroring
void CMainVDlg::Vertical_Mirroring()//Vertical_Mirroring
{
	Data_set();
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			map[i * size + j] = map_temp[(size-1-i) * size + j];
		}
	}

}
//Change SNR 8
void CMainVDlg::SNR_8()//Change SNR 9
{
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
	SNR = 8;
}
//Change SNR 9
void CMainVDlg::SNR_9()//Change SNR 9
{
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
	SNR = 9;
}
//MSE 값 생성
void CMainVDlg::Change_MSE()//MSE 값 생성
{
	double mse = 0;
	//map = now img, map_Orignal = org img
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			mse += (double)((map_Original[i * size + j] - map[i * size + j]) * (map_Original[i * size + j] - map[i * size + j]));
		}
	}
	mse /= size * size;
	CString temp;
	temp.Format("%lf", mse);
	MSE.SetWindowTextA(temp);
}