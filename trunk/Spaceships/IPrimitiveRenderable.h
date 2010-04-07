#pragma once
#include "RenderableBase.h"


class IPrimitiveRenderable :
	public RenderableBase
{
public:
	IPrimitiveRenderable(int bufferSize);
	IPrimitiveRenderable(const IPrimitiveRenderable& renderableToCopy);
	~IPrimitiveRenderable(void);
	
	// things to do prior to rendering
	virtual void PreRender(View* view);

	// the view has things like the sprite and dd3dobject stuff in it that 
	// this can use to render stuff
	virtual void Render(View* view);

	// things to do after rendering, like restoring state if necessary
	virtual void PostRender(View* view);	

	virtual void AddVertex(Vertex value);
protected:
	D3DXMATRIX worldTransform;
	D3DXMATRIX identityMatrix;

private:
	Vertex* vertices;
	// number of vertices currently in the buffer 
	// TODO: do we really need this if we have bufferSize and bufferPosition?
	int vertexCount;
	int bufferSize;
	int bufferPosition;
};

