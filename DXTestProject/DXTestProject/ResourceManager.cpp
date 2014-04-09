#include "stdafx.h"
#include "ResourceManager.h"
#include <D3D11.h>
#include <D3DX11tex.h>
#include <assert.h>
#include <sstream>

#include "GameEngine.h"
#include "GraphicsManager.h"

ResourceManager* ResourceManager::sResourceManagerInstance = NULL;

ResourceManager* ResourceManager::GetInstance()
{
  if (sResourceManagerInstance == NULL)
  {
    sResourceManagerInstance = new ResourceManager();
  }

  return sResourceManagerInstance;
}

void ResourceManager::Shutdown()
{
  if (sResourceManagerInstance != NULL)
  {
    delete sResourceManagerInstance;
    sResourceManagerInstance = NULL;
  }
}

ResourceManager::ResourceManager() { }

ResourceManager::~ResourceManager() 
{
  ResourceMap copyMap = mLoadedResources;
  ResourceMap::iterator copyItr = copyMap.begin();
  for (; copyItr != copyMap.end(); copyItr++)
  {
    ID3D11DeviceChild *resource = copyItr->second.first;
    UnloadResource(&resource);
  }
}

ID3D11ShaderResourceView* ResourceManager::LoadTexture(TCHAR* filename)
{
  std::wstring keyString = filename;
  ID3D11ShaderResourceView *toReturn = nullptr;

  ResourceMap::iterator isResLoadedItr = mLoadedResources.find(keyString);
  if (isResLoadedItr != mLoadedResources.end())
  {
    // If we have it loaded, we just need to increase the reference, and then pass it on!
    ResourcePair &entry = isResLoadedItr->second;
    toReturn = (ID3D11ShaderResourceView *)entry.first;
    toReturn->AddRef();
    entry.second++;
  }
  else
  {
    ID3D11Device *device = GameEngine::GetInstance()->GetGraphicsManager()->GetGraphicsDevice();

    //If not, then we have to load it!
    D3DX11_IMAGE_LOAD_INFO imageInfo;
    HRESULT result = D3DX11CreateShaderResourceViewFromFile(device, filename, &imageInfo, NULL, (ID3D11ShaderResourceView **)&toReturn, NULL);

    if (FAILED(result))
    {
      printf("There was a problem loading \"%s\"\n", filename);
    }

    ResourcePair entry = ResourcePair((ID3D11DeviceChild *)toReturn, 1);
    mLoadedResources.insert(std::pair<std::wstring, ResourcePair>(keyString, entry));
  }

  return toReturn;
}

ID3D11VertexShader* ResourceManager::LoadVertexShaderAndInputLayout(SHADER_DESC desc, MODEL_DESC* input, ID3D11InputLayout** layout)
{
  return (ID3D11VertexShader *)LoadShaderAndInputLayout(desc, input, layout);
}

ID3D11PixelShader* ResourceManager::LoadPixelShader(SHADER_DESC desc)
{
  return (ID3D11PixelShader *)LoadShaderAndInputLayout(desc);
}

ID3D11DeviceChild* ResourceManager::LoadShaderAndInputLayout(SHADER_DESC desc, MODEL_DESC* input, ID3D11InputLayout** layout)
{
  ID3D11DeviceChild *toReturn = nullptr;
  bool success = true;
  std::string entryPointName = desc.entryPoint;
  wchar_t *entryPointWC = new wchar_t[entryPointName.size()];
  mbstowcs(entryPointWC, entryPointName.c_str(), entryPointName.size());

  std::wstringstream keyStream;
  keyStream << desc.filepath << L"-" << entryPointWC;

  std::wstringstream layoutKeyStream;
  layoutKeyStream << L"D3D11InputLayout-" << entryPointWC;

  ResourceMap::iterator isResLoadedItr = mLoadedResources.find(keyStream.str());
  if (isResLoadedItr != mLoadedResources.end())
  {
    // If we have it loaded, we just need to increase the reference, and then pass it on!
    ResourcePair &entry = isResLoadedItr->second;
    toReturn = (ID3D11DeviceChild *)entry.first;
    toReturn->AddRef();
    entry.second++;

    ResourceMap::iterator isLayoutLoadedItr = mLoadedResources.find(layoutKeyStream.str());
    if (isLayoutLoadedItr != mLoadedResources.end())
    {
      // If we have it loaded, we just need to increase the reference, and then pass it on!
      ResourcePair &layoutEntry = isLayoutLoadedItr->second;
      (*layout) = (ID3D11InputLayout *)layoutEntry.first;
      (*layout)->AddRef();
      layoutEntry.second++;
    }
  }
  else
  {
    ID3D11Device *device = GameEngine::GetInstance()->GetGraphicsManager()->GetGraphicsDevice();

    //If not, then we have to load it!
    ID3DBlob *shaderCode = NULL;
    ID3DBlob *errorMessage = nullptr;

    //HRESULT result = GraphicsManager::CompileShaderFromFile(desc.filepath, esc.entryPoint, desc.profile, &shaderCode, &errorMessage);
    HRESULT result = D3DX11CompileFromFile(desc.filepath, NULL, NULL, desc.entryPoint, desc.profile, 0, 0, NULL, &shaderCode, &errorMessage, NULL);

    std::string profile = desc.profile;
    if (profile.find("vs") != std::string::npos)
    {
      result = device->CreateVertexShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), NULL, (ID3D11VertexShader **)&toReturn);
      assert(SUCCEEDED(result));

      result = device->CreateInputLayout(input->elementDescs, input->elementCount, shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), layout);
      assert(SUCCEEDED(result));

      ResourcePair entry = ResourcePair((ID3D11DeviceChild *)*layout, 1);
      mLoadedResources.insert(std::pair<std::wstring, ResourcePair>(layoutKeyStream.str(), entry));
    }
    else
    {
      result = device->CreatePixelShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), NULL, (ID3D11PixelShader **)&toReturn);
    }

    if (errorMessage != NULL)
    {
      std::string output = (char *)errorMessage->GetBufferPointer();
      printf("There was a problem loading \"%s\": \n", desc.filepath, output.c_str());
      errorMessage->Release();

      shaderCode->Release();
      success = false;
    }
    else
    {
      ResourcePair entry = ResourcePair((ID3D11DeviceChild *)toReturn, 1);
      mLoadedResources.insert(std::pair<std::wstring, ResourcePair>(keyStream.str(), entry));
    }
  }

  delete[] entryPointWC;
  return toReturn;
}

bool ResourceManager::UnloadCachedResource(ID3D11DeviceChild **resource)
{
  bool isLoaded = false;
  ID3D11DeviceChild *ourResource = static_cast<ID3D11DeviceChild *>(*resource);
  if (ourResource)
  {
    ResourceMap::iterator isResLoadedItr = mLoadedResources.begin();
    for (; isResLoadedItr != mLoadedResources.end(); isResLoadedItr++)
    {
      ResourcePair &entry = isResLoadedItr->second;
      if (entry.first == ourResource)
      {
        // Clear the passed in resource; we don't want them to have access to this memory after we've released it.
        ourResource->Release();
        resource = NULL;

        // Reduce the reference count.
        entry.second--;

        // If we've run out of references..
        if (entry.second <= 0)
        {
          // Delete the key, and then erase the entry.
          mLoadedResources.erase(isResLoadedItr);
        }

        isLoaded = true;
        break;
      }
    }
  }

  return isLoaded;
}
