#include "stdafx.h"
#include "BasicShaderModelBuilder.h"
#include "GraphicsIncludes.h"

BasicShaderModelBuilder::BasicShaderModelBuilder() { }

Model* BasicShaderModelBuilder::Create(const MODEL_DESC &modelDesc)
{
  SHADER_DESC vertexShaderDesc;
  vertexShaderDesc.filepath = L"./basicShader.fx";
  vertexShaderDesc.entryPoint = "BasicVertexShader";
  vertexShaderDesc.profile = "vs_4_0";

  SHADER_DESC pixelShaderDesc;
  pixelShaderDesc.filepath = L"./basicShader.fx";
  pixelShaderDesc.entryPoint = "BasicFragmentShader";
  pixelShaderDesc.profile = "ps_4_0";

  return ModelBuilder::Create(modelDesc, vertexShaderDesc, pixelShaderDesc);
}

void BasicShaderModelBuilder::Destroy(Model *model)
{
  ModelBuilder::Destroy(model);
}