
// BHClientDlg.h: 헤더 파일
//

#pragma once

// 채팅 Message ID
#define NM_CHAT_DATA     1     // 여기서 1은 채팅 데이터가 들어 왔다는 것을 의미함.         

class CBHClientDlg;
// MyClient 클래스에서 CBHClientDlg 대화상자 클래스를 사용할 수 있도록 선언함.
/* MyClient 클래스와 CBHClientDlg 클래스가 서로의 클래스에
   멤버 변수로 상호참조 되어 있기 때문에 CBHClientDlg 클래스 선언문을
   위에 써줘야 오류가 나지 않는다. */

// 자신이 클라이언트용 소켓으로 사용할 MyClient 클래스를 정의함.
class MyClient : public BH_ClientSocket
{
private:
	// 대화 상자 CBHClientDlg의 주소를 저장할 멤버 변수
	CBHClientDlg *mp_parent;

public:
	/* MyClient 클래스가 대화상자 CBHClientDlg 안에 있는 리스트 박스
	   m_event_list를 쓰기 위해 이 대화상자의 주소를 MyClient 클래스가 
	   알도록 해주기 위해서 MyClient 클래스 객체가 생성될 때 이 대화상자의
	   주소를 MyClient 클래스의 객체 생성자의 매개변수 ap_parent에
	   인자값으로 넘겨줄 것임. */

	// 프로토콜 헤더의 시작 바이트를 0x27 값으로 설정함.
	MyClient(CBHClientDlg *ap_parent) : BH_ClientSocket(0x27)
	{
		mp_parent = ap_parent;
		/* 대화상자의 포인터(ap_parent)의 값을 mp_parent에
		   저장하여 기억하고 있다가 나중에 출력하고 싶은 것이
		   있으면 포인터(mp_parent)를 이용해서 리스트 박스
		   (m_event_list)에 접근할 수 있음. */
	}

	// 클라이언트에서 보낸 데이터가 수신되면 호출되는 함수 ProcessRecvData
	virtual int ProcessRecvData(SOCKET ah_socket, unsigned char a_msg_id, char *ap_recv_data, BSize a_body_size);
};

// CBHClientDlg 대화 상자
class CBHClientDlg : public CDialogEx
{
private:
	MyClient m_client;

// 생성입니다.
public:
	CBHClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	// 리스트 박스(m_event_list)에 문자열을 추가하는 함수 AddEventStr
	void AddEventStr(const wchar_t *ap_str);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BHCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// 대화상자 CBHClientDlg의 리스트 박스 m_event_list
	CListBox m_event_list;

	// 클라이언트에서 입력한 문자열 데이터를 서버로 전송하는 함수 OnBnClickedSendBtn
	afx_msg void OnBnClickedSendBtn();

protected:
	// 서버에 접속한 결과를 알려주는 함수 OnConnected
	afx_msg LRESULT OnConnected(WPARAM wParam, LPARAM lParam);

	/* 접속한 서버에서 데이터를 클라이언트에게 전송하거나 클라이언트가 
	   접속을 해제할 때 호출되는 함수 OnReadAndClose */
	afx_msg LRESULT OnReadAndClose(WPARAM wParam, LPARAM lParam);

public:
	// 엔터키를 누르거나 전송 버튼을 클릭하면 서버로 문자열 데이터를 전송하는 함수 OnBnClickedOk
	afx_msg void OnBnClickedOk();

	// 에디트 컨트롤에 입력한 IP 주소에 해당되는 서버와 연결시켜 주는 함수 OnBnClickedConnectBtn
	afx_msg void OnBnClickedConnectBtn();
};
