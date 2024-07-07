#include "StringUtil.h"
#include "Debug.h"
#include <windows.h>
#include <vector>
#include <algorithm>

std::wstring StringToWString(const std::string& oString)
{
	// SJIS �� wstring
	int32_t iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str()
		, -1, (wchar_t*)NULL, 0);

	// �o�b�t�@�̎擾
	std::vector<wchar_t>buff;
	buff.resize(iBufferSize);

	wchar_t* buffPtr = buff.data();

	// SJIS �� wstring
	MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, buffPtr
		, iBufferSize);

	// string�̐���
	std::wstring oRet(buffPtr, buffPtr + iBufferSize - 1);

	// �ϊ����ʂ�Ԃ�
	return(oRet);
}

std::string ToLower(const std::string& str)
{
	std::string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
	return lowerStr;
}
