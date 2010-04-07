///////////////////////////////////////////////////////////////////////////////
///
///	\file ObjectLinkedList.h
///	
///	Authors: Chris Peters, Benjamin Ellinger
///	Copyright 2009, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#pragma once

namespace Framework
{
	///	Intrusively linked list for objects. This enables the ability to store 
	/// objects in an efficient linked list structure without duplicating code. 
	/// Object to be used in this container must have Prev and Next pointers as
	/// members.
	template< typename type >
	class ObjectLinkList
	{
	public:
		typedef type * pointer;
		typedef type& reference;

		ObjectLinkList()
		{
			First = NULL;
			Last = NULL;
		}

		void push_back(type * object)
		{
			if( Last == NULL )
			{
				First = object;
				Last = object;
				object->Next = NULL;
				object->Prev = NULL;
			}
			else
			{
				object->Prev = Last;
				object->Next = NULL;
				Last->Next = object;
				Last = object;
			}
		}

		void erase(type * object)
		{
			if( object->Next == NULL && object->Prev == NULL)
			{
				//Only object in list
				Last = NULL;
				First = NULL;
			}
			else if( object->Next == NULL )
			{
				//Object is last
				Last = object->Prev;
				if( Last ) Last->Next = NULL;

			}
			else if( object->Prev == NULL )
			{
				//Object is first
				First = object->Next;
				if( First ) First->Prev = NULL;
			}
			else
			{
				//Object is in middle
				object->Prev->Next = object->Next;
				object->Next->Prev = object->Prev;
			}
		}

		///Intrusive linked list iterator
		class iterator
		{
		public:
			friend class ObjectLinkList;
			iterator(){};
			iterator(pointer p) : item(p) {}
			void operator--(){item=item->Prev;}
			void operator++(){item=item->Next;}
			reference operator*(){return *item;}
			pointer operator->(){return item;}
			bool operator==(const iterator& i){return item==i.item;}
			bool operator!=(const iterator& i){return item!=i.item;}
			operator bool(){return item!=NULL;}
			operator pointer(){return item;}
			pointer GetPtr(){return item;}
		private:
			pointer item;
		};

		iterator begin(){ return First; }
		iterator end(){ return NULL;}
		pointer last(){ return Last; }
	private:
		type * First;
		type * Last;
	};
}
