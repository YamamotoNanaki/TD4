#include "Sound.h"
#include "StringUtil.h"
#include "Debug.h"
#include <cassert>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfobjects.h>
#include <mfmediaengine.h>

#pragma comment(lib,"xaudio2.lib")

using namespace IFE;

void IFE::Sound::SetMasterVolume(float masterVolume)
{
	masterVolume_ = masterVolume;
}

void IFE::Sound::SetBGMVolume(float bgmVolume)
{
	bgmVolume_ = bgmVolume;
}

void IFE::Sound::SetSEVolume(float seVolume)
{
	seVolume_ = seVolume;
}

void IFE::Sound::SetMasterVolume(int32_t masterVolume)
{
	SetMasterVolume(masterVolume / 255.f);
}

void IFE::Sound::AllSoundVolumeUpdate()
{
	for (uint16_t i = 0; i < sMAX_SOUND_; i++)
	{
		if (soundDatas_[i].free == false)continue;
		ChengeMasterVolume(soundDatas_[i]);
	}
}

void IFE::Sound::Initialize()
{
	HRESULT result = XAudio2Create(&xAudio_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result) && "サウンドの初期化に失敗しました");
	result = xAudio_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(result) && "サウンドの初期化に失敗しました");
	result = MFStartup(MF_VERSION);
	assert(SUCCEEDED(result) && "WindowsMediaFoundationの初期化に失敗しました");
}

void IFE::Sound::Update()
{
	XAUDIO2_VOICE_STATE state;
	for (uint16_t i = 0; i < sMAX_SOUND_; i++)
	{
		if (soundDatas_[i].free == false)continue;
		if (!soundDatas_[i].pSourceVoice)continue;
		soundDatas_[i].pSourceVoice->GetState(&state);
		if (state.BuffersQueued > 0)soundDatas_[i].isPlaying = true;
		else soundDatas_[i].isPlaying = false;
	}
}

uint16_t IFE::Sound::LoadWave(const std::string& filename, SoundSettings setting)
{
	for (uint16_t i = 0; i < Sound::sMAX_SOUND_; i++)
	{
		if (soundDatas_[i].free == false)continue;
		if (soundDatas_[i].name == filename + "_w")return i;
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
	soundDatas_[num].name = filename + "_w";
	soundDatas_[num].bgm = bool(setting);

	return num;
}

uint16_t IFE::Sound::LoadMP3(const std::string& mp3, SoundSettings setting)
{
	for (uint16_t i = 0; i < Sound::sMAX_SOUND_; i++)
	{
		if (soundDatas_[i].free == false)continue;
		if (soundDatas_[i].name == mp3 + "_m")return i;
	}
	std::string name = "Data/Sound/";
	name += mp3 + ".mp3";

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


	IMFSourceReader* pReader = NULL;
	HRESULT hr = MFCreateSourceReaderFromURL(StringToWString(name).c_str(), NULL, &pReader);
	if (FAILED(hr)) {
		return 0xffff;
	}

	IMFMediaType* pAudioType = NULL;
	hr = MFCreateMediaType(&pAudioType);
	if (SUCCEEDED(hr)) {
		pAudioType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		pAudioType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
		hr = pReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, pAudioType);
		pAudioType->Release();
	}

	if (FAILED(hr)) {
		pReader->Release();
		return 0xffff;
	}

	SoundData& soundData = soundDatas_[num];

	IMFMediaType* pOutputType = NULL;
	hr = pReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pOutputType);
	if (SUCCEEDED(hr)) {
		UINT32 cbFormat = 0;
		WAVEFORMATEX* pwfx = NULL;

		hr = MFCreateWaveFormatExFromMFMediaType(pOutputType, &pwfx, &cbFormat);
		if (SUCCEEDED(hr)) {
			soundData.wfex = *pwfx;
			CoTaskMemFree(pwfx);
		}
		pOutputType->Release();
	}

	if (FAILED(hr)) {
		pReader->Release();
		return 0xffff;
	}

	IMFSample* pSample = NULL;
	IMFMediaBuffer* pBuffer = NULL;

	while (true) {
		DWORD dwFlags = 0;
		hr = pReader->ReadSample(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, NULL, &dwFlags, NULL, &pSample);
		if (FAILED(hr) || dwFlags & MF_SOURCE_READERF_ENDOFSTREAM) {
			break;
		}
		if (pSample) {
			hr = pSample->ConvertToContiguousBuffer(&pBuffer);
			if (SUCCEEDED(hr)) {
				BYTE* pAudioData = NULL;
				DWORD cbBuffer = 0;
				hr = pBuffer->Lock(&pAudioData, NULL, &cbBuffer);
				if (SUCCEEDED(hr)) {
					soundData.pBuffer.insert(soundData.pBuffer.end(), pAudioData, pAudioData + cbBuffer);
				}
				pBuffer->Unlock();
				pBuffer->Release();
			}
			pSample->Release();
		}
	}

	soundData.bufferSize = static_cast<uint32_t>(soundData.pBuffer.size());
	pReader->Release();

	soundDatas_[num].free = true;
	soundDatas_[num].name = mp3 + "_m";
	soundDatas_[num].bgm = bool(setting);

	return num;
}

