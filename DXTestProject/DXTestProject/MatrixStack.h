/* MatrixStack
 * Author: Justin Wilkinson
 * Purpose: To provide and ease the usage of matrices when calculating a world matrix.
 * This is a LIFO (last-in, first-out) stack, which will allow users to continually push and pop matrices
 * relative to reference state, which is helpful when trying to calculate and maintain a world matrix.
 * As this is LIFO and not FIFO, calculations are done from the last element to the first, which is important
 * because that means you might be multiplying your matrices differently than is expected.
 *
 * eg. If I want to multiply a translation matrix, rotation matrix, and scaling matrix (in that order), I would
 * need to push the scaling matrix first, then the rotation, and then the translation. 
 * world = t * r * s, then push s, r, t.
 *
 * The benefit of this method is that if you're trying to position an object (B) relative to another object (A),
 * and you've pushed A's world matrix into the stack, then all you need to do is push B's matrices onto the stack;
 * since A is already in the stack, everything will be multiplied by it automatically. No hassle, no fuss.
 */

#pragma once

#include "GraphicsIncludes.h"
#include <stack>

class MatrixStack
{
public:
  /* Default constructor.
  */
  MatrixStack();

  /* Push a matrix onto the stack. 
   * @param matrix New matrix to push onto the stack. 
   */
  void PushMatrix(const XMMATRIX &matrix);

  /* Pop last matrix off of the stack.
   * @return The matrix popped off the stack.
   */
  const XMMATRIX& PopMatrix();

  /* Clear the matrix stack, and return it to an Identity state.
   */
  void Clear();

  /* Get the matrix that describes the current conglomeration of the stack.
   * @return A matrix that represents the current value of the stack.
   */
  XMMATRIX GetCurrentMatrix();

protected:
  XMFLOAT4X4 mCurrentMatrixValue;
  std::stack<XMFLOAT4X4> mMatrixValueStack;
};