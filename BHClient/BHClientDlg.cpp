
// BHClientDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "BHClient.h"
#include "BHClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 클라이언트에서 보낸 데이터가 수신되면 호출되는 함수 ProcessRecvData의 정의
int MyClient::ProcessRecvData(SOCKET ah_socket, unsigned char a_msg_id, char *ap_recv_data, BSize a_body_size)
{
	// 대용량 데이터가 전송 또는 수신될 때, 필요한 기본 코드를 수행함.

	// 수신된 데이터가 채팅 데이터인 경우(클라이언트가 채팅 데이터를 서버에 보낸 경우)
	if (a_msg_id == NM_CHAT_DATA)
	{
		/* 수신된 데이터에 채팅 정보가 유니코드 형태의 문자열로 저장되어
		   있기 때문에 유니코드 문자열 형태(wchar_t *)로 캐스팅하여
		   리스트 박스에 추가함. */
		mp_parent->AddEventStr((wchar_t *)ap_recv_data);
	}

	return 1;
	/* ProcessRecvData가 0을 반환하면 실패 했다는 것을 의미함.
	   쉽게 말해 내부적으로 네트웍 진행을 막아버리는 코드가
	   있기 때문에 처리를 못 할 경우 0을 반환한다는 의미임. 
	   
	   1을 반환하면 정상적으로 처리 했음을 의미함. */
}

// CBHClientDlg 대화 상자
/* MyClient 클래스의 객체 m_client로 대화상자 CBHClientDlg의 주소가
   매개변수 this에 인자값으로 넘겨줌.
   그리고 이 넘겨준 주소가 MyClient 클래스의 객체 생성자에 있는
   매개변수 ap_parent로 들어오게 됨.
   그리고 이 들어온 주소를 MyClient 클래스가 알게 됨. */
CBHClientDlg::CBHClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BHCLIENT_DIALOG, pParent), m_client(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBHClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EVENT_LIST, m_event_list);
}

BEGIN_MESSAGE_MAP(CBHClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND_BTN, &CBHClientDlg::OnBnClickedSendBtn)
	ON_MESSAGE(26001, &CBHClientDlg::OnConnected)
	ON_MESSAGE(26002, &CBHClientDlg::OnReadAndClose)
	ON_BN_CLICKED(IDOK, &CBHClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CONNECT_BTN, &CBHClientDlg::OnBnClickedConnectBtn)
END_MESSAGE_MAP()


// CBHClientDlg 메시지 처리기

BOOL CBHClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	AddEventStr(L"채팅 클라이언트 서비스를 시작합니다.");
	AddEventStr(L"서버 IP 주소를 입력한 후 서버 접속 버튼을 클릭하세요!");
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

//  대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CBHClientDlg::OnPaint()
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

//  사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBHClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 클라이언트에서 입력한 문자열 데이터를 서버로 전송하는 함수 OnBnClickedSendBtn의 정의 
void CBHClientDlg::OnBnClickedSendBtn()
{
	CString str;
	/* GetDlgItemText 함수를 호출하여 IDC_CHAT_EDIT 박스(채팅글 입력창)에 
	   적혀 있는 문자열을 복사해와서 str에 넣어줌. */
	// 에디트 컨트롤에 입력된 글을 가져옴.
	GetDlgItemText(IDC_CHAT_EDIT, str);

	/* IDC_CHAT_EDIT 박스에 적혀 있는 문자열을 복사해와서 str에 넣어준 후에
	   SetDlgItemText 함수를 호출하여 IDC_CHAT_EDIT 박스(채팅글 입력창)에
	   적혀 있는 문자열을 지워줌. 
	   이렇게 해야 기존에 썼던 채팅글을 다시 지우지 않고 다음 채팅글을 바로 
	   입력할 수 있도록 IDC_CHAT_EDIT 박스에 기존에 있던 문자열을 깨끗하게 지워줌. */
	SetDlgItemText(IDC_CHAT_EDIT, L"");

	/* IsConnected 함수의 반환 값이 1이면 서버에 접속이 된 경우이다.
	   반면 반환 값이 0일 경우 서버에 접속이 안된 경우이다. 
	   if(m_client.IsConnected() == 1)이라고 해도 되지만 
	   아래처럼 if(m_client.IsConnected()) 라고 해도 된다.
	   IsConnected 함수의 반환값이 어차피 1이고, 1이면 참이기 때문에
	   if문을 바로 실행할 수 있기 때문이다. */ 

	// IsConnected 함수를 호출하여 서버와 접속 상태인지 확인함.
	if (m_client.IsConnected())
	{
		// str에 들어있는 문자열을 서버로 전송함.
		/* str 객체가 관리하고 있는 문자열의 시작 주소 값을
		   (char *)로 형변환 해서 이 주소 값을 사용함. */

		/* 채팅 메시지를 서버로 전송함. 문자열을 전송할 때는 NULL 문자를
		   포함해서 전송하기 때문에 문자열 길이에 1을 더하고 유니코드에서 
		   문자 1개는 2바이트를 차지하기 때문에 문자열 길이에 2를 곱함. */
		m_client.SendFrameData(NM_CHAT_DATA, (char *)(const wchar_t *)str, (str.GetLength() + 1) * 2);
	}
}

