#pragma once


#include<Windows.h>
/**  2018.07.08  작업

** Trace 구현 초안 (목표)

DebugLog  :  디버그 모드일시에만 출력창에 출력(포맷, 숫자,가변인자)
DebugLog  :  위와 동일하나 문자열만 받음

*/
/*inline void DebugLog(const WCHAR* format)
{
#if _DEBUG
	OutputDebugString(format);
#endif 
}*/
inline void DebugLog(const WCHAR* format, ...)
{
#if _DEBUG
	WCHAR buffer[256];

	va_list Arglist;
	va_start(Arglist, format);
	wvsprintf(buffer, format, Arglist);
	OutputDebugString(buffer);
	va_end(Arglist);
#endif 
}





