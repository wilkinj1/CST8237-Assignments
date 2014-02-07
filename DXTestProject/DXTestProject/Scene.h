/* Scene.h
 * Author: Justin Wilkinson
 * Purpose: An abstract representation of a scene for use by our engine.
 */

#pragma once

#include <vector>

class GameObject;

class Scene
{
public:
	virtual ~Scene();

	/** Initialize our scene. 
	 */
	virtual void Initialize() = 0;

	/** Update our scene.
	 *	@param dt The current elapsed time since the last frame.
	 */
	virtual void Update(float dt) = 0;

	/** Render/draw our scene.
	 */
	virtual void Render() = 0;

protected:
	/** The game objects that our scene is managing. */
	std::vector<GameObject *> mGameObjects;
};