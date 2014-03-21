/* Camera
 * Author: Justin Wilkinson
 * Purpose: To provide a basic interface for portraying display information imperative to creating a 3D environment.
 * Without a view and projection matrix, there's no way to ensure that objects are where they need to be on on the screen
 * and within the game's world. The developer can create and implement their own cameras to provide additional 
 * functionality as they require.
 *
 * The field of view dictates how objects are stretched and skewed in the environment; for humans, PI/4 is about right,
 * whereas horses, and animals with eyes on the sides of their head would need a different value, as they can see a much
 * wider area. Think of a fish-eye camera lens in comparison to a normal lens.
 * The aspect ratio is so that we provide the user with a view that's relative to the size of their viewport.
 */

#pragma once

#include "GraphicsIncludes.h"

class Camera
{
public:
	Camera();

  /** A constructor that allows the developer to set all information pertinent to the camera.
   * @param fov The camera's field-of-view.
   * @param width The width of the camera's view.
   * @param height The height of the camera's view.
   * @param lookAtTarget The point in space that the camera should be looking at.
   * @param position The new position of the camera.
   * @param up The direction currently representing 'up'.
   */
	Camera(float fov, float width, float height, XMFLOAT4 lookAtTarget, XMFLOAT4 position, XMFLOAT4 up = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	~Camera();

  /** Get the current field-of-view.
   */
	float GetFOV();

  /** Set the current field-of-view.
   * @param fov The new field-of-view.
   */
	void SetFOV(float fov);

  /** Get the current aspect ratio.
   */
	float GetAspectRatio();

  /** Set the current aspect ratio.
   * @param fov The new aspect ratio.
   */
	void SetAspectRatio(float aspectRatio);

  XMFLOAT2 GetViewSize();
  void SetViewSize(float width, float height);

	XMFLOAT4 GetLookAtTarget();
	void SetLookAtTarget(XMFLOAT4 lookAtTarget);

	XMFLOAT4 GetPosition();
	void SetPosition(XMFLOAT4 position);

	XMFLOAT4 GetUpVector();
	void SetUpVector(XMFLOAT4 upVector);

	void Update(float dt);

  /** Get the current view matrix, which represents the window of which our player's will see the game's world.
   * @return The camera's view matrix.
   */
  XMMATRIX GetViewMatrix();

  /** Get the current projection matrix, which represents how objects will be presented in the game's world.
  * @return The camera's projection matrix.
  */
  XMMATRIX GetProjectionMatrix();

  /** Get a pre-multiplied matrix with represents both the view and the projection matrix.
   * @return The camera's view-projection matrix.
   */
	XMMATRIX GetViewProjectionMatrix();

protected:
	void CalculateViewMatrix();
	void CalculateProjectionMatrix();

	float mFOV;
	float mAspectRatio;
  XMFLOAT2 mViewSize;

	XMFLOAT4 mLookAtTarget;
	XMFLOAT4 mPosition;
	XMFLOAT4 mUpVector;

	XMFLOAT4X4 mViewMatrix;
	XMFLOAT4X4 mProjectionMatrix;
	XMFLOAT4X4 mViewProjectionMatrix;

	bool mRecalculateViewMatrix;
	bool mRecalculateProjectionMatrix;
};