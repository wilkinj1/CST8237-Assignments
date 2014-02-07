#include "stdafx.h"
#include "Player.h"
#include <d3d11.h>

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3(float x, float y, float z):
		x(x), y(y), z(z) { }
}

struct VertexDescription
{
	Vector3 position;
	Vector3 color;

	VertexDescription(Vector3 pos, Vector3 col):
		position(pos.x, pos.y, pos.z),
		color(col.x, col.y, col.z) { }
};

Player::~Player()
{

}

void Player::Initialize()
{
	VertexDescription vertices[] = 
	{
		VertexDescription(/*position*/ Vector3(0.0f, 0.0f, 0.0f), /*color*/ Vector3(1.0f, 0.0f, 0.0f)),
		VertexDescription(/*position*/ Vector3(1.0f, 0.0f, 0.0f), /*color*/ Vector3(1.0f, 0.0f, 0.0f)),
		VertexDescription(/*position*/ Vector3(0.0f, 1.0f, 0.0f), /*color*/ Vector3(1.0f, 0.0f, 0.0f)),
	};
	int numberOfVertices = sizeof(vertices) / sizeof(VertexDescription);
}

void Player::Update(float dt)
{

}

void Player::Render()
{

}