/* ModelUtils.h
* Author: Justin Wilkinson
* Purpose: To provide the user with a quick way to make simple geometric models.
*/

#pragma once

#include "GraphicsIncludes.h"
#include "Model.h"

class ModelUtils
{
public:
  /* Return a pre-made, fully UV-mapped cube made with the BasicShaderModelBuilder.
   * @return Pre-made cube model. 
   * WARNING: YOU MUST ENSURE THAT THE MEMORY IS FREED.
   */
  static Model* CreateCubeModelPCNT();

  static Model* CreateCubeModelPCNTWithTextureDesc(TEXTURE_DESC textureDesc);

  static Model* CreateCubeModelPCNTWithTextureFilename(wchar_t* filename);

  static Model* CreateSquareSprite(wchar_t* filename);

  static Model* CreateCubeModelPCNTWithAnimationFilename(wchar_t* filename);

  /* Destroys and frees any memory allocated using any of this class' creation method.
   * @param Model to destroy.
   */
  static void DestroyModel(Model *model);
};