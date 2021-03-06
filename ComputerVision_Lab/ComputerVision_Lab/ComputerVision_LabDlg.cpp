
// ComputerVision_LabDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ComputerVision_Lab.h"
#include "ComputerVision_LabDlg.h"
#include "afxdialogex.h"
#include "DebugManager.h"
#include <vector>
using std::vector;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CComputerVisionLabDlg 对话框



CComputerVisionLabDlg::CComputerVisionLabDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMPUTERVISION_LAB_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CComputerVisionLabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPENFILE1, OpenFile1);
	DDX_Control(pDX, IDC_OPENFILE2, OpenFile2);
	DDX_Control(pDX, IDC_FILEPATH1, FilePath1);
	DDX_Control(pDX, IDC_FILEPATH2, FilePath2);
	DDX_Control(pDX, IDC_STARTPROCESS, StartProcess);
	DDX_Control(pDX, IDC_PICTURE1, Picture1Area);
	DDX_Control(pDX, IDC_PICTURE2, Picture2Area);
	DDX_Control(pDX, IDC_DEBUGINFO, DebugInfo);
	DDX_Control(pDX, IDC_ALGORITHMTAB, AlgorithmTab);
	DDX_Control(pDX, IDC_PICTURE1RESULT, Picture1Result);
	DDX_Control(pDX, IDC_PICTURE2RESULT, Picture2Result);
}

BEGIN_MESSAGE_MAP(CComputerVisionLabDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPENFILE1, &CComputerVisionLabDlg::OnBnClickedOpenfile1)
	ON_BN_CLICKED(IDC_OPENFILE2, &CComputerVisionLabDlg::OnBnClickedOpenfile2)
	ON_BN_CLICKED(IDC_STARTPROCESS, &CComputerVisionLabDlg::OnBnClickedStartprocess)
	ON_NOTIFY(TCN_SELCHANGE, IDC_ALGORITHMTAB, &CComputerVisionLabDlg::OnTcnSelchangeAlgorithmtab)
END_MESSAGE_MAP()


// CComputerVisionLabDlg 消息处理程序

BOOL CComputerVisionLabDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	DebugManager::createInstance(this);

	/**************************Tab相关**************************/
	//各个算法的Tab项
	AlgorithmTab.InsertItem(0, _T("SIFT"));
	AlgorithmTab.InsertItem(1, _T("SURF"));
	AlgorithmTab.InsertItem(2, _T("ORB(FAST+BRIEF)"));
	AlgorithmTab.InsertItem(3, _T("Random Ferns"));

	//关联主对话框和各个子Tab页对话框
	SIFTDlg.Create(IDD_SIFTDIALOG, GetDlgItem(IDD_COMPUTERVISION_LAB_DIALOG));
	SURFDlg.Create(IDD_SURFDIALOG, GetDlgItem(IDD_COMPUTERVISION_LAB_DIALOG));
	ORBDlg.Create(IDD_ORBDIALOG, GetDlgItem(IDD_COMPUTERVISION_LAB_DIALOG));
	FERNSDlg.Create(IDD_FERNSDIALOG, GetDlgItem(IDD_COMPUTERVISION_LAB_DIALOG));

	//创建显示区域
	CRect tabArea;
	AlgorithmTab.GetClientRect(&tabArea);

	//调整对话框的位置
	tabArea.top += 105;
	tabArea.bottom += 80;
	tabArea.left += 59;
	tabArea.right += 55;

	//移动子Tab页到指定的地方
	SIFTDlg.MoveWindow(&tabArea);
	SURFDlg.MoveWindow(&tabArea);
	ORBDlg.MoveWindow(&tabArea);
	FERNSDlg.MoveWindow(&tabArea);


	//分别设置隐藏和显示 
	SIFTDlg.ShowWindow(true);
	SURFDlg.ShowWindow(false);
	ORBDlg.ShowWindow(false);
	FERNSDlg.ShowWindow(false);

	//设置默认的选项卡
	AlgorithmTab.SetCurSel(0);
	/**************************Tab相关**************************/


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CComputerVisionLabDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		showPicture1();
		showPicture2();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CComputerVisionLabDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//读取图片1
