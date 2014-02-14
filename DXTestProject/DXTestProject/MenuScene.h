#pragma once

#include "Scene.h"

class MenuScene: public Scene
{
public:
	MenuScene();
	virtual ~MenuScene();

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

  virtual void OnEnter();
  virtual void OnExit();
};