/* ModelBuilder
 * Author: Justin Wilkinson
 * Purpose: A class to help with the creation of model objects for use in the world.
 * I tried to ensure that as much customization is available without overwhelming anyone trying to implement it.
 */

#pragma once

class Model;
struct D3D11_INPUT_ELEMENT_DESC;

struct MODEL_DESC
{
  void *modelData; // all of the actual vertices that make up the model.
  unsigned int sizeOfData; // the total size of all of the model data.
  unsigned int vertexCount; // how many individual vertices we're working with.
  D3D11_INPUT_ELEMENT_DESC *elementDescs; // the description of how the elements in the vertices map to semantics in the shader.
  unsigned int elementCount; // how many elements are in the description.
  unsigned int sizeOfVertexDesc; // how big each vertex is. (eg. Color (4b), Position (4b) = 8b)
};

struct SHADER_DESC
{
  const TCHAR *filepath; // the filepath of the shader (eg. L"./basicShader.fx")
  const char *entryPoint; // the 'main' entry point of the shader (eg. BasicVertexShader)
  const char *profile; // the profile with which to compile it for (eg. vs_4_0 = Vertex Shader, Shader Model 4.0)
};

class ModelBuilder
{
public:
  /** Creates a new Model object based on the descriptions given. 
   * @param modelDesc The description of the model to be made.
   * @param vertexDesc The description of the vertex shader to be made.
   * @param pixelDesc The description of the pixel shader to be made.
   * WARNING: ANY MODELS MADE USING THIS METHOD NEED TO BE FREED USING DESTROY.
   */
  static Model* Create(const MODEL_DESC &modelDesc, const SHADER_DESC &vertexDesc, const SHADER_DESC &pixelDesc);

  /** Frees the memory of a given model.
   * @param model The model to be cleaned up.
   */
  static void Destroy(Model *model);

protected:
  ModelBuilder();
};