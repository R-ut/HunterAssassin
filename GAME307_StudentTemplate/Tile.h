#ifndef TILE_H
#define TILE_H

#include "Node.h"
#include "Scene.h"

class Tile
{
private:
	float width, height;
	Uint8 r, g, b, a;
	Vec3 pos;
	Node* node;
	Scene* scene;




	bool explored = false;
	bool isPath = false;
	bool isWall = false;

	void setRGBA(Uint8 r_, Uint8 g_, Uint8 b_, Uint8 a_)
	{
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}

public:

	// Setters
	void setExplored(bool value) { explored = value; }
	void setPath(bool value) { isPath = value; }
	void setWall(bool value) { isWall = value; }


	Tile(Node* node_, Vec3 pos_, float width_, float height_, Scene* scene_);
	virtual ~Tile() {}
	void Render();
	Node* getNode() { return node; }
	Vec3 getPos() { return pos; }
};
#endif