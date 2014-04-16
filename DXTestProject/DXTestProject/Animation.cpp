#include "stdafx.h"
#include "Animation.h"
#include <stdio.h>

#include "rapidjson\rapidjson.h"
#include "rapidjson\filestream.h"
#include "rapidjson\document.h"

#include <string>
#include <sstream>
#include "ResourceManager.h"
#include "GraphicsManager.h"
#include "GameEngine.h"

#include "StringUtils.h"

Animation::Animation()
{
  mCurrentState = UNINITIALIZED;
  mTexture = NULL;
  mTextureSize = XMFLOAT2(0.0f, 0.0f);
}

Animation::~Animation()
{
  if (mTexture)
  {
    ResourceManager::GetInstance()->UnloadResource<ID3D11ShaderResourceView *>(&mTexture);
  }
}

void Animation::Initialize(const char *filename)
{
  FILE *fp = NULL;
  fopen_s(&fp, filename, "r");

  rapidjson::FileStream fileStream(fp);
  rapidjson::Document doc;
  doc.ParseStream<0>(fileStream);

  if (doc.HasMember("filename"))
  {
    mTextureFilename = StringUtils::GetWideStringFromCString(doc["filename"].GetString());
    mTexture = ResourceManager::GetInstance()->LoadTexture(mTextureFilename.c_str());

    D3D11_BUFFER_DESC animFrameDesc;
    ZeroMemory(&animFrameDesc, sizeof(animFrameDesc));
    animFrameDesc.ByteWidth = sizeof(XMFLOAT4);
    animFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    animFrameDesc.Usage = D3D11_USAGE_DEFAULT;

    ID3D11Device *dc = GameEngine::GetInstance()->GetGraphicsManager()->GetGraphicsDevice();
    HRESULT hr = dc->CreateBuffer(&animFrameDesc, NULL, &mAnimationFrameBuffer);
    assert(SUCCEEDED(hr));
  }

  if (doc.HasMember("size"))
  {
    rapidjson::Value &size = doc["size"];
    mTextureSize.x = size["width"].GetDouble();
    mTextureSize.y = size["height"].GetDouble();
  }

  if (doc.HasMember("animationData"))
  {
    AnimationFrameData frameData;
    frameData.startX = 0.0f;
    frameData.startY = 0.0f;
    frameData.width = mTextureSize.x;
    frameData.height = mTextureSize.y;

    std::pair<std::string, AnimationFrameData> animationPair("", frameData);
    mAnimationData.insert(animationPair);

    rapidjson::Value& animationData = doc["animationData"];
    rapidjson::Value::MemberIterator animItr = animationData.MemberBegin();
    for (; animItr != animationData.MemberEnd(); animItr++)
    {
      rapidjson::Value& animationFrame = animItr->value;

      frameData.startX = animationFrame["startX"].GetDouble();
      frameData.startY = animationFrame["startY"].GetDouble();
      frameData.width = animationFrame["width"].GetDouble();
      frameData.height = animationFrame["height"].GetDouble();

      std::pair<std::string, AnimationFrameData> animationPair(animItr->name.GetString(), frameData);
      mAnimationData.insert(animationPair);
    }
  }

  if (doc.HasMember("animations"))
  {
    std::vector<SequenceFrame> frames;

    SequenceFrame frame;
    frame.animationKey = "";
    frame.duration = 0.0f;
    frames.push_back(frame);

    AnimationSequence sequence;
    sequence.name = "";
    sequence.looping = false;
    sequence.frames = frames;

    std::pair<std::string, AnimationSequence> sequencePair(sequence.name, sequence);
    mAnimations.insert(sequencePair);

    rapidjson::Value& animations = doc["animations"];
    rapidjson::Value::ValueIterator animItr = animations.Begin();
    for (; animItr != animations.End(); animItr++)
    {
      if (animItr->IsObject())
      {
        frames.clear();
        rapidjson::Value& animation = *animItr;

        std::vector<SequenceFrame> frames;
        rapidjson::Value::ValueIterator frameItr = animation["frames"].Begin();
        for (; frameItr != animation["frames"].End(); frameItr++)
        {
          rapidjson::Value& frameValue = *frameItr;

          frame.animationKey = frameValue["key"].GetString();
          frame.duration = frameValue["duration"].GetDouble();

          frames.push_back(frame);
        }

        sequence.name = animation["name"].GetString();
        sequence.looping = animation["looping"].GetBool();
        sequence.frames = frames;

        sequencePair = std::pair<std::string, AnimationSequence>(sequence.name, sequence);
        mAnimations.insert(sequencePair);
      }
    }
  }
}

std::wstring Animation::GetTextureFilename()
{
  return mTextureFilename;
}

bool Animation::HasAnimation(const char *animationName)
{
  return mAnimations.find(animationName) != mAnimations.end();
}

void Animation::Play(const char *animationName)
{
  if (HasAnimation(animationName))
  {
    mCurrentAnimationName = animationName;
    mCurrentState = PLAYING;
  }
}

void Animation::Pause()
{
  if (mCurrentState == PLAYING)
  {
    mCurrentState = PAUSED;
  }
}

void Animation::Stop()
{
  if (mCurrentState == PLAYING || mCurrentState == PAUSED)
  {
    mCurrentFrameIndex = 0;
    mCurrentState = STOPPED;
  }
}

void Animation::Update(float dt)
{
  if (mCurrentState == PLAYING)
  {
    mAnimationTime -= dt;
    if (mAnimationTime <= 0.0f)
    {
      mCurrentFrameIndex++;
      AnimationSequence &sequence = mAnimations[mCurrentAnimationName];

      if (mCurrentFrameIndex >= sequence.frames.size() && !sequence.looping)
      {
        Stop();
      }
      else
      {
        mCurrentFrameIndex %= sequence.frames.size();
        mAnimationTime = sequence.frames[mCurrentFrameIndex].duration;
      }
    }
  }
}

void Animation::SetAnimationProperties(ID3D11DeviceContext *dc)
{
  AnimationSequence &currentSequence = mAnimations[mCurrentAnimationName];
  SequenceFrame &currentSequenceFrame = currentSequence.frames[mCurrentFrameIndex];
  AnimationFrameData currentFrame = mAnimationData[currentSequenceFrame.animationKey];

  // do setup for the next frame.
  XMFLOAT4 animationFrameData;
  animationFrameData.x = currentFrame.startX / mTextureSize.x;
  animationFrameData.y = currentFrame.startY / mTextureSize.y;
  animationFrameData.z = currentFrame.width / mTextureSize.x;
  animationFrameData.w = currentFrame.height / mTextureSize.y;

  dc->UpdateSubresource(mAnimationFrameBuffer, 0, NULL, &animationFrameData, 0, 0);
  dc->VSSetConstantBuffers(2, 1, &mAnimationFrameBuffer);
}
