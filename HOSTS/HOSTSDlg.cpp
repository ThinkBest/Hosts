
// HOSTSDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HOSTS.h"
#include "HOSTSDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPTSTR HostsFile;

// CHOSTSDlg 对话框

CHOSTSDlg::CHOSTSDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CHOSTSDlg::IDD, CHOSTSDlg::IDH, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHOSTSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

// CHOSTSDlg 消息处理程序

BOOL CHOSTSDlg::OnInitDialog()
{
	//设置出现滚动条
	SetHostFlags(DOCHOSTUIFLAG_NO3DBORDER);
	
	CDHtmlDialog::OnInitDialog();

	EnableAutomation();
	SetExternalDispatch(GetIDispatch(TRUE));
  
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 查找Hosts文件
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
	// 备份Hosts文件
	LPTSTR BackupFile = new TCHAR[MAX_PATH];
	lstrcpy(BackupFile,SystemDir);
	BackupFile = lstrcat(BackupFile,L"\\Drivers\\etc\\hosts.2013.bak");
	if(GetFileAttributes(BackupFile) == -1)
	{
		CopyFile(HostsFile,BackupFile,true);
	}
	// 使用外部文件
	LPTSTR sPath = new TCHAR[MAX_PATH];
	::GetModuleFileName(GetModuleHandle(NULL), sPath, MAX_PATH);
	CString dir = CString(sPath);
	dir = dir.Left(dir.ReverseFind('\\'));//解析运行程序所在文件夹的目录
	CString MainFile = CString(dir+L"\\main.html");
	if(GetFileAttributes(MainFile) != -1)
	{
		CDHtmlDialog::Navigate(MainFile);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHOSTSDlg::OnPaint()
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
		CDHtmlDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
		//取得临时文件尺寸
		DWORD dwFileSize=::GetFileSize(pFile, NULL); 
		//申请空间
		char* Output = new char[dwFileSize+1];
		DWORD bytesRead;
		//移动读写指针到文件头部
		::SetFilePointer(pFile,0,0,FILE_BEGIN);
		//读取文件全部内容
		::ReadFile(pFile,Output,dwFileSize,&bytesRead,NULL);
		Output[bytesRead]='\0';
		//关闭文件
		CloseHandle(pFile);
		//ANSI转Unicode
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
		//移动读写指针到文件头部
		::SetFilePointer(pFile,0,0,FILE_BEGIN);
		::WriteFile(pFile,str,strlen,&bytesRead,NULL);
		//关闭文件
		CloseHandle(pFile);
		return true;
	}
	else{
		return false;
	}
} 


