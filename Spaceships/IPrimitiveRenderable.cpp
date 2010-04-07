#include "precompiled.h"
#include "IPrimitiveRenderable.h"

IPrimitiveRenderable::IPrimitiveRenderable(int bufferSize)
{
	this->bufferSize = bufferSize;
	vertices = new Vertex[bufferSize];
	bufferPosition = 0;
	vertexCount = 0;
}

IPrimitiveRenderable::IPrimitiveRenderable(const IPrimitiveRenderable& renderableToCopy)
{
	bufferSize = renderableToCopy.bufferSize;
	vertices = new Vertex[bufferSize];
	memcpy(vertices, renderableToCopy.vertices, sizeof(Vertex) * bufferSize);
	vertexCount = 0;
	bufferPosition = 0;
}

IPrimitiveRenderable::~IPrimitiveRenderable(void)
{
	delete vertices;
}

void IPrimitiveRenderable::PreRender(View* view)
{
	
}

void IPrimitiveRenderable::Render(View* view)
{
	RECT rect = view->GetViewRect();	
	float x = PositionX() - rect.left;
	float y = PositionY() - rect.top;

	// calculate screen position of these things
	for(int i = 0; i < bufferSize; i++)
	{
		Vertex temp;
		temp.x = vertices[i].x + x + localCoordinates.x;
		temp.y = vertices[i].y + y + localCoordinates.y;
		temp.z = 0.0f;
		temp.color = vertices[i].color;		
		view->AddPointToRender(temp);
	}	

	view->UnlockBuffer(bufferSize);
}

void IPrimitiveRenderable::PostRender(View* view)
{

}

void IPrimitiveRenderable::AddVertex(Vertex value)
{
	if(this->bufferPosition < this->bufferSize)
	{		
		vertices[bufferPosition] = value;
		bufferPosition++;
		vertexCount++;
	}
}