uint16_t IFE::Sound::GetSoundNum(const std::string& wave)
{
	for (uint16_t i = 0; i < sMAX_SOUND_; i++)
	{
		if (wave == soundDatas_[i].name || wave + "_m" == soundDatas_[i].name || wave + "_w" == soundDatas_[i].name)
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
	if (stop)StopSound(soundNum);
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
	soundDatas_[soundNum].isPlaying = true;
}

Sound* IFE::Sound::Instance()
{
	static Sound inst;
	return &inst;
}

void IFE::Sound::SetVolume(uint16_t soundNum, int32_t volume)
{
	soundDatas_[soundNum].volume = (float)volume / 255.0f;
	ChengeMasterVolume(soundDatas_[soundNum]);
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
	SetVolume(soundNum, volume);
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
	soundDatas_[soundNum].isPlaying = false;
}

void IFE::Sound::SoundChangeSpeedAndPitch(std::string soundName, float speed)
{
	uint16_t soundNum = GetSoundNum(soundName);
	if (soundNum == uint16_t(-1))return;
	SoundChangeSpeedAndPitch(soundNum, speed);
}

void IFE::Sound::SoundChangeSpeedAndPitch(uint16_t soundNum, float speed)
{
#ifdef _DEBUG
	HRESULT result = soundDatas_[soundNum].pSourceVoice->SetFrequencyRatio(speed);
	assert(SUCCEEDED(result) && "サウンド変更失敗");
#else
	soundDatas_[soundNum].pSourceVoice->SetFrequencyRatio(speed);
#endif
}

//void IFE::Sound::SoundChangeSpeed(std::string soundName, float speed)
//{
//	uint16_t soundNum = GetSoundNum(soundName);
//	if (soundNum == uint16_t(-1))return;
//	SoundChangeSpeed(soundNum, speed);
//}
//
//void IFE::Sound::SoundChangeSpeed(uint16_t soundNum, float speed)
//{
//#ifdef _DEBUG
//	HRESULT result = soundDatas_[soundNum].pSourceVoice->SetFrequencyRatio(speed);
//	assert(SUCCEEDED(result) && "サウンド変更失敗");
//#else
//	soundDatas_[soundNum].pSourceVoice->SetFrequencyRatio(speed);
//#endif
//}
//
//void IFE::Sound::SoundChangePitch(std::string soundName, int16_t pitch)
//{
//	uint16_t soundNum = GetSoundNum(soundName);
//	if (soundNum == uint16_t(-1))return;
//	SoundChangePitch(soundName, pitch);
//}
//
//void IFE::Sound::SoundChangePitch(uint16_t soundNum, int16_t pitch)
//{
//	float semitoneDown = std::pow(2.0f, pitch / 12.0f);
//#ifdef _DEBUG
//	HRESULT result = soundDatas_[soundNum].pSourceVoice->SetFrequencyRatio(semitoneDown);
//	assert(SUCCEEDED(result) && "サウンド変更失敗");
//#else
//	soundDatas_[soundNum].pSourceVoice->SetFrequencyRatio(semitoneDown);
//#endif
//}

bool IFE::Sound::GetPlayStatus(std::string soundName)
{
	uint16_t soundNum = GetSoundNum(soundName);
	if (soundNum == uint16_t(-1))return false;
	return GetPlayStatus(soundNum);
}

bool IFE::Sound::GetPlayStatus(uint16_t soundNum)
{
	return soundDatas_[soundNum].isPlaying;
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

void IFE::Sound::ChengeMasterVolume(SoundData& sound)
{
	if (sound.pSourceVoice != nullptr)
	{
		if(sound.bgm)sound.pSourceVoice->SetVolume(sound.volume * masterVolume_ * bgmVolume_);
		else sound.pSourceVoice->SetVolume(sound.volume * masterVolume_ * seVolume_);
	}
}