/* CBHClientDlg 클래스 밖에서 이 리스트 박스(m_event_list)의 항목을
   추가할 때 사용하는 인터페이스 함수 AddEventStr의 정의 */
// 리스트 박스(m_event_list)에 문자열을 추가하는 함수 AddEventStr의 정의
void CBHClientDlg::AddEventStr(const wchar_t *ap_str)
{
	/* 리스트 박스의 가장 아래 쪽에 문자열을 추가함.
	   문자열(ap_str)을 리스트 박스(m_event_list)에 보냄. */
	int index = m_event_list.InsertString(-1, ap_str);

	/* SetCurSel 함수를 호출하여 인덱스 값을 얻고 해당 인덱스를 커서로 
	   설정하여 문자열을 추가할 때마다 해당 문자열을 표시하게 해줌. 
	   이렇게 하면 입력한 곳에 자동으로 커서 위치가 설정이 되기 때문에
	   화면을 셋팅 했을 때 가장 마지막으로 입력한 부분으로 커서가 가게 됨. */
	m_event_list.SetCurSel(index);
}


// 서버에 접속한 결과를 알려주는 함수 OnConnected의 정의
afx_msg LRESULT CBHClientDlg::OnConnected(WPARAM wParam, LPARAM lParam)
{
	// 반환값이 1이면 서버에 접속을 성공함, 0이면 서버에 접속을 실패함.
	/* ResultOfConn 함수를 호출하여 서버 접속에 대한 결과를 
	   알려주는 메시지를 처리함. ResultOfConn 함수의 반환값이 1일 경우
	   서버 접속에 성공했음을 의미함. */
	if (m_client.ResultOfConn(lParam) == 1)   // FD_CONNECT
	{
		AddEventStr(L"서버 접속 성공!");
	}		
	else   // 서버 접속에 실패한 경우
	{
		AddEventStr(L"서버 접속 실패!");
	}
			
	return 0;
}

/* 접속한 서버에서 데이터를 클라이언트에게 전송하거나 클라이언트가 접속을 해제할 때 
   호출되는 함수 OnReadAndClose의 정의 */
afx_msg LRESULT CBHClientDlg::OnReadAndClose(WPARAM wParam, LPARAM lParam)
{
	// 접속한 서버에서 데이터를 전송하거나 접속을 해제할 때 발생하는 메시지를 처리함.
	m_client.ProcessServerEvent(wParam, lParam);   // FD_READ, FD_CLOSE

	return 0;
}

/* 엔터키를 누르거나 전송 버튼을 클릭하면 서버로 문자열 데이터를 전송하는 함수
   OnBnClickedOk의 정의 */
void CBHClientDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialogEx::OnOK();

	// 클라이언트에서 입력한 문자열 데이터를 서버로 전송하는 함수 OnBnClickedSendBtn 호출함.
	OnBnClickedSendBtn();
}

 /* 에디트 컨트롤에 입력한 IP 주소에 해당되는 서버와 연결시켜 주는 함수
    OnBnClickedConnectBtn의 정의 */
void CBHClientDlg::OnBnClickedConnectBtn()
{
	// 서버와 접속 상태인지 확인함.
	if (!m_client.IsConnected())
	{
		CString str;

		// 사용자가 입력한 주소를 얻음.
		GetDlgItemText(IDC_IP_EDIT, str);

		/* IsEmpty는 입력값(CString 클래스의 객체 str)이 
		   초기화 상태(비어 있는 상태)인지 확인하는 함수임.
		   입력값이 비어 있다면 True를 반환하고,
		   입력값이 들어 있다면 False를 반환함. 
		   False는 0을 의미하기도 해서 0의 부정(!0)은 1이 되므로
		   if문을 실행할 수 있음. 
		   
		   참고사항 
		   IsEmpty("")의 반환값은 false 이다!
		   왜냐하면 인자값으로 공백값("")을 받았기 때문에
		   초기화 된 상태가 아니라 빈 문자열을 값으로 갖는 것이기 때문임.
		   그래서 비어 있는 상태라고 볼 수 없고, 따라서 false가 
		   결과로 표시됨. */

		// str 객체에 주소가 들어 있는 경우
		if (!str.IsEmpty())
		{
			AddEventStr(L"서버 접속 시도 중... : " + str);

			// 사용자가 접속 요청한 IP 주소에 포트 번호가 27100인 서버로 접속을 시도함.
			m_client.ConnToServer(str, 27100, m_hWnd);
		}
	}

}


