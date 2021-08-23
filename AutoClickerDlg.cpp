
// AutoClickerDlg.cpp : implementation file
//

#include "framework.h"
#include "AutoClicker.h"
#include "AutoClickerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutoClickerDlg dialog



CAutoClickerDlg::CAutoClickerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AUTOCLICKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAutoClickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAutoClickerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CAutoClickerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAutoClickerDlg message handlers
bool	flagNeedClick = false;
HHOOK					hGlobalHook;
extern "C" __declspec(dllexport) LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	static bool flagOnCtrl = false;
	bool		flagNeedMoreHook = true;
	if (nCode >= 0 && nCode == HC_ACTION)
	{
		LPKBDLLHOOKSTRUCT keyParam = (LPKBDLLHOOKSTRUCT)(void*)lParam;
		if (wParam == WM_KEYUP)
		{
			switch (keyParam->vkCode)
			{
			case VK_LCONTROL:
			case VK_RCONTROL:
				flagOnCtrl = false;
				break;
			case VK_F1:
				flagNeedClick = !flagNeedClick;
				return 1;
				break;
			}
		}

	}
	return CallNextHookEx(hGlobalHook, nCode, wParam, lParam);
}
BOOL CAutoClickerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//hGlobalHook = SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, GetModuleHandle(NULL), 0);
	mainTimerId = SetTimer(1, 1000, NULL);
	SetTimer(2, 50, NULL);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAutoClickerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAutoClickerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


HWND		GetBlueStackWindows(void)
{
	HWND d3Wnd = FindWindowW(L"BlueStacksApp", L"_ctl.Window");
	if (d3Wnd == NULL) d3Wnd = FindWindowW(L"BlueStacksApp", NULL);
	if (d3Wnd == NULL) d3Wnd = FindWindowW(NULL, L"BlueStacks");
	if (d3Wnd == NULL) d3Wnd = FindWindowW(NULL, L"_ctl.Window");
	return d3Wnd;
}
void		SendLeftMouseClick(int x, int y)
{
	HWND blWnd = GetBlueStackWindows();
	if (blWnd)
	{
		blWnd = FindWindowEx(blWnd, NULL, NULL, NULL);

		POINT point = { 0 };
		point.x = x;
		point.y = y;
		//GetCursorPos(&point);

		RECT d3Rect = { 0 };
		GetWindowRect(blWnd, &d3Rect);

		LPARAM lParam = (point.x - d3Rect.left) | ((point.y - d3Rect.top) << 16);

		SendMessage(blWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
		Sleep(10 + (rand() % 3));
		SendMessage(blWnd, WM_LBUTTONUP, 0, lParam);
		Sleep(10 + (rand() % 3));
	}
}

void CAutoClickerDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString infoStr;
	POINT point = { 0 };
	GetCursorPos(&point);
	infoStr.AppendFormat(L"Mouse: %d %d\r\n", point.x, point.y);
	//	HWND blWnd = GetBlueStackWindows();
	//	blWnd = ::FindWindowEx(blWnd, NULL, NULL, NULL);
	//	infoStr.AppendFormat(L"blWnd: %d\r\n",(int) blWnd);


	if (nIDEvent == mainTimerId)
	{
		//1600 333
		if (flagNeedClick)
		{
			SendLeftMouseClick(1600, 333);
			//SendLeftMouseClick(200, 200);

		}
		infoStr.AppendFormat(L"Send click\r\n", point.x, point.y);
	}
	else
	{
	}


	
	GetDlgItem(IDC_INFO)->SetWindowTextW(infoStr);

	CDialogEx::OnTimer(nIDEvent);
}


void CAutoClickerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
	flagNeedClick = !flagNeedClick;
	if (flagNeedClick) GetDlgItem(IDOK)->SetWindowTextW(L"Stop");
	else GetDlgItem(IDOK)->SetWindowTextW(L"Start");
}
