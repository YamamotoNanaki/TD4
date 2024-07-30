#pragma once
#include <string>

std::wstring StringToWString(const std::string& oString);
std::u16string StringToU16String(const std::string& oString);

// ファイル名が指定されたプレフィックスで始まるかどうかを確認する関数（拡張子を無視）
bool MatchesPrefix(const std::string& filename, const std::string& prefix);

bool Contains(const std::string& text, const std::string& pattern);

std::string ToLower(const std::string& str);

