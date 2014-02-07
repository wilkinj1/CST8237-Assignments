/* Player.h
 * Author: Justin Wilkinson
 * Purpose: A concrete class describing my player character.
 */

#pragma once

class Player
{
public:
	virtual ~Player();

	/** Initialize our object. 
	 */
	virtual void Initialize();

	/** Update our object.
	 *	@param dt The current elapsed time since the last frame.
	 */
	virtual void Update(float dt);

	/** Render/draw our object.
	 */
	virtual void Render();
};