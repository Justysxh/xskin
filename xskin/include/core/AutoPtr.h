#ifndef __SCOPED_PTR_H_INCLUDED__
#define __SCOPED_PTR_H_INCLUDED__

#include <assert.h>
#include <malloc.h>

template<class _Ty>
class XSKIN_API AutoPtr
{
public:
	explicit AutoPtr(_Ty* p = NULL) : ptr_(p) {}

	~AutoPtr()
	{
		enum { type_must_be_complete = sizeof(_Ty) };
		delete ptr_;
	}

	void Reset(_Ty* p = NULL)
	{
		if(p != ptr_)
		{
			enum { type_must_be_complete = sizeof(_Ty) };
			delete ptr_;
			ptr_ = p;
		}
	}

	_Ty& operator*() const
	{
		assert(ptr_ != NULL);
		return *ptr_;
	}

	_Ty* operator->() const
	{
		assert(ptr_ != NULL);
		return ptr_;
	}

	_Ty* Get() const { return ptr_; }

	bool operator==(_Ty* p) const { return ptr_ == p; }

	bool operator!=(_Ty* p) const { return ptr_ != p; }

	void Swap(AutoPtr& p2)
	{
		_Ty* tmp = ptr_;
		ptr_ = p2.ptr_;
		p2.ptr_ = tmp;
	}

	_Ty* Release()
	{
		_Ty* retVal = ptr_;
		ptr_ = NULL;
		return retVal;
	}

private:
	_Ty* ptr_;

	template<class C2> bool operator==(AutoPtr<C2> const& p2) const;

	template<class C2> bool operator!=(AutoPtr<C2> const& p2) const;

	AutoPtr(const AutoPtr&);

	void operator=(const AutoPtr&);
};

template<class _Ty>
void Swap(AutoPtr<_Ty>& p1, AutoPtr<_Ty>& p2)
{
	p1.Swap(p2);
}

template<class _Ty>
bool operator==(_Ty* p1, const AutoPtr<_Ty>& p2)
{
	return p1 == p2.Get();
}

template<class _Ty>
bool operator!=(_Ty* p1, const AutoPtr<_Ty>& p2)
{
	return p1 != p2.Get();
}


template<class _Ty>
class XSKIN_API AutoPtrArray
{
public:
	typedef _Ty element_type;

	explicit AutoPtrArray(_Ty* p = NULL) : array_(p) { }

	~AutoPtrArray()
	{
		enum { type_must_be_complete = sizeof(_Ty) };
		delete[] array_;
	}

	void Reset(_Ty* p = NULL)
	{
		if(p != array_)
		{
			enum { type_must_be_complete = sizeof(_Ty) };
			delete[] array_;
			array_ = p;
		}
	}

	_Ty& operator[](std::ptrdiff_t i) const
	{
		assert(i >= 0);
		assert(array_ != NULL);
		return array_[i];
	}

	_Ty* Get() const
	{
		return array_;
	}

	bool operator==(_Ty* p) const { return array_ == p; }

	bool operator!=(_Ty* p) const { return array_ != p; }

	void Swap(AutoPtrArray& p2)
	{
		_Ty* tmp = array_;
		array_ = p2.array_;
		p2.array_ = tmp;
	}

	_Ty* Release()
	{
		_Ty* retVal = array_;
		array_ = NULL;
		return retVal;
	}

private:
	_Ty* array_;

	template<class C2> bool operator==(AutoPtrArray<C2> const& p2) const;

	template<class C2> bool operator!=(AutoPtrArray<C2> const& p2) const;

	AutoPtrArray(const AutoPtrArray&);
	void operator=(const AutoPtrArray&);
};

template<class _Ty>
void Swap(AutoPtrArray<_Ty>& p1, AutoPtrArray<_Ty>& p2)
{
	p1.Swap(p2);
}

template<class _Ty>
bool operator==(_Ty* p1, const AutoPtrArray<_Ty>& p2)
{
	return p1 == p2.Get();
}

template<class _Ty>
bool operator!=(_Ty* p1, const AutoPtrArray<_Ty>& p2)
{
	return p1 != p2.Get();
}

#endif //! __SCOPED_PTR_H_INCLUDED__