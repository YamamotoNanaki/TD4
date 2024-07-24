#include "StringUtil.h"
#include "Debug.h"
#include <windows.h>
#include <vector>
#include <algorithm>
#include <codecvt>
#include <locale>

std::wstring StringToWString(const std::string& oString)
{
    // �K�v�ȃo�b�t�@�T�C�Y���擾
    int32_t iBufferSize = MultiByteToWideChar(CP_UTF8, 0, oString.c_str(), -1, nullptr, 0);

    // �o�b�t�@�̎擾
    std::vector<wchar_t> buff(iBufferSize);

    // UTF-8 ���� wstring �ւ̕ϊ�
    MultiByteToWideChar(CP_UTF8, 0, oString.c_str(), -1, buff.data(), iBufferSize);

    // wstring �̐���
    std::wstring oRet(buff.data());

    // �ϊ����ʂ�Ԃ�
    return oRet;
}

std::u16string StringToU16String(const std::string& oString)
{
    // �K�v�ȃo�b�t�@�T�C�Y���擾
    int32_t iBufferSize = MultiByteToWideChar(CP_UTF8, 0, oString.c_str(), -1, nullptr, 0);

    // �o�b�t�@�̎擾
    std::vector<char16_t> buff(iBufferSize);

    // UTF-8 ���� UTF-16 �ւ̕ϊ�
    MultiByteToWideChar(CP_UTF8, 0, oString.c_str(), -1, reinterpret_cast<wchar_t*>(buff.data()), iBufferSize);

    // u16string �̐���
    std::u16string oRet(buff.data());

    // �ϊ����ʂ�Ԃ�
    return oRet;
}

std::string ToLower(const std::string& str)
{
	std::string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
	return lowerStr;
}
