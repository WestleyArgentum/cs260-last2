#pragma once
#include "IRenderable.h"
class RenderableBase : IRenderable
{
public:
	RenderableBase(void);
	~RenderableBase(void);

	// renders the object to the supplied view
	void RenderObject(View* view);

	// calculate center of this object
	virtual D3DXVECTOR3 GetCenter();

	virtual void SetLocalCoordinates(D3DXVECTOR3 localCoords);

	// object's position in world space (used for world transform.  that's right, I said world transform.
	// someone's been reading about 3D graphics)	
	virtual void SetPosition(float xValue, float yValue);

	// sets this object's rotation.  Rotation value should be between 0 and 2*PI
	virtual void SetRotation(float rotationValue);

	// Implementation of composite pattern.  This is not quite true to the pattern but it serves
	// the purpose of being able to render multiple objects associated with a single game object.
	void Add(IRenderable* render);

	virtual void SetOwner(GameObject* owner);

		
protected: 
	// get current position of this game object
	virtual float PositionX();
	virtual float PositionY();

	// things to do prior to rendering
	virtual void PreRender(View* view) = 0;

	// the view has things like the sprite and dd3dobject stuff in it that 
	// this can use to render stuff
	virtual void Render(View* view) = 0;

	// things to do after rendering, like restoring state if necessary
	virtual void PostRender(View* view) = 0;

	float positionX, positionY, rotation;
	D3DXVECTOR3 localCoordinates;
	float localRotation;
	GameObject* owner;

private:
	list<IRenderable*> renderables;
};

