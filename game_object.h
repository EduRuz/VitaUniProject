#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <graphics/mesh_instance.h>
#include <box2d/Box2D.h>

enum OBJECT_TYPE {PLAYER, ORB, KEY};

class GameObject : public gef::MeshInstance
{
public:
	void UpdateFromSimulation(const b2Body* body);

	inline void set_type(OBJECT_TYPE type) { type_ = type; }
	inline OBJECT_TYPE type() { return type_; }
	inline void set_yRotation(float y) { yRotation = y; }
	inline void setScale(float x, float y) { xScale = x; yScale = y; }

	void setid(int num) { id = num; }
	const int getid() { return id; }
private:
	OBJECT_TYPE type_;
	float yRotation = 0.0f;
	float xScale = 1.0f;
	float yScale = 1.0f;
	int id;

protected:

};

class Player : public GameObject
{
public:
	Player();
};

class Orb : public GameObject
{
public:
	Orb();

	int getNumofOrbs() { return numOfOrbs; }
	void setNumOfOrbs(int val) { numOfOrbs = val; }
	void decrementNumOfOrbs() { numOfOrbs--; }

private:
	int numOfOrbs;
};


class Key : public GameObject
{
public:
	Key();
};
#endif // _GAME_OBJECT_H