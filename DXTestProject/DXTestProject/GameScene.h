/* GameScene.h
 * Author: Justin Wilkinson
 * Purpose: An concrete class representing my Game.
 */

#pragma once

#include "Scene.h"

class GameScene
{
public:
	virtual ~GameScene();

	/** Initialize our scene. 
	 */
	virtual void Initialize();

	/** Update our scene.
	 *	@param dt The current elapsed time since the last frame.
	 */
	virtual void Update(float dt);

	/** Render/draw our scene.
	 */
	virtual void Render();
};