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
			float volume = 50;
			IXAudio2SourceVoice* pSourceVoice = nullptr;
			std::vector<BYTE> pBuffer;
			uint32_t bufferSize;
			std::string name;
			bool free = false;
		};
	private:
		Microsoft::WRL::ComPtr<IXAudio2> xAudio_;
		IXAudio2MasteringVoice* masterVoice_;
		static const uint16_t sMAX_SOUND_ = 128;
		std::array<SoundData, sMAX_SOUND_> soundDatas_;

	public:
		void Initialize();
		uint16_t LoadWave(const std::string& wave);
		uint16_t GetSoundNum(const std::string& wave);
		void SoundUnLoad(uint16_t soundNum);
		void AllUnLoad();
		void SetVolume(uint16_t soundNum, std::int32_t volume);
		void SoundPlay(uint16_t soundNum, bool roop = true, bool stop = false);
		void StopSound(uint16_t soundNum);
		void SetVolume(std::string soundName, std::int32_t volume);
		void SoundPlay(std::string soundName, bool roop = true, bool stop = false);
		void StopSound(std::string soundName);
		void AllStop();
		static Sound* Instance();
		static void Finalize();

	private:
		Sound() {};
		Sound(const Sound&) {};
		Sound& operator=(const Sound&) {}
		~Sound() {};
	};
}
