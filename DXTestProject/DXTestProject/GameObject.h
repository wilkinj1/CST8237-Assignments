/* GameObject.h
 * Author: Justin Wilkinson
 * Purpose: An abstract representation of an object for use by our engine.
 */

#pragma once

class GameObject
{
public:
	virtual ~GameObject();

	/** Initialize our object. 
	 */
	virtual void Initialize() = 0;

	/** Update our object.
	 *	@param dt The current elapsed time since the last frame.
	 */
	virtual void Update(float dt) = 0;

	/** Render/draw our object.
	 */
	virtual void Render() = 0;
};