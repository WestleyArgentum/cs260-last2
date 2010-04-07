#include "precompiled.h"
#include "RenderableBase.h"
#include "precompiled.h"
#include "IRenderable.h"

RenderableBase::RenderableBase(void)
{
	owner = NULL;
	SecureZeroMemory(&localCoordinates, sizeof(D3DXVECTOR3));
	positionX = 0;
	positionY = 0;
}

RenderableBase::~RenderableBase(void)
{
	GlobalDebug::GetInstance()->DbgOut("Deleting IRenderable base destructor");
	while(renderables.size() > 0)
	{
		IRenderable* temp = renderables.front();
		renderables.pop_front();
		delete temp;
	}
}

void RenderableBase::SetPosition(float xValue, float yValue)
{
	// must set position of everything in the stack
	positionX = xValue;
	positionY = yValue;
	list<IRenderable*>::iterator renderableIter;
	for(renderableIter = renderables.begin(); renderableIter != renderables.end(); ++renderableIter)
	{
		((IRenderable*)*renderableIter)->SetPosition(xValue, yValue);
	}

}

void RenderableBase::SetRotation(float rotationValue)
{
	rotation = rotationValue;
	list<IRenderable*>::iterator renderableIter;
	for(renderableIter = renderables.begin(); renderableIter != renderables.end(); ++renderableIter)
	{
		((IRenderable*)*renderableIter)->SetRotation(rotationValue);
	}
}

float RenderableBase::PositionX()
{
	return positionX + localCoordinates.x;
}

float RenderableBase::PositionY()
{
	return positionY + localCoordinates.y;
}

// default: return this object's position
D3DXVECTOR3 RenderableBase::GetCenter()
{
	D3DXVECTOR3 vector;
	vector.x = positionX;
	vector.y = positionY;
	vector.z = 1;
	return vector;
}

void RenderableBase::RenderObject(View* view)
{
	PreRender(view);
	list<IRenderable*>::iterator renderableIter;
	for(renderableIter = renderables.begin(); renderableIter != renderables.end(); ++renderableIter)
	{
		((IRenderable*)*renderableIter)->RenderObject(view);
	}
	Render(view);
	PostRender(view);

}

void RenderableBase::Add(IRenderable* render)
{
	this->renderables.push_back(render);
}

void RenderableBase::SetLocalCoordinates(D3DXVECTOR3 localCoords)
{
	this->localCoordinates = localCoords;
}

void RenderableBase::SetOwner(GameObject* owner)
{
	this->owner = owner;
}