void CComputerVisionLabDlg::OnBnClickedOpenfile1()
{
	TCHAR szFilter[] = _T("JPEG(*jpg)|*.jpg|*.bmp|*.png|TIFF(*.tif)|*.tif|All Files （*.*）|*.*||");
	CString filePath("");

	CFileDialog fileOpenDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	//如果用户选择了OK按钮
	if (fileOpenDialog.DoModal() == IDOK)
	{
		VERIFY(filePath = fileOpenDialog.GetPathName());
		CString strFilePath(filePath);
		Picture1_Path = filePath;
		FilePath1.SetWindowTextW(strFilePath);

		/*if (Picture1_Data != NULL)
		{
			delete Picture1_Data;
		}*/
		Picture1_Data.Load(strFilePath);

		/*if (handledImage != NULL)
		{
			delete handledImage;
		}
		handledImage = new CImage();
		handledImage->Load(strFilePath);*/

		//重新绘制
		this->Invalidate();
	}
}

//读取图片2
void CComputerVisionLabDlg::OnBnClickedOpenfile2()
{
	TCHAR szFilter[] = _T("JPEG(*jpg)|*.jpg|*.bmp|*.png|TIFF(*.tif)|*.tif|All Files （*.*）|*.*||");
	CString filePath("");

	CFileDialog fileOpenDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	//如果用户选择了OK按钮
	if (fileOpenDialog.DoModal() == IDOK)
	{
		VERIFY(filePath = fileOpenDialog.GetPathName());
		CString strFilePath(filePath);
		Picture2_Path = filePath;
		FilePath2.SetWindowTextW(strFilePath);

		/*if (Picture2_Data != NULL)
		{
			delete Picture2_Data;
		}*/
		Picture2_Data.Load(strFilePath);

		/*if (handledImage != NULL)
		{
			delete handledImage;
		}
		handledImage = new CImage();
		handledImage->Load(strFilePath);*/

		//重新绘制
		this->Invalidate();
	}
}

//更新图片1区域的内容
void CComputerVisionLabDlg::showPicture1()
{
	if (Picture1_Data != NULL)
	{
		int height;
		int width;

		CRect WorkSpaceRect;
		CRect ShowImageRect;

		//记录图片本身的长和宽
		height = Picture1_Data.GetHeight();
		width = Picture1_Data.GetWidth();

		Picture1Area.GetClientRect(&WorkSpaceRect); //根据GUI中的工作区生成一个矩形对象，左上角为（0，0）
		CDC *pDC = Picture1Area.GetDC();	//设备上下文（Device Contex）->指向图片数据？
		SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);//设置图片拉伸的模式，设置为HalfTone，这个模式下，应用程序必须调用Win32函数 SetBrushOrgEx 设置画笔原点

		if (width <= WorkSpaceRect.Width() && height <= WorkSpaceRect.Width()) //图片完全可以在工作区中以原来的大小放下
		{
			ShowImageRect = CRect(WorkSpaceRect.TopLeft(), CSize(width, height)); //创建另一个矩形，左上角点和工作区一致，使用CSize传入矩形大小
			Picture1_Data.StretchBlt(pDC->m_hDC, ShowImageRect, SRCCOPY);//使用SRCCOPY直接将原图片数据传递至展示图片的矩形（ShowImageRect）
		}
		else //图片在工作区中不能以原来的大小放下
		{
			float xScale = (float)WorkSpaceRect.Width() / (float)width; //记录原工作区矩形和图片的长度比例
			float yScale = (float)WorkSpaceRect.Height() / (float)height; //记录原工作区矩形和图片的宽度比例
			float ScaleIndex = (xScale <= yScale ? xScale : yScale); //选出较小的比例

			ShowImageRect = CRect(WorkSpaceRect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex)); //创建一个由原图片按照刚刚获取的较小的比例缩放的，保证能整个放入工作区的矩形
			Picture1_Data.StretchBlt(pDC->m_hDC, ShowImageRect, SRCCOPY);//使用SRCCOPY直接将原图片数据传递至展示图片的矩形（ShowImageRect）
		}
		ReleaseDC(pDC); //将上下文释放
	}
}

