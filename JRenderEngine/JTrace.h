#pragma once


#include<Windows.h>
/**  2018.07.08  �۾�

** Trace ���� �ʾ� (��ǥ)

DebugLog  :  ����� ����Ͻÿ��� ���â�� ���(����, ����,��������)
DebugLog  :  ���� �����ϳ� ���ڿ��� ����

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





