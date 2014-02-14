/* SceneManager.h
 * Author: Justin Wilkinson
 * Purpose: 
 */

#pragma once

#include <vector>

class Scene;

class SceneManager
{
public:
	/** Initialize our scene manager. 
	 */
	void Initialize();

	/** Update our scene manager.
	 *	@param dt The current elapsed time since the last frame.
	 */
	void Update(float dt);

	/** Render/draw the scenes in our scene manager.
	 */
	void Render();

	/** Push our scene onto the scene stack.
	 *  @param scene The new scene to push onto our stack.
	 */
	void PushScene(Scene *scene);

	/** Pop the top scene off the scene stack.
	 */
	void PopScene();

  void PopAllScenes();

protected:
	/** The scenes that we're going to manage. */
	std::vector<Scene *> mScenes;
};