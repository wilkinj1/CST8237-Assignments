#include "stdafx.h"
#include "AnimatedTexturedModelBuilder.h"
#include "Animation.h"
#include "GraphicsIncludes.h"
#include "StringUtils.h"

AnimatedTexturedModelBuilder::AnimatedTexturedModelBuilder() { }

Model* AnimatedTexturedModelBuilder::Create(MODEL_DESC & const modelDesc, wchar_t* filename)
{
  SHADER_DESC vertexShaderDesc;
  vertexShaderDesc.filepath = L"./basicAnimationShader.fx";
  vertexShaderDesc.entryPoint = "BasicVertexShader";
  vertexShaderDesc.profile = "vs_4_0";

  SHADER_DESC pixelShaderDesc;
  pixelShaderDesc.filepath = L"./basicAnimationShader.fx";
  pixelShaderDesc.entryPoint = "BasicFragmentShader";
  pixelShaderDesc.profile = "ps_4_0";

  Animation *animation = new Animation();
  animation->Initialize(StringUtils::GetCStringFromWideString((const wchar_t*)filename).c_str());

  D3D11_SAMPLER_DESC samplerDesc;
  ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
  samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

  TEXTURE_DESC textureDesc;
  wchar_t* textureFilename = new wchar_t[animation->GetTextureFilename().size() + 1];
  lstrcpy(textureFilename, animation->GetTextureFilename().c_str());

  textureDesc.filepath = textureFilename;
  textureDesc.samplerDesc = &samplerDesc;

  Model *newModel = ModelBuilder::Create(modelDesc, vertexShaderDesc, pixelShaderDesc, textureDesc);
  newModel->SetAnimation(animation);
  delete[] textureFilename;

  return newModel;
}

void AnimatedTexturedModelBuilder::Destroy(Model *model)
{
  // TODO: destroy animation.


  ModelBuilder::Destroy(model);
}