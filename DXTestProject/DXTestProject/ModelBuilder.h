/* ModelBuilder
 * Author: Justin Wilkinson
 * Purpose: A class to help with the creation of model objects for use in the world.
 * I tried to ensure that as much customization is available without overwhelming anyone trying to implement it.
 */

#pragma once

#include "Model.h"
#include "ResourceManager.h"

class ModelBuilder
{
public:
  /** Creates a new Model object based on the descriptions given. 
   * @param modelDesc The description of the model to be made.
   * @param vertexDesc The description of the vertex shader to be made.
   * @param pixelDesc The description of the pixel shader to be made.
   * WARNING: ANY MODELS MADE USING THIS METHOD NEED TO BE FREED USING DESTROY.
   */
  static Model* Create(MODEL_DESC & const modelDesc, SHADER_DESC & const vertexDesc, SHADER_DESC & const pixelDesc, TEXTURE_DESC & const textureDesc);

  /** Creates a new Model object based on the descriptions given.
  * @param modelDesc The description of the model to be made.
  * @param vertexDesc The description of the vertex shader to be made.
  * @param pixelDesc The description of the pixel shader to be made.
  * WARNING: ANY MODELS MADE USING THIS METHOD NEED TO BE FREED USING DESTROY.
  */
  static Model* Create(MODEL_DESC & const modelDesc, SHADER_DESC & const vertexDesc, SHADER_DESC & const pixelDesc);


  /** Frees the memory of a given model.
   * @param model The model to be cleaned up.
   */
  static void Destroy(Model *model);

protected:
  ModelBuilder();
};