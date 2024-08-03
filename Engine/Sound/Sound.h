#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <array>
#include <string>
#include <vector>

namespace IFE
{
	class Sound
	{
	public:
		enum class SoundSettings
		{
			SE, BGM
		};
	private:
		struct ChunkHeader
		{
			char id[4];
			int32_t size;
		};

		struct RiffHeader
		{
			ChunkHeader chunk;
			char type[4];
		};

		struct FormatChunk
		{
			ChunkHeader chank;
			WAVEFORMATEX fmt;
		};

		struct SoundData
		{
			WAVEFORMATEX wfex;
			float volume = 0.5f;
			IXAudio2SourceVoice* pSourceVoice = nullptr;
			std::vector<BYTE> pBuffer;
			uint32_t bufferSize = 0;
			std::string name;
			bool free = false;
			bool isPlaying = false;
			bool bgm = bool(SoundSettings::BGM);
		};

	private:
		Microsoft::WRL::ComPtr<IXAudio2> xAudio_;
		IXAudio2MasteringVoice* masterVoice_;
		static const uint16_t sMAX_SOUND_ = 128;
		std::array<SoundData, sMAX_SOUND_> soundDatas_;
		float masterVolume_ = 0.5f;
		float bgmVolume_ = 0.5f;
		float seVolume_ = 0.5f;

	public:
		void SetMasterVolume(float masterVolume);
		void SetBGMVolume(float bgmVolume);
		void SetSEVolume(float seVolume);
		void SetMasterVolume(int32_t masterVolume);
		void AllSoundVolumeUpdate();
		void Initialize();
		void Update();
		uint16_t LoadWave(const std::string& wave, SoundSettings setting);
		uint16_t LoadMP3(const std::string& mp3, SoundSettings setting);
		uint16_t GetSoundNum(const std::string& wave);
		void SoundUnLoad(uint16_t soundNum);
		void AllUnLoad();
		void SetVolume(uint16_t soundNum, std::int32_t volume);
		void SoundPlay(uint16_t soundNum, bool roop = true, bool stop = false);
		void StopSound(uint16_t soundNum);
		void SetVolume(std::string soundName, std::int32_t volume);
		void SoundPlay(std::string soundName, bool roop = true, bool stop = false);
		void StopSound(std::string soundName);

		//変更スピードに合わせてピッチが変わる
		void SoundChangeSpeedAndPitch(std::string soundName,float speed);
		//変更スピードに合わせてピッチが変わる
		void SoundChangeSpeedAndPitch(uint16_t soundNum, float speed);

		////元のピッチのままスピードが変わる(デフォルト1)
		//void SoundChangeSpeed(std::string soundName, float speed);
		////元のピッチのままスピードが変わる(デフォルト1)
		//void SoundChangeSpeed(uint16_t soundNum, float speed);
		////元のスピードのままピッチが変わる(デフォルト0、12で1オクターブ)
		//void SoundChangePitch(std::string soundName, int16_t pitch);
		////元のスピードのままピッチが変わる(デフォルト0、12で1オクターブ)
		//void SoundChangePitch(uint16_t soundNum, int16_t pitch);

		bool GetPlayStatus(std::string soundName);
		bool GetPlayStatus(uint16_t soundNum);
		void AllStop();
		static Sound* Instance();
		static void Finalize();

	private:
		void ChengeMasterVolume(SoundData& sound);
		Sound() {};
		Sound(const Sound&) {};
		Sound& operator=(const Sound&) {}
		~Sound() {};
	};
}