//更新图片2区域的内容
void CComputerVisionLabDlg::showPicture2()
{
	if (Picture2_Data != NULL)
	{
		int height;
		int width;

		CRect WorkSpaceRect;
		CRect ShowImageRect;

		//记录图片本身的长和宽
		height = Picture2_Data.GetHeight();
		width = Picture2_Data.GetWidth();

		Picture2Area.GetClientRect(&WorkSpaceRect); //根据GUI中的工作区生成一个矩形对象，左上角为（0，0）
		CDC *pDC = Picture2Area.GetDC();	//设备上下文（Device Contex）->指向图片数据？
		SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);//设置图片拉伸的模式，设置为HalfTone，这个模式下，应用程序必须调用Win32函数 SetBrushOrgEx 设置画笔原点

		if (width <= WorkSpaceRect.Width() && height <= WorkSpaceRect.Width()) //图片完全可以在工作区中以原来的大小放下
		{
			ShowImageRect = CRect(WorkSpaceRect.TopLeft(), CSize(width, height)); //创建另一个矩形，左上角点和工作区一致，使用CSize传入矩形大小
			Picture2_Data.StretchBlt(pDC->m_hDC, ShowImageRect, SRCCOPY);//使用SRCCOPY直接将原图片数据传递至展示图片的矩形（ShowImageRect）
		}
		else //图片在工作区中不能以原来的大小放下
		{
			float xScale = (float)WorkSpaceRect.Width() / (float)width; //记录原工作区矩形和图片的长度比例
			float yScale = (float)WorkSpaceRect.Height() / (float)height; //记录原工作区矩形和图片的宽度比例
			float ScaleIndex = (xScale <= yScale ? xScale : yScale); //选出较小的比例

			ShowImageRect = CRect(WorkSpaceRect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex)); //创建一个由原图片按照刚刚获取的较小的比例缩放的，保证能整个放入工作区的矩形
			Picture2_Data.StretchBlt(pDC->m_hDC, ShowImageRect, SRCCOPY);//使用SRCCOPY直接将原图片数据传递至展示图片的矩形（ShowImageRect）
		}
		ReleaseDC(pDC); //将上下文释放
	}
}

//更新图片2结果区域的内容
void CComputerVisionLabDlg::showPicture2Result()
{
	if (Picture2_Result_Data != NULL)
	{
		int height;
		int width;

		CRect WorkSpaceRect;
		CRect ShowImageRect;

		//记录图片本身的长和宽
		height = Picture2_Result_Data.GetHeight();
		width = Picture2_Result_Data.GetWidth();

		Picture2Result.GetClientRect(&WorkSpaceRect); //根据GUI中的工作区生成一个矩形对象，左上角为（0，0）
		CDC *pDC = Picture2Result.GetDC();	//设备上下文（Device Contex）->指向图片数据？
		SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);//设置图片拉伸的模式，设置为HalfTone，这个模式下，应用程序必须调用Win32函数 SetBrushOrgEx 设置画笔原点

		if (width <= WorkSpaceRect.Width() && height <= WorkSpaceRect.Width()) //图片完全可以在工作区中以原来的大小放下
		{
			ShowImageRect = CRect(WorkSpaceRect.TopLeft(), CSize(width, height)); //创建另一个矩形，左上角点和工作区一致，使用CSize传入矩形大小
			Picture2_Result_Data.StretchBlt(pDC->m_hDC, ShowImageRect, SRCCOPY);//使用SRCCOPY直接将原图片数据传递至展示图片的矩形（ShowImageRect）
		}
		else //图片在工作区中不能以原来的大小放下
		{
			float xScale = (float)WorkSpaceRect.Width() / (float)width; //记录原工作区矩形和图片的长度比例
			float yScale = (float)WorkSpaceRect.Height() / (float)height; //记录原工作区矩形和图片的宽度比例
			float ScaleIndex = (xScale <= yScale ? xScale : yScale); //选出较小的比例

			ShowImageRect = CRect(WorkSpaceRect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex)); //创建一个由原图片按照刚刚获取的较小的比例缩放的，保证能整个放入工作区的矩形
			Picture2_Result_Data.StretchBlt(pDC->m_hDC, ShowImageRect, SRCCOPY);//使用SRCCOPY直接将原图片数据传递至展示图片的矩形（ShowImageRect）
		}
		ReleaseDC(pDC); //将上下文释放
	}
}

