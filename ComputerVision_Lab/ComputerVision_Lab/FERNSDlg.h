#pragma once


// FERNSDlg 对话框

class FERNSDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FERNSDlg)

public:
	FERNSDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~FERNSDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FERNSDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
