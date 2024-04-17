#include "Sound.h"
#include "Debug.h"
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

using namespace IFE;

void IFE::Sound::Initialize()
{
	HRESULT result = XAudio2Create(&xAudio_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result) && "サウンドの初期化に失敗しました");
	result = xAudio_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(result) && "サウンドの初期化に失敗しました");
}

uint16_t IFE::Sound::LoadWave(const std::string& filename)
{
	for (uint16_t i = 0; i < Sound::sMAX_SOUND_; i++)
	{
		if (soundDatas_[i].free == false)continue;
		if (soundDatas_[i].name == filename)return i;
	}
	std::string name = "Data/Sound/";
	name += filename + ".wav";

	uint16_t num = 0xffff;
	for (uint16_t i = 0; i < Sound::sMAX_SOUND_; i++)
	{
		if (soundDatas_[i].free == false)
		{
			num = i;
			break;
		}
	}

	if (num == -1)assert(0 && "SoundDataの空きがありません");

	std::ifstream file;

	file.open(name, std::ios_base::binary);
	assert(file.is_open() && "ファイルが開けません");

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)assert(0 && "ファイルがRIFFではありません");
	if (strncmp(riff.type, "WAVE", 4) != 0)assert(0 && "フォーマットがWAVEではありません");

	FormatChunk format;

	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chank.id, "fmt ", 4) != 0)assert(0 && "チャンクの確認に失敗しました");
	assert(format.chank.size <= sizeof(format.fmt) && "40byteは対応してません");
	file.read((char*)&format.fmt, format.chank.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	if (strncmp(data.id, "JUNK ", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "data ", 4) != 0)assert(0 && "データの確認に失敗しました");

	std::vector<char> pBuffer(data.size);
	std::vector<BYTE> byteBuff(data.size);

	file.read(pBuffer.data(), data.size);
	file.close();

	for (size_t i = 0; i < pBuffer.size(); i++)
	{
		byteBuff[i] = static_cast<BYTE>(pBuffer[i]);
	}

	soundDatas_[num].wfex = format.fmt;
	soundDatas_[num].pBuffer = std::move(byteBuff);
	soundDatas_[num].bufferSize = data.size;
	soundDatas_[num].free = true;
	soundDatas_[num].name = filename;

	return num;
}

uint16_t IFE::Sound::GetSoundNum(const std::string& wave)
{
	for (uint16_t i = 0; i < sMAX_SOUND_; i++)
	{
		if (wave == soundDatas_[i].name)
		{
			return i;
		}
	}
	return (uint16_t)-1;
}

void IFE::Sound::SoundUnLoad(uint16_t soundNum)
{
	if (!soundDatas_[soundNum].free)return;
	soundDatas_[soundNum].pBuffer.clear();

	soundDatas_[soundNum].bufferSize = 0;
	soundDatas_[soundNum].wfex = {};
	soundDatas_[soundNum].free = false;
	soundDatas_[soundNum].pSourceVoice = nullptr;
}

void IFE::Sound::AllUnLoad()
{
	for (uint16_t i = 0; i < sMAX_SOUND_; i++)
	{
		SoundUnLoad(i);
	}
}

void IFE::Sound::SoundPlay(uint16_t soundNum, bool roop, bool stop)
{
	if(stop)StopSound(soundNum);
	HRESULT result;
	result = xAudio_.Get()->CreateSourceVoice(&soundDatas_[soundNum].pSourceVoice, &soundDatas_[soundNum].wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundDatas_[soundNum].pBuffer.data();
	buf.AudioBytes = soundDatas_[soundNum].bufferSize;
	if (roop)buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	else buf.LoopCount = 0;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	soundDatas_[soundNum].pSourceVoice->SetVolume(soundDatas_[soundNum].volume);
	result = soundDatas_[soundNum].pSourceVoice->SubmitSourceBuffer(&buf);
	result = soundDatas_[soundNum].pSourceVoice->Start();
}

Sound* IFE::Sound::Instance()
{
	static Sound inst;
	return &inst;
}

void IFE::Sound::SetVolume(uint16_t soundNum, int32_t volume)
{
	soundDatas_[soundNum].volume = (float)volume / 255.0f;
	if (soundDatas_[soundNum].pSourceVoice != nullptr)soundDatas_[soundNum].pSourceVoice->SetVolume(soundDatas_[soundNum].volume);
}

void IFE::Sound::StopSound(uint16_t soundNum)
{
	if (soundDatas_[soundNum].pSourceVoice == nullptr)return;
#ifdef _DEBUG
	HRESULT result = soundDatas_[soundNum].pSourceVoice->Stop();
	assert(SUCCEEDED(result));

#else
	soundDatas_[soundNum].pSourceVoice->Stop();
#endif
	soundDatas_[soundNum].pSourceVoice = nullptr;
}

void IFE::Sound::SetVolume(std::string soundName, std::int32_t volume)
{
	uint16_t soundNum = GetSoundNum(soundName);
	if (soundNum == uint16_t(-1))return;
	soundDatas_[soundNum].volume = (float)volume / 255.0f;
	if (soundDatas_[soundNum].pSourceVoice != nullptr)soundDatas_[soundNum].pSourceVoice->SetVolume(soundDatas_[soundNum].volume);
}

void IFE::Sound::SoundPlay(std::string soundName, bool roop, bool stop)
{
	uint16_t soundNum = GetSoundNum(soundName);
	if (soundNum == uint16_t(-1))return;
	SoundPlay(soundNum, roop, stop);
}

void IFE::Sound::StopSound(std::string soundName)
{
	uint16_t soundNum = GetSoundNum(soundName);
	if (soundNum == uint16_t(-1))return;
	if (soundDatas_[soundNum].pSourceVoice == nullptr)return;
#ifdef _DEBUG
	HRESULT result = soundDatas_[soundNum].pSourceVoice->Stop();
	assert(SUCCEEDED(result));

#else
	soundDatas_[soundNum].pSourceVoice->Stop();
#endif
	soundDatas_[soundNum].pSourceVoice = nullptr;
}

void IFE::Sound::AllStop()
{
	for (uint16_t i = 0; i < sMAX_SOUND_; i++)
	{
		StopSound(i);
	}
}

void IFE::Sound::Finalize()
{
	Sound* inst = Instance();
	inst->xAudio_.Reset();
	inst->AllUnLoad();
}
