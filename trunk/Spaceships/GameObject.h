#pragma once
#include "Collider.h"
#include "Mobile.h"
#include "IRenderable.h"
#include "IVisitor.h"
#define NO_PARENT -1

// no idea why the compiler wants me to forward declare this here, but 
// if you take this out it won't compile.  I will chase this down later
// but for now all I can say is that my love of managed languages without
// dll hell has increased by a full order of magnitude
class Controller;

enum ObjectType
{
	SPACESHIP = 0,
	ASTEROID,
	BULLET,
};

class GameObject
{
public:
	GameObject();
	virtual ~GameObject(void);
	int GetID();
	void SetID(int id);
	void Update();

	GameObject* GetParent();
	void SetParent(GameObject* value);
	
	//position
	void SetPosition(float x, float y);	

	//collision
	bool CollidesWith(GameObject* obj);
	
	Mobile* GetMobile();
	void SetMobile(Mobile* value);

	//lifetime- how long this thing lasts before it just goes away (-1 = infinity)
	void SetLifetime(int lifetime);
	void KillSlowly();
	virtual bool IsDead();
	void SendMessage(Message* msg);

	// process all messages.  This invokes the PreProcess and PostProcess accordingly and calls
	// ProcessMessage() for each message that has been passed to this thing.
	void ProcessMessages();
	
	// the controller that owns this object; useful for passing messages back to
	// the controller if necessary
	void SetController(Controller* owningController);

	// graphics stuff
	IRenderable* GetRenderable();
	void SetRenderable(IRenderable* renderable);

	// how much damage do I do if I hit something (0 = no damage)
	int GetDamage();
	void SetDamage(int value);

	// set a font in case a game object needs to draw something
	void SetFont(LPD3DXFONT font);

	virtual ObjectType GetType() = 0;

	void AcceptVisitor(IVisitor* visitor);

	void RemoveVisitor(IVisitor* visitor);

	void PassMessageDirect(Message* msg);
	
protected:	
	int _lifeTime; //set to -1 to live forever, otherwise the number of frames to keep this thing around
	// for inherited objects to be able to do things around message processing
	virtual void PreProcessMessages() = 0;
	virtual void PostProcessMessages() = 0;
	// derived classes are responsible for actually processing messages
	virtual void ProcessMessage(Message* msg) = 0;	

	virtual void PreGetRenderable() = 0;
	// called at the start of update
	virtual void OnUpdate() = 0;
	IRenderable* renderable;
	Mobile* mobile;
	Controller* owningController;
	float _turnstep;
	int hitPoints;
	int damage;
	LPD3DXFONT font;

private:
	int _ID;	
	list<Message*> messages;
	list<IVisitor*> visitors;
	GameObject* parent;
};

struct GameObjectFinder: public std::binary_function<GameObject*, int, bool>
{
public:
	bool operator () (GameObject* object, const int id) const 
	{
		return object->GetID() == id;	
	}
};

struct GameObjectWithMobile
{
public:
	bool operator() (GameObject* object)
	{
		return object->GetMobile() != NULL;
	}
};