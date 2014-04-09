#include "stdafx.h"
#include "TexturedModelBuilder.h"
#include "GraphicsIncludes.h"

TexturedModelBuilder::TexturedModelBuilder() { }

Model* TexturedModelBuilder::Create(MODEL_DESC & const modelDesc, TEXTURE_DESC & const textureDesc)
{
  SHADER_DESC vertexShaderDesc;
  vertexShaderDesc.filepath = L"./basicShader.fx";
  vertexShaderDesc.entryPoint = "BasicVertexShader";
  vertexShaderDesc.profile = "vs_4_0";

  SHADER_DESC pixelShaderDesc;
  pixelShaderDesc.filepath = L"./basicShader.fx";
  pixelShaderDesc.entryPoint = "BasicFragmentShader";
  pixelShaderDesc.profile = "ps_4_0";

  return ModelBuilder::Create(modelDesc, vertexShaderDesc, pixelShaderDesc, textureDesc);
}

void TexturedModelBuilder::Destroy(Model *model)
{
  ModelBuilder::Destroy(model);
}