//更新图片1结果区域的内容
void CComputerVisionLabDlg::showPicture1Result()
{
	if (Picture1_Result_Data != NULL)
	{
		int height;
		int width;

		CRect WorkSpaceRect;
		CRect ShowImageRect;

		//记录图片本身的长和宽
		height = Picture1_Result_Data.GetHeight();
		width = Picture1_Result_Data.GetWidth();

		Picture1Result.GetClientRect(&WorkSpaceRect); //根据GUI中的工作区生成一个矩形对象，左上角为（0，0）
		CDC *pDC = Picture1Result.GetDC();	//设备上下文（Device Contex）->指向图片数据？
		SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);//设置图片拉伸的模式，设置为HalfTone，这个模式下，应用程序必须调用Win32函数 SetBrushOrgEx 设置画笔原点

		if (width <= WorkSpaceRect.Width() && height <= WorkSpaceRect.Width()) //图片完全可以在工作区中以原来的大小放下
		{
			ShowImageRect = CRect(WorkSpaceRect.TopLeft(), CSize(width, height)); //创建另一个矩形，左上角点和工作区一致，使用CSize传入矩形大小
			Picture1_Result_Data.StretchBlt(pDC->m_hDC, ShowImageRect, SRCCOPY);//使用SRCCOPY直接将原图片数据传递至展示图片的矩形（ShowImageRect）
		}
		else //图片在工作区中不能以原来的大小放下
		{
			float xScale = (float)WorkSpaceRect.Width() / (float)width; //记录原工作区矩形和图片的长度比例
			float yScale = (float)WorkSpaceRect.Height() / (float)height; //记录原工作区矩形和图片的宽度比例
			float ScaleIndex = (xScale <= yScale ? xScale : yScale); //选出较小的比例

			ShowImageRect = CRect(WorkSpaceRect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex)); //创建一个由原图片按照刚刚获取的较小的比例缩放的，保证能整个放入工作区的矩形
			Picture1_Result_Data.StretchBlt(pDC->m_hDC, ShowImageRect, SRCCOPY);//使用SRCCOPY直接将原图片数据传递至展示图片的矩形（ShowImageRect）
		}
		ReleaseDC(pDC); //将上下文释放
	}
}


