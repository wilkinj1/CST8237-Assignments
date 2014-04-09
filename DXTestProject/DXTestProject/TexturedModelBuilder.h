#pragma once

#include "ModelBuilder.h"

class TexturedModelBuilder : public ModelBuilder
{
public:
  static Model* Create(MODEL_DESC & const modelDesc, TEXTURE_DESC & const textureDesc);
  static void Destroy(Model *model);

protected:
  TexturedModelBuilder();
};