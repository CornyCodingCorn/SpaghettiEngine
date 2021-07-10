#pragma once
#include "Sound.h"
#include <random>
#include <initializer_list>

class AudioPlayer
{
public:
	AudioPlayer() = default;
	AudioPlayer(const std::initializer_list<std::wstring>& wavFiles, float freqDev, float masterVol, unsigned int seed);

	void PlayAt(float vol, int pos);
	void PlayRandom(float vol);
	void PlayAll(float vol);

	void ContinueAt(int pos);
	void ContinueAll();

	void PauseAt(int pos);
	void PauseAll();

	void StopAt(int pos);
	void StopAll();

	void ChangeVolumeAt(float vol, int pos);
	void ChangeVolumeAll(float vol);
	void ChangeMasterVolume(float vol);
private:
	float masterVolume = 1.0f;
	std::mt19937 rng;
	std::uniform_int_distribution<unsigned int> soundDist;
	std::normal_distribution<float> freqDist;
	std::vector<Sound> sounds;
};