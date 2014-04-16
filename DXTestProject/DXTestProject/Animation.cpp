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
    AnimationFrame frame;
    frame.startX = 0.0f;
    frame.startY = 0.0f;
    frame.width = mTextureSize.x;
    frame.height = mTextureSize.y;

    std::pair<std::string, AnimationFrame> animationPair("", frame);
    mAnimationData.insert(animationPair);

    rapidjson::Value& animationData = doc["animationData"];
    rapidjson::Value::MemberIterator animItr = animationData.MemberBegin();
    for (; animItr != animationData.MemberEnd(); animItr++)
    {
      rapidjson::Value& animationFrame = animItr->value;

      frame.startX = animationFrame["startX"].GetDouble();
      frame.startY = animationFrame["startY"].GetDouble();
      frame.width = animationFrame["width"].GetDouble();
      frame.height = animationFrame["height"].GetDouble();

      std::pair<std::string, AnimationFrame> animationPair(animItr->name.GetString(), frame);
      mAnimationData.insert(animationPair);
    }
  }

  if (doc.HasMember("animations"))
  {
    std::vector<std::string> frames;
    frames.push_back("");
    std::pair<std::string, std::vector<std::string> > sequence("", frames);
    mAnimations.insert(sequence);

    rapidjson::Value& animations = doc["animations"];
    rapidjson::Value::ValueIterator animItr = animations.Begin();
    for (; animItr != animations.End(); animItr++)
    {
      if (animItr->IsObject())
      {
        frames.clear();
        rapidjson::Value& animation = *animItr;

        std::vector<std::string> frames;
        rapidjson::Value::ValueIterator frameItr = animation["frames"].Begin();
        for (; frameItr != animation["frames"].End(); frameItr++)
        {
          frames.push_back(frameItr->GetString());
        }

        sequence = std::pair<std::string, std::vector<std::string> >(animation["name"].GetString(), frames);
        mAnimations.insert(sequence);
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
    std::vector<std::string> &animationSequence = mAnimations[animationName];
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
    //mCurrentState = STOPPED;
  }
}

void Animation::Update(float dt)
{
  if (mCurrentState == PLAYING)
  {
    mAnimationTime -= dt;
    if (mAnimationTime <= 0.0f)
    {
      mAnimationTime = 1.0f;
      mCurrentFrameIndex++;
      if (mCurrentFrameIndex >= mAnimations[mCurrentAnimationName].size())
      {
        Stop();
      }      
    }
  }
}

void Animation::SetAnimationProperties(ID3D11DeviceContext *dc)
{
  std::vector<std::string> &currentSequence = mAnimations[mCurrentAnimationName];
  std::string &currentFrameKey = currentSequence[mCurrentFrameIndex];
  AnimationFrame currentFrame = mAnimationData[currentFrameKey];

  // do setup for the next frame.
  XMFLOAT4 animationFrameData;
  animationFrameData.x = currentFrame.startX / mTextureSize.x;
  animationFrameData.y = currentFrame.startY / mTextureSize.y;
  animationFrameData.z = currentFrame.width / mTextureSize.x;
  animationFrameData.w = currentFrame.height / mTextureSize.y;

  dc->UpdateSubresource(mAnimationFrameBuffer, 0, NULL, &animationFrameData, 0, 0);
  dc->VSSetConstantBuffers(2, 1, &mAnimationFrameBuffer);
}
