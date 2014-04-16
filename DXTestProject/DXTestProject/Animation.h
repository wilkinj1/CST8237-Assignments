#pragma once

#include <vector>
#include <map>
#include "GraphicsIncludes.h"

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct ID3D11DeviceContext;

struct AnimationFrameData
{
  float startX;
  float startY;
  float width;
  float height;
};

struct SequenceFrame
{
  std::string animationKey;
  float duration;
};

struct AnimationSequence
{
  std::string name;
  bool looping;
  std::vector<SequenceFrame> frames;
};

enum AnimationState
{
  UNINITIALIZED,
  LOADED,
  PLAYING,
  PAUSED,
  STOPPED
};

class Animation
{
public:
  Animation();
  ~Animation();

  void Initialize(const char *filename);

  bool HasAnimation(const char *animationName);
  void Play(const char *animationName);
  void Pause();
  void Stop();

  void Update(float dt);

  std::wstring GetTextureFilename();
  void SetAnimationProperties(ID3D11DeviceContext *dc);

protected:
  std::wstring mTextureFilename;
  ID3D11ShaderResourceView *mTexture;
  XMFLOAT2 mTextureSize;
  AnimationState mCurrentState;
  
  unsigned int mCurrentFrameIndex;
  std::string mCurrentAnimationName;
  float mAnimationTime;
  ID3D11Buffer *mAnimationFrameBuffer;

  std::map<std::string, AnimationFrameData> mAnimationData;
  std::map<std::string, AnimationSequence> mAnimations;
};