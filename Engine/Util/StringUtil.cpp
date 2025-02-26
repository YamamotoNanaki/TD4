#include "StringUtil.h"
#include "Debug.h"
#include <windows.h>
#include <vector>
#include <algorithm>
#include <codecvt>
#include <locale>

std::wstring StringToWString(const std::string& oString)
{
	// 必要なバッファサイズを取得
	int32_t iBufferSize = MultiByteToWideChar(CP_UTF8, 0, oString.c_str(), -1, nullptr, 0);

	// バッファの取得
	std::vector<wchar_t> buff(iBufferSize);

	// UTF-8 から wstring への変換
	MultiByteToWideChar(CP_UTF8, 0, oString.c_str(), -1, buff.data(), iBufferSize);

	// wstring の生成
	std::wstring oRet(buff.data());

	// 変換結果を返す
	return oRet;
}

std::u16string StringToU16String(const std::string& oString)
{
	// 必要なバッファサイズを取得
	int32_t iBufferSize = MultiByteToWideChar(CP_UTF8, 0, oString.c_str(), -1, nullptr, 0);

	// バッファの取得
	std::vector<char16_t> buff(iBufferSize);

	// UTF-8 から UTF-16 への変換
	MultiByteToWideChar(CP_UTF8, 0, oString.c_str(), -1, reinterpret_cast<wchar_t*>(buff.data()), iBufferSize);

	// u16string の生成
	std::u16string oRet(buff.data());

	// 変換結果を返す
	return oRet;
}

bool MatchesPrefix(const std::string& filename, const std::string& prefix)
{
	// ファイル名の最後の'.'の位置を取得
	size_t dotPosition = filename.find_last_of('.');

	// '.'が見つからない場合、ファイル名全体をチェック
	if (dotPosition == std::string::npos) {
		dotPosition = filename.length();
	}

	// プレフィックスの長さを取得
	size_t prefixLength = prefix.length();

	// ファイル名がプレフィックスで始まるかどうかを確認
	if (filename.compare(0, prefixLength, prefix) == 0) {
		return true;
	}

	return false;
}

bool Contains(const std::string& text, const std::string& pattern)
{
	return text.find(pattern) != std::string::npos;
}

std::string ToLower(const std::string& str)
{
	std::string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
	return lowerStr;
}