/*Mat和CImage之间的转换*/
void CComputerVisionLabDlg::MatToCImage(Mat & mat, CImage & cimage)
{
	if (0 == mat.total())
	{
		return;
	}


	int nChannels = mat.channels();
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = mat.cols;
	int nHeight = mat.rows;


	//重建cimage
	cimage.Destroy();
	cimage.Create(nWidth, nHeight, 8 * nChannels);


	//拷贝数据


	uchar* pucRow;									//指向数据区的行指针
	uchar* pucImage = (uchar*)cimage.GetBits();		//指向数据区的指针
	int nStep = cimage.GetPitch();					//每行的字节数,注意这个返回值有正有负


	if (1 == nChannels)								//对于单通道的图像需要初始化调色板
	{
		RGBQUAD* rgbquadColorTable;
		int nMaxColors = 256;
		rgbquadColorTable = new RGBQUAD[nMaxColors];
		cimage.GetColorTable(0, nMaxColors, rgbquadColorTable);
		for (int nColor = 0; nColor < nMaxColors; nColor++)
		{
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;
		}
		cimage.SetColorTable(0, nMaxColors, rgbquadColorTable);
		delete[]rgbquadColorTable;
	}


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				*(pucImage + nRow * nStep + nCol) = pucRow[nCol];
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					*(pucImage + nRow * nStep + nCol * 3 + nCha) = pucRow[nCol * 3 + nCha];
				}
			}
		}
	}
}
void CComputerVisionLabDlg::CImageToMat(CImage & cimage, Mat & mat)
{
	if (true == cimage.IsNull())
	{
		return;
	}


	int nChannels = cimage.GetBPP() / 8;
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = cimage.GetWidth();
	int nHeight = cimage.GetHeight();


	//重建mat
	if (1 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC1);
	}
	else if (3 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC3);
	}


	//拷贝数据


	uchar* pucRow;									//指向数据区的行指针
	uchar* pucImage = (uchar*)cimage.GetBits();		//指向数据区的指针
	int nStep = cimage.GetPitch();					//每行的字节数,注意这个返回值有正有负


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				pucRow[nCol] = *(pucImage + nRow * nStep + nCol);
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					pucRow[nCol * 3 + nCha] = *(pucImage + nRow * nStep + nCol * 3 + nCha);
				}
			}
		}
	}
}

//开始进行处理
void CComputerVisionLabDlg::OnBnClickedStartprocess()
{
	switch (AlgorithmTab.GetCurSel())
	{
	case 0:
	{
		//SIFT
		//SIFT();
		SIFTMosaic();
		break;
	}
	case 1:
	{
		break;
	}
	case 2:
	{
		break;
	}
	case 3:
	{
		break;
	}
	default:
		break;
	}

}

//tab页的选择
void CComputerVisionLabDlg::OnTcnSelchangeAlgorithmtab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int currentTab = AlgorithmTab.GetCurSel();

	SIFTDlg.ShowWindow(false);
	SURFDlg.ShowWindow(false);
	ORBDlg.ShowWindow(false);
	FERNSDlg.ShowWindow(false);

	switch (currentTab)
	{
	case 0:
	{
		SIFTDlg.ShowWindow(true);
		break;
	}
	case 1:
	{
		SURFDlg.ShowWindow(true);
		break;
	}
	case 2:
	{
		ORBDlg.ShowWindow(true);
		break;
	}
	case 3:
	{
		FERNSDlg.ShowWindow(true);
		break;
	}
	default:
		break;
	}
	*pResult = 0;
}


/**********************SIFT算法**********************/
typedef struct
{
	Point2f left_top;
	Point2f left_bottom;
	Point2f right_top;
	Point2f right_bottom;
}four_corners_t;

four_corners_t corners;

void CComputerVisionLabDlg::CalcCorners(const Mat& H, const Mat& src)
{
	double v2[] = { 0, 0, 1 };//左上角
	double v1[3];//变换后的坐标值
	Mat V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
	Mat V1 = Mat(3, 1, CV_64FC1, v1);  //列向量

	V1 = H * V2;
	//左上角(0,0,1)
	/*cout << "V2: " << V2 << endl;
	cout << "V1: " << V1 << endl;*/
	corners.left_top.x = v1[0] / v1[2];
	corners.left_top.y = v1[1] / v1[2];

	//左下角(0,src.rows,1)
	v2[0] = 0;
	v2[1] = src.rows;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
	V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
	V1 = H * V2;
	corners.left_bottom.x = v1[0] / v1[2];
	corners.left_bottom.y = v1[1] / v1[2];

	//右上角(src.cols,0,1)
	v2[0] = src.cols;
	v2[1] = 0;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
	V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
	V1 = H * V2;
	corners.right_top.x = v1[0] / v1[2];
	corners.right_top.y = v1[1] / v1[2];

	//右下角(src.cols,src.rows,1)
	v2[0] = src.cols;
	v2[1] = src.rows;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
	V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
	V1 = H * V2;
	corners.right_bottom.x = v1[0] / v1[2];
	corners.right_bottom.y = v1[1] / v1[2];

}


