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
  /* Return a pre-made, fully UV-mapped cube.
   * @return Pre-made cube model. 
   * WARNING: YOU MUST ENSURE THAT THE MEMORY IS FREED.
   */
  static Model* CreateCubeModelPCNT();

  /* Destroys and frees any memory allocated using any of this class' creation method.
   * @param Model to destroy.
   */
  static void DestroyModel(Model *model);
};