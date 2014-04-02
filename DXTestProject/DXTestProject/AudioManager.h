#pragma once

struct IXAudio2;
struct IXAudio2MasteringVoice;

class AudioManager
{
public:
  AudioManager();
  ~AudioManager();

  void Initialize();

private:
  IXAudio2 *mAudioObj;
  IXAudio2MasteringVoice *mMasteringVoice;
};