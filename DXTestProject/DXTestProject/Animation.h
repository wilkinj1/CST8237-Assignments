#pragma once

#include <vector>
#include <map>

struct AnimationFrame
{
  float startX;
  float startY;
  float width;
  float height;
};

class Animation
{
public:
  void Initialize(char *filename);

protected:
  std::vector<AnimationFrame> mAnimationData;
  std::map<std::string, std::vector<int> > mAnimations;
};