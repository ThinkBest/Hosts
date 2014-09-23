
// HOSTSDlg.h : 头文件
//

#pragma once


// CHOSTSDlg 对话框
class CHOSTSDlg : public CDHtmlDialog
{
// 构造
public:
	CHOSTSDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HOSTS_DIALOG, IDH = IDR_HTML_HOSTS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL IsExternalDispatchSafe(){ return TRUE;}//重写此函数可以屏蔽掉网页弹出的ActiveX安全警告对话框

	BSTR	LoadHosts();
	bool	SaveHosts(VARIANT& Param);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_DISPATCH_MAP()
};