//使用SIFT进行特征点匹配
void CComputerVisionLabDlg::SIFT()
{
	/******************进行特征点识别******************/
	Mat sourceImage1;
	CImageToMat(Picture1_Data, sourceImage1);

	Mat sourceImage2;
	CImageToMat(Picture2_Data, sourceImage2);	

	//进行高斯模糊
	Mat sourceImage1AfterBlur, sourceImage2AfterBlur;
	cv::GaussianBlur(sourceImage1, sourceImage1AfterBlur, Size(3, 3), 0.5);
	cv::GaussianBlur(sourceImage2, sourceImage2AfterBlur, Size(3, 3), 0.5);

	//检测特征点并保存到两个特征点数组中
	Ptr<Feature2D>SIFT = xfeatures2d::SIFT::create();
	vector<KeyPoint> keyPoint1, keyPoint2;
	SIFT->detect(sourceImage1AfterBlur, keyPoint1);
	SIFT->detect(sourceImage2AfterBlur, keyPoint2);

	/*画出特征点并展示*/
	//{
	//	drawKeypoints(sourceImage1AfterBlur, keyPoint1, sourceImage1AfterBlur, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	//	drawKeypoints(sourceImage2AfterBlur, keyPoint2, sourceImage2AfterBlur, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	//	MatToCImage(sourceImage1AfterBlur, Picture1_Result_Data);
	//	MatToCImage(sourceImage2AfterBlur, Picture2_Result_Data);

	//	showPicture1Result();
	//	showPicture2Result();
	//}

	/******************进行两幅图的特征点匹配工作******************/
	Mat descriptors_1, descriptors_2;
	SIFT->compute(sourceImage1AfterBlur, keyPoint1, descriptors_1);
	SIFT->compute(sourceImage2AfterBlur, keyPoint2, descriptors_2);

	BFMatcher matcher;
	vector<DMatch>matches;
	matcher.match(descriptors_1, descriptors_2, matches);

	//画出消去前的图像
	Mat Result;
	drawMatches(sourceImage1AfterBlur, keyPoint1, sourceImage2AfterBlur, keyPoint2, matches, Result);

	MatToCImage(Result, Picture1_Result_Data);
	showPicture1Result();

	/*******************使用RANSAC进行消除工作******************/
	vector<KeyPoint> R_keypoint1, R_keypoint2;
	for (size_t i = 0; i < matches.size(); i++)
	{
		R_keypoint1.push_back(keyPoint1[matches[i].queryIdx]);
		R_keypoint2.push_back(keyPoint2[matches[i].trainIdx]);
		//这两句话的理解：R_keypoint1是要存储img01中能与img02匹配的特征点，
		//matches中存储了这些匹配点对的img01和img02的索引值
	}

	//坐标转换
	vector<Point2f>p01, p02;
	for (size_t i = 0; i < matches.size(); i++)
	{
		p01.push_back(R_keypoint1[i].pt);
		p02.push_back(R_keypoint2[i].pt);
	}

	vector<uchar> RansacStatus;
	Mat Fundamental = findFundamentalMat(p01, p02, RansacStatus, FM_RANSAC);

	vector<KeyPoint> RR_keypoint01, RR_keypoint02;
	vector<DMatch> MatchePointsAfterRANSAC;
	int index = 0;
	for (size_t i = 0; i < matches.size(); i++)
	{
		if (RansacStatus[i] != 0)
		{
			RR_keypoint01.push_back(R_keypoint1[i]);
			RR_keypoint02.push_back(R_keypoint2[i]);
			matches[i].queryIdx = index;
			matches[i].trainIdx = index;
			MatchePointsAfterRANSAC.push_back(matches[i]);
			index++;
		}
	}

	Mat ResultAfterRANSAC;
	drawMatches(sourceImage1AfterBlur, RR_keypoint01, sourceImage2AfterBlur, RR_keypoint02, MatchePointsAfterRANSAC, ResultAfterRANSAC);

	MatToCImage(ResultAfterRANSAC, Picture2_Result_Data);
	showPicture2Result();
	imwrite("C:\\Users\\39471\\Desktop\\图像拼接\\result.jpg", ResultAfterRANSAC);
}

