#pragma once


// SIFTDlg 对话框

class SIFTDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SIFTDlg)

public:
	SIFTDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SIFTDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIFTDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
