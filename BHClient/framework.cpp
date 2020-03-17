#include "pch.h"
#include "framework.h"

#pragma comment(lib, "WS2_32.lib")   
// Windows 소켓 라이브러리(WS2_32.lib)를 사용함.

#ifdef _DEBUG 
// Debug로 선택한 후 컴파일 할 때 D_BH_Socket.lib 라이브러리 파일이 사용됨. 
	#pragma comment(lib, "D_BH_Socket.lib")
#else
// Release로 선택한 후 컴파일 할 때 R_BH_Socket.lib 라이브러리 파일이 사용됨.
	#pragma comment(lib, "R_BH_Socket.lib")
#endif
