/* Scene
 * Author: Justin Wilkinson
 * Purpose: A basic representation of a 'scene', which is used to organize any conglomeration of objects. It's an abstract
 * class for use in many different ways. Need a level? Create a scene! Need a menu for the player to navigate to? Create a
 * scene! It provides basic functionality to allow the implementer initialize data, update information important to their
 * scene, and paint it when everything's ready. A camera is also provided for whatever use is desired.
 */

#pragma once

#include <vector>
#include "GraphicsIncludes.h"

class GameObject;
class Camera;
struct ID3D11DeviceContext;
struct ID3D11Device;

class Scene
{
public:
  Scene() : mIsInitialized(false) { }
  virtual ~Scene() { }

  /** Initializes the scene.
   */
	virtual void Initialize() = 0;

  /** Updates the scene.
   * @param dt The amount of time that has elasped since the last call to Update.
   */
	virtual void Update(float dt) = 0;

  /** Paints the scene.
   */
	virtual void Paint() = 0;

  virtual void HandleInput(float dt) { }

  /** Whether or not the scene has been initialized yet.
   */
	virtual bool isInitialized()
	{
		return mIsInitialized;
	}

  /** Accessor for the default camera bundles with the scene.
   * @return The default camera for the scene.
   */
	Camera *GetSceneCamera() 
	{
		return mSceneCamera;
	}

  /** A method that allows the developer to perform functionality when the game enters this scene.
   */
  virtual void OnEnter() = 0;

  /** A method that allows the developer to perform functionality when the game exits this scene.
   */
  virtual void OnExit() = 0;

protected:
	std::vector<GameObject *> mGameObjects;
	bool mIsInitialized;
	Camera *mSceneCamera;
};