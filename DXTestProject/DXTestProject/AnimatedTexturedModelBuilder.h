#pragma once

#include "ModelBuilder.h"

class AnimatedTexturedModelBuilder : public ModelBuilder
{
public:
  static Model* Create(MODEL_DESC & const modelDesc, wchar_t* filename);
  static void Destroy(Model *model);

protected:
  AnimatedTexturedModelBuilder();
};