//使用SIFT进行特征点匹配然后进行图像拼接
void CComputerVisionLabDlg::SIFTMosaic()
{
	Mat SourceImage1;
	CImageToMat(Picture1_Data, SourceImage1);

	Mat SourceImage2;
	CImageToMat(Picture2_Data, SourceImage2);

	Mat GreySourceImage1;
	Mat GreySourceImage2;

	cvtColor(SourceImage1, GreySourceImage1, CV_BGR2GRAY);
	cvtColor(SourceImage2, GreySourceImage2, CV_BGR2GRAY);

	//-- Step 1: 使用SIFT识别出特征点
	Ptr<Feature2D>SIFT = xfeatures2d::SIFT::create();
	std::vector<KeyPoint> KeyPoints1, KeyPoints2;
	SIFT->detect(GreySourceImage1, KeyPoints1);
	SIFT->detect(GreySourceImage2, KeyPoints2);

	//-- Step 2: 描述SIFT特征
	Mat Descriptors1, Descriptors2;
	SIFT->compute(GreySourceImage1, KeyPoints1, Descriptors1);
	SIFT->compute(GreySourceImage2, KeyPoints2, Descriptors2);

	//-- Step 3: 匹配
	BFMatcher Matcher;//BFMatcher为强制匹配
	std::vector< DMatch > MatchPoints;
	Matcher.match(Descriptors1, Descriptors2, MatchPoints);

	//计算所有匹配点中的最大最小距离
	double max_dist = 0; double min_dist = 100;
	for (int i = 0; i < Descriptors1.rows; i++)
	{
		double dist = MatchPoints[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	std::vector< DMatch > GoodMatchPoints;

	//如果特征描述子的距离小于最小距离的3倍，则被认为是好的匹配点
	for (int i = 0; i < Descriptors1.rows; i++)
	{
		if (MatchPoints[i].distance <= 3 * min_dist)
		{
			GoodMatchPoints.push_back(MatchPoints[i]);
		}
	}

	//把图像二向图像一映射
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;
	for (int i = 0; i < (int)GoodMatchPoints.size(); i++)
	{
		//这里采用“帧向拼接图像中添加的方法”，因此左边的是scene,右边的是obj
		scene.push_back(KeyPoints1[GoodMatchPoints[i].queryIdx].pt);
		obj.push_back(KeyPoints2[GoodMatchPoints[i].trainIdx].pt);
	}
	//直接调用ransac,计算单应矩阵（计算映射矩阵）
	Mat Homography = findHomography(obj, scene, CV_RANSAC);

	//图像对准
	Mat result;
	warpPerspective(SourceImage2, result, Homography, Size(2 * GreySourceImage2.cols, GreySourceImage2.rows));
	Mat half(result, cv::Rect(0, 0, GreySourceImage2.cols, GreySourceImage2.rows));
	SourceImage1.copyTo(half);

	MatToCImage(result, Picture2_Result_Data);
	showPicture2Result();
	//imshow("result", result);
}

