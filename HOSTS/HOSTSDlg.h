
// HOSTSDlg.h : ͷ�ļ�
//

#pragma once


// CHOSTSDlg �Ի���
class CHOSTSDlg : public CDHtmlDialog
{
// ����
public:
	CHOSTSDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HOSTS_DIALOG, IDH = IDR_HTML_HOSTS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual BOOL IsExternalDispatchSafe(){ return TRUE;}//��д�˺����������ε���ҳ������ActiveX��ȫ����Ի���

	BSTR	LoadHosts();
	bool	SaveHosts(VARIANT& Param);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_DISPATCH_MAP()
};
