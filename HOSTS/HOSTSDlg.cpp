
// HOSTSDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HOSTS.h"
#include "HOSTSDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPTSTR HostsFile;

// CHOSTSDlg �Ի���

CHOSTSDlg::CHOSTSDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CHOSTSDlg::IDD, CHOSTSDlg::IDH, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHOSTSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

// CHOSTSDlg ��Ϣ�������

BOOL CHOSTSDlg::OnInitDialog()
{
	//���ó��ֹ�����
	SetHostFlags(DOCHOSTUIFLAG_NO3DBORDER);
	
	CDHtmlDialog::OnInitDialog();

	EnableAutomation();
	SetExternalDispatch(GetIDispatch(TRUE));
  
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// ����Hosts�ļ�
	HostsFile = new TCHAR[MAX_PATH];
	LPTSTR SystemDir = new TCHAR[MAX_PATH];
	GetSystemDirectory(SystemDir,MAX_PATH);
	lstrcpy(HostsFile,SystemDir);
	HostsFile = lstrcat(HostsFile,L"\\Drivers\\etc\\hosts");
	if(GetFileAttributes(HostsFile) == -1)
	{
		AfxMessageBox(L"Hosts Not Found");
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
	// ����Hosts�ļ�
	LPTSTR BackupFile = new TCHAR[MAX_PATH];
	lstrcpy(BackupFile,SystemDir);
	BackupFile = lstrcat(BackupFile,L"\\Drivers\\etc\\hosts.2013.bak");
	if(GetFileAttributes(BackupFile) == -1)
	{
		CopyFile(HostsFile,BackupFile,true);
	}
	// ʹ���ⲿ�ļ�
	LPTSTR sPath = new TCHAR[MAX_PATH];
	::GetModuleFileName(GetModuleHandle(NULL), sPath, MAX_PATH);
	CString dir = CString(sPath);
	dir = dir.Left(dir.ReverseFind('\\'));//�������г��������ļ��е�Ŀ¼
	CString MainFile = CString(dir+L"\\main.html");
	if(GetFileAttributes(MainFile) != -1)
	{
		CDHtmlDialog::Navigate(MainFile);
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHOSTSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDHtmlDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CHOSTSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_DISPATCH_MAP(CHOSTSDlg, CDHtmlDialog)  
	DISP_FUNCTION(CHOSTSDlg, "loadHosts",LoadHosts,VT_BSTR,VTS_NONE)
	DISP_FUNCTION(CHOSTSDlg, "saveHosts",SaveHosts,VT_BOOL,VTS_VARIANT)  
END_DISPATCH_MAP()  

BSTR CHOSTSDlg::LoadHosts()  
{  
	HANDLE pFile = ::CreateFile(HostsFile,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	CString Result;
	if (pFile != INVALID_HANDLE_VALUE)
	{
		//ȡ����ʱ�ļ��ߴ�
		DWORD dwFileSize=::GetFileSize(pFile, NULL); 
		//����ռ�
		char* Output = new char[dwFileSize+1];
		DWORD bytesRead;
		//�ƶ���дָ�뵽�ļ�ͷ��
		::SetFilePointer(pFile,0,0,FILE_BEGIN);
		//��ȡ�ļ�ȫ������
		::ReadFile(pFile,Output,dwFileSize,&bytesRead,NULL);
		Output[bytesRead]='\0';
		//�ر��ļ�
		CloseHandle(pFile);
		//ANSIתUnicode
		int wcsLen = ::MultiByteToWideChar(CP_ACP,NULL,Output,bytesRead,NULL,0);
		TCHAR* T = new TCHAR[wcsLen+1];
		::MultiByteToWideChar(CP_ACP,NULL,Output,bytesRead,T,wcsLen);
		T[wcsLen] = '\0';
		Result = CString(T);
	}
	else{
		Result = _T("");
	}
	return Result.AllocSysString();
}

bool  CHOSTSDlg::SaveHosts(VARIANT& Param)  
{  
	CComVariant Content(Param);
    Content.ChangeType(VT_BSTR);
    USES_CONVERSION;
    CString Text;
    Text.Format(_T("%s"),OLE2T(Content.bstrVal));
    if( !Text ){
		return false;
	}
	HANDLE pFile = ::CreateFile(HostsFile,GENERIC_WRITE,0,NULL,TRUNCATE_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (pFile != INVALID_HANDLE_VALUE)
	{
		int strlen = ::WideCharToMultiByte(CP_ACP, NULL, Text, wcslen(Text), NULL, 0, NULL, FALSE);
		char* str = new char[strlen + 1];
		::WideCharToMultiByte(CP_ACP, NULL, Text, wcslen(Text), str, strlen, NULL, FALSE);
		str[strlen] = '\0';
		DWORD bytesRead;
		//�ƶ���дָ�뵽�ļ�ͷ��
		::SetFilePointer(pFile,0,0,FILE_BEGIN);
		::WriteFile(pFile,str,strlen,&bytesRead,NULL);
		//�ر��ļ�
		CloseHandle(pFile);
		return true;
	}
	else{
		return false;
	}
} 


