#pragma once

#include "ModelBuilder.h"

class BasicShaderModelBuilder : public ModelBuilder
{
public:
  static Model* Create(const MODEL_DESC &modelDesc);
  static void Destroy(Model *model);

protected:
  BasicShaderModelBuilder();
};