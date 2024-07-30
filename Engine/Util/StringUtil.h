#pragma once
#include <string>

std::wstring StringToWString(const std::string& oString);
std::u16string StringToU16String(const std::string& oString);

// �t�@�C�������w�肳�ꂽ�v���t�B�b�N�X�Ŏn�܂邩�ǂ������m�F����֐��i�g���q�𖳎��j
bool MatchesPrefix(const std::string& filename, const std::string& prefix);

bool Contains(const std::string& text, const std::string& pattern);

std::string ToLower(const std::string& str);

