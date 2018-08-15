#pragma once

namespace JRenderEngine
{
	class JMessage
	{
	public:
		JMessage();
		virtual ~JMessage();
		unsigned  Flags;
		size_t   Size;
		void*    pData;
		void**   pDataArray;
	};
	class Message
	{
	public:
		Message() {}
		~Message() {}
	};
}


