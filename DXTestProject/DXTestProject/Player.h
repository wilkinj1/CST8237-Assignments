/* Player.h
 * Author: Justin Wilkinson
 * Purpose: A concrete class describing my player character.
 */

#pragma once

struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

class Player
{
public:
  Player();
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

 protected:
   ID3D11Buffer *mVertexBuffer;
   ID3D11VertexShader *mVertexShader;
   ID3D11PixelShader *mPixelShader;
   ID3D11InputLayout *mInputLayout;

   // This is a buffer that we'll use to pass our world-view-projection transformation to our shader.
   ID3D11Buffer *mWVPMatrixBuffer;
};