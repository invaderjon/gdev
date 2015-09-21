// iallocator.h
#ifndef INCLUDED_IALLOCATOR
#define INCLUDED_IALLOCATOR

namespace StevensDev
{

namespace sgdm
{

template<typename T>
class IAllocator
{
  public:
    // CONSTRUCTORS
    virtual ~IAllocator() = 0;
      // The virtual destructor.

    // MEMBER FUNCTIONS
    virtual T* get( int count ) = 0;
      // Gets a memory allocation for count instances of the given type.
      //
      // Behavior is undefined when:
      // T is void
      // count is less than or equal to zero
      // out of memory

    virtual void release( T* pointer, int count ) = 0;
      // Releases the memory allocation at the given pointer location.
      //
      // Behavior is undefined when:
      // T is void
      // pointer is equal to nullptr (zero) or is invalid
      // count is less than or equal to zero

    virtual void construct( T* pointer, const T& copy ) = 0;
      // Constructs the object in place using the copy constructor.
      //
      // Behavior is undefined when:
      // T is void
      // pointer is equal to nullptr (zero) or is invalid

    virtual void construct( T* pointer, T&& copy ) = 0;
      // Constructs the object in place using the move constructor.
      //
      // Behavior is undefined when:
      // T is void
      // pointer is equal to nullptr (zero) or is invalid

    virtual void destruct( T* pointer ) = 0;
      // Call the destructor on an object.
      //
      // Behavior is undefined when:
      // T is void
      // pointer is equal to nullptr (zero) or is invalid
};

template<typename T>
inline
IAllocator<T>::~IAllocator()
{
}

} // End nspc sgdm

} // End nspc StevensDev

#endif
