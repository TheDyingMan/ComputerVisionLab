#pragma once


// SURFDlg 对话框

class SURFDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SURFDlg)

public:
	SURFDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SURFDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SURFDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
