#include "stdafx.h"
#include "AudioManager.h"
#include <assert.h>
#include <mmreg.h>

#include <XAudio2.h>

AudioManager::AudioManager() :
mAudioObj(NULL),
mMasteringVoice(NULL)
{

}

AudioManager::~AudioManager()
{
  if (mMasteringVoice)
  {
    mMasteringVoice->DestroyVoice();
    mMasteringVoice = NULL;
  }

  // Always release this last.
  if (mAudioObj)
  {
    mAudioObj->StopEngine();

    mAudioObj->Release();
    mAudioObj = NULL;
  }
}

void AudioManager::Initialize()
{
  HRESULT hr = XAudio2Create(&mAudioObj, 0, XAUDIO2_DEFAULT_PROCESSOR);
  assert(SUCCEEDED(hr));

  hr = mAudioObj->CreateMasteringVoice(&mMasteringVoice);
  assert(SUCCEEDED(hr));

  mAudioObj->StartEngine();


}