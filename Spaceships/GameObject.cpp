#include "GameObject.h"

GameObject::GameObject(void)
{
	_turnstep = .01f;
	mobile = NULL;
	renderable = NULL;
	parent = NULL;
	font = NULL;
}

GameObject::~GameObject(void)
{
	messages.clear();
	if(mobile != NULL)
	{
		delete mobile;
	}

	if(renderable != NULL)
	{
		delete renderable;
	}
}

int GameObject::GetID(){

	return this->_ID;
}

void GameObject::SetID(int id){

	this->_ID = id;
}

void GameObject::SetMobile(Mobile* value)
{
	assert(value != NULL);
	mobile = value;
	mobile->SetOwner(this);		
}

Mobile* GameObject::GetMobile()
{
	return mobile;
}

bool GameObject::CollidesWith(GameObject* obj){
	return false;

}

void GameObject::KillSlowly()
{
	if(_lifeTime > 0)
		_lifeTime--;
}

void GameObject::SetLifetime(int lifetime)
{
	this->_lifeTime = lifetime;
}

bool GameObject::IsDead()
{
	return this->_lifeTime == 0;
}

void GameObject::SendMessage(Message* msg)
{
	if(visitors.size() > 0)
	{
		// TODO:  passing a copy to each visitor could potentially result in multiple copies of the message being
		// sent back to this object, which is bad.  Find a way around this
		// pass a copy to each visitor to avoid leaking memory
		list<IVisitor*>::iterator visitorIter;
		for(visitorIter = visitors.begin(); visitorIter != visitors.end(); ++visitorIter)
		{
			((IVisitor*)*visitorIter)->PassMessage(new Message(*msg));
		}
		// very important to delete here to avoid leaking memory 
		delete msg;
	}
	else
	{
		PassMessageDirect(msg);
	}
}

void GameObject::ProcessMessages()
{
	PreProcessMessages();

	list<Message*>::iterator msgIter;	
	while(messages.size() > 0)
	{
		Message* message = messages.front();
		messages.pop_front();
		// TODO:  refactor this if statement into some sort of map based on message type so that
		// messages that are not object-specific can be handled in the base class instead of 
		// being passed to the derived class
		if(message->GetMessageType() == MessageType::GAME_OBJECT_ACTION && 
			message->GetMessageContent() == MessageContent::VISITOR_LEAVE)
		{
			RemoveVisitor((IVisitor*)message->GetExtended());
		}
		else
		{
			ProcessMessage(message);
		}
	}
	
	PostProcessMessages();
}

void GameObject::SetController(Controller* owningController)
{
	this->owningController = owningController;
}

void GameObject::SetRenderable(IRenderable* value)
{
	renderable = value;
}

IRenderable* GameObject::GetRenderable()
{
	PreGetRenderable();
	return this->renderable;
}

GameObject* GameObject::GetParent()
{
	return parent;

}
void GameObject::SetParent(GameObject* value)
{
	parent = value;
}

int GameObject::GetDamage()
{
	return damage;

}

void GameObject::SetDamage(int value)
{
	damage = value;
}

void GameObject::SetFont(LPD3DXFONT font)
{
	this->font = font;
}

void GameObject::AcceptVisitor(IVisitor* visitor)
{
	this->visitors.push_back(visitor);
	visitor->VisitorAccepted(this, owningController);
}

void GameObject::RemoveVisitor(IVisitor* visitor)
{
	this->visitors.remove(visitor);
}

void GameObject::PassMessageDirect(Message* msg)
{
	this->messages.push_back(msg);
}

void GameObject::Update()
{
	list<IVisitor*>::iterator visitorIter;
		for(visitorIter = visitors.begin(); visitorIter != visitors.end(); ++visitorIter)
		{
			((IVisitor*)*visitorIter)->Visit(this);
			((IVisitor*)*visitorIter)->ProcessMessages();
		}
	ProcessMessages();

	OnUpdate();
}