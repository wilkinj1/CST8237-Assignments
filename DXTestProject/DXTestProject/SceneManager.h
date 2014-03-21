/* Scene
 * Author: Justin Wilkinson
 * Purpose: A basic system to help the developer handle any scenes they've created. It's a LIFO stack, so whichever
 * scene was pushed last is the one that is currently available.
 */

#pragma once

#include <vector>
#include "GraphicsIncludes.h"

class Scene;
struct ID3D11DeviceContext;

class SceneManager
{
public:
	SceneManager(void);
	virtual ~SceneManager(void);

  /** Update the top-most scene.
   * @param dt The amount of elapsed time since the last call to Update.
   */
	void Update(float dt);

  /** Paint the top-most scene.
   */
	void Paint();

  /** Pushes a new scene onto the scene stack.
   * @param scene The new scene
   */
  void PushScene(Scene *scene);

  /** Pops the top-most scene off of the stack.
   * @return The old top-most scene
   */
  Scene* PopScene();

  /** Pops the top-most scene off of the stack.
  * @return The old top-most scene
  */
  Scene* GetActiveScene();

  /** Pops all of the scenes of the stack.
   */
  void PopAllScenes();

private:
	std::vector<Scene *> mScenes;
};
