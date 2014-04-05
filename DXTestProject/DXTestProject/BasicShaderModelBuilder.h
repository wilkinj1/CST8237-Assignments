#pragma once

#include "ModelBuilder.h"

class BasicShaderModelBuilder : public ModelBuilder
{
public:
  static Model* Create(MODEL_DESC & const modelDesc);
  static void Destroy(Model *model);

protected:
  BasicShaderModelBuilder();
};