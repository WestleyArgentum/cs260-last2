#pragma once

template <class T> class SmartPtr;

template <class T> class IRefCount
{

	friend class SmartPtr<T>;
protected:
		virtual void AddReference() = 0;
		virtual void RemoveReference() = 0;
		const virtual T* GetPtr() = 0;
};

template <class T> class IRefCountImpl : public IRefCount<T>
{
protected:
	virtual void AddRef() { count++; }
	virtual void Release(){
		assert(count >= 0);
		count--;
		if(count <= 0)
		{
			Destroy();
		}
	}
	const virtual T* GetPtr() {return ((T*)this); }
	virtual void Destroy() 
	{
		if(GetPtr() != NULL)
		{
			delete GetPtr();
		}
	}

private:
	int count;

};


template <class T> class SmartPtr
{
public:
	
	SmartPtr()
	{
		refCount = NULL;
	}

	SmartPtr(T* ptr)
	{
		refCount = NULL;
		Assign(ptr);
	}

	SmartPtr(const SmartPtr& sp)
	{
		refCount = NULL;
		Assign(sp.refCount);
	}

	virtual ~SmartPtr()
	{
		Assign((IRefCount<T>*)NULL);
	}

	const T* GetPtr()
	{
		return refCount == NULL ? NULL : refCount->GetPtr();
	}

	SmartPtr& operator = (const SmartPtr& sp)
	{
		Assign(sp.refCount);
		return *this;
	}

	SmartPtr& operator = (T* ptr)
	{
		Assign(ptr);
		return *this;
	}

	T* operator ->()
	{
		assert(GetPtr() != NULL);
		return GetPtr();
	}

	operator T* () const
	{
		return GetPtr();
	}

	bool operator !()
	{
		return GetPtr() == NULL;
	}

	bool operator ==(const SmartPtr& sp)
	{
		return GetPtr() == sp.GetPtr();
	}

	bool operator !=(const SmartPtr& sp)
	{
		return GetPtr() != sp.GetPtr();
	}
	

private:
	IRefCount<T> *refCount;

	class RefCounter : public IRefCountImpl<T>
	{
	public:
		RefCounter(T* ptr) 
		{
			this->ptr = ptr;
		}
		virtual ~RefCounter()
		{
			IRefCountImpl<T>::Destroy();
		}
	protected:
		const virtual T* GetPtr()
		{
			return ptr;
		}

		virtual void Destroy()
		{
			delete this;
		}

	private:
		T* ptr;
	};

	void Assign(void *ptr)
	{
		if(ptr == NULL)
		{
			Assign((IRefCount<T>*)NULL);
		}
		else
		{
			Assign(new RefCounter(static_cast<T*>(ptr)));
		}
	}

	void Assign(IRefCount<T>* refCount)
	{
		if(refCount != NULL)
		{
			refCount->AddRef();
		}

		IRefCount<T>* oldRef = this->refCount;
		this->refCount = refCount;
		if(oldRef != NULL)
		{
			oldref->Release();
		}
	}


};