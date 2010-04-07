#include "GameView.h"
#include <stdio.h>

GameView::GameView(Screen* screen, Model* m, LPDIRECT3DDEVICE9 d3dd) : View(screen, m, d3dd)
{
	this->_model = (GameModel*) m;
	//load the background
	screenRect = screen->GetScreenSize();	
}

GameView::~GameView(void)
{
}


void GameView::Draw(){
	this->_dd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255,255,255), 1.0f, 0);
	HRESULT res;
		
	D3DXMATRIX mat;
	D3DXVECTOR3 pos(0,0,1);
	D3DXMatrixTransformation2D(&mat, NULL, NULL, NULL, NULL, NULL, NULL);

	_sprite->SetTransform(&mat);
	res = _sprite->Draw(this->background, &screenRect, NULL, NULL, D3DCOLOR_XRGB(255,255,255));
	_sprite->End();
	_sprite->Begin(D3DXSPRITE_ALPHABLEND);

	string position;		
	char buf[50];	
	position.append("screen.top: ");

	int ret = _snprintf(buf, 50, "%f", (float)this->screenRect.top);
	position.append(buf, ret);
	LogError(1, position);

	position.clear();
	position.append("screen.left: ");
	ret = _snprintf(buf, 50, "%f", (float)this->screenRect.left);
	position.append(buf, ret);
	LogError(2, position);

	IRenderable* renderable;

	while(renderables.size() > 0)
	{
		renderable = renderables.front();
		renderables.pop_front();
		renderable->RenderObject(this);
	}

	_sprite->Flush();
	LogError(3, "test");

	int vertexCount = points.size();
	// TODO:  learn more about directx so that this code can die
	if(vertexCount > 0)
	{
		this->_sprite->End();
		this->_sprite->Begin(D3DXSPRITE_ALPHABLEND);
		list<Vertex>::iterator iter;
		Vertex* vertices;
		
		vertices = new Vertex[vertexCount];

		int i = 0;
		for(iter = this->points.begin(); iter != points.end(); ++iter)
		{
			Vertex v = *iter;
			vertices[i] = v;
			i++;
		}

		assert(i == vertexCount);

		void* temp;
		this->pointBuffer->Lock(0, sizeof(Vertex) * vertexCount, (void**)&temp, 0);
		memcpy(temp, vertices, sizeof(Vertex) * vertexCount);
		pointBuffer->Unlock();
		_dd3dDevice->SetFVF(D3DFVF_SPACESHIPSVERTEX);
		_dd3dDevice->SetStreamSource(0, pointBuffer, 0, sizeof(Vertex) );	
		_dd3dDevice->DrawPrimitive(D3DPT_POINTLIST, 0, vertexCount);
		points.clear();
		delete vertices;
	}	
}

void GameView::InitRects()
{


}

void GameView::HandleMessage(Message* msg)
{
	if(msg->GetMessageType() == MessageType::GAME_OBJECT_ACTION)
	{
		// something that I should render
		if(msg->GetMessageContent() == MessageContent::SHOULD_RENDER)
		{
			IRenderable* renderable = (IRenderable*)msg->GetExtended();
			if(this->IsVisible(renderable))
			{
				this->renderables.push_back(renderable);
			}
		}
	}
	delete msg;
}

void GameView::SetPlayerPosition(int x, int y, int worldSizeX, int worldSizeY)
{
	RECT screenSize = screen->GetScreenSize();
	// set screen rect based on player position.  screen rect is in worldspace

	//figure out the top
	int leftedge = x - screenSize.right / 2;
	int topedge = y - screenSize.bottom / 2;

	if(leftedge < 0)
	{
		leftedge = 0;
	}

	if(leftedge >= worldSizeX - screenSize.right)
	{
		leftedge = worldSizeX - screenSize.right;
	}

	if(topedge < 0)
	{
		topedge = 0;
	}

	if(topedge >= worldSizeY - screenSize.bottom)
	{
		topedge = worldSizeY - screenSize.bottom;
	}

	
	screenRect.top = topedge;
	screenRect.left = leftedge;

	// now that we've determined the correct top left, set the bottom and right
	screenRect.bottom = screenRect.top + screenSize.bottom;
	screenRect.right = screenRect.left + screenSize.right;
}

void GameView::PreRender()
{
	_sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void GameView::PostRender()
{
	renderables.clear();
	_sprite->End();
}

bool GameView::IsVisible(IRenderable* r)
{
	// is this renderable set within the screen coordinate?
	D3DXVECTOR3 center = r->GetCenter();
	return ( (center.x >= this->screenRect.left) &&
		(center.x <= screenRect.right) &&
		(center.y >= screenRect.top) &&
		(center.y <= screenRect.bottom));
}