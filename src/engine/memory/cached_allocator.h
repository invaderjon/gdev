// cached_allocator.h
#ifndef INCLUDED_CACHED_ALLOCATOR
#define INCLUDED_CACHED_ALLOCATOR

#include <engine/containers/dynamic_array.h>
#include <engine/containers/map.h>
#include <limits>

namespace StevensDev
{

namespace sgdm
{

template <typename T>
class CachedAllocator : public IAllocator<T>
{
  private:
    // TYPES
    struct Block
    {
        T* ptr;
          // The pointer to the block.

        int count;
          // The number of items.
    };

    // MEMBERS
    DefaultAllocator<T> d_allocator;
      // The underlying allocator.

    sgdc::DynamicArray<Block> d_allocations;
      // The cached objects.

    sgdc::DynamicArray<Block> d_free;
      // The indices of the free items.

    // HELPER FUNCTIONS
    T* getFreeBlock( int count );
      // Gets a free block of the given size.
      // Returns nullptr if one could not be found.

    void reduce();
      // This reduces all contiguous blocks.

  public:
    // CONSTRUCTORS
    CachedAllocator();
      // Constructs a new allocator that caches instances.

    CachedAllocator( const CachedAllocator<T>& allocator );
      // Constructs a copy of the given allocator.

    CachedAllocator( CachedAllocator<T>&& allocator );
      // Moves the other allocator's resources to this instance.

    ~CachedAllocator();
      // Destructs the cached allocator.

    // OPERATORS
    CachedAllocator<T>& operator=( const CachedAllocator<T>& allocator );
      // Makes this a copy of the other allocator.

    CachedAllocator<T>& operator=( CachedAllocator<T>&& allocator );
      // Moves the allocator resources to this instance.

    // MEMBER FUNCTIONS
    virtual T* get( int count );
      // Gets a memory allocation for count instances of the given type from
      // the cache. If the cache is fully in use more memory will be allocated.
      //
      // Requirements:
      // count is greater than zero
      //
      // Behavior is undefined when:
      // Out of memory
      // T is void

    virtual void release( T* pointer, int count );
      // Releases the memory allocation at the given pointer location by
      // adding it to the free items.
      //
      // Requirements:
      // pointer is not equal to nullptr (zero)
      // count is greater than zero
      //
      // Behavior is undefined when:
      // T is void
      // pointer is otherwise invalid

    virtual void construct( T* pointer, const T& copy );
      // Constructs the object in place using the copy constructor.
      //
      // Behavior is undefined when:
      // T is void
      // pointer is equal to nullptr (zero) or is invalid

    virtual void construct( T* pointer, T&& copy );
      // Constructs the object in place using the move constructor.
      //
      // Behavior is undefined when:
      // T is void
      // pointer is equal to nullptr (zero) or is invalid

    virtual void destruct( T* pointer );
      // Call the destructor on an object.
      //
      // Behavior is undefined when:
      // T is void
      // pointer is equal to nullptr (zero) or is invalid
};

// FREE OPERATORS
template<typename T>
inline
std::ostream& operator<<( std::ostream& stream,
                          const CachedAllocator<T>& allocator )
{
    return stream << "{ }";
}

// CONSTRUCTORS
template <typename T>
inline
CachedAllocator<T>::CachedAllocator() : d_allocator(), d_allocations(),
                                        d_free()
{
}

template <typename T>
inline
CachedAllocator<T>::CachedAllocator(
        const sgdm::CachedAllocator<T>& allocator )
        : d_allocator( allocator.d_allocator ),
          d_allocations( allocator.d_allocations ),
          d_free( allocator.d_free )
{
}

template <typename T>
inline
CachedAllocator<T>::CachedAllocator( sgdm::CachedAllocator<T>&& allocator )
        : d_allocator( std::move( allocator.d_allocator ) ),
          d_allocations( std::move( allocator.d_allocations ) ),
          d_free( std::move( allocator.d_free ) )
{
}

template <typename T>
inline
CachedAllocator<T>::~CachedAllocator()
{
    Block block;
    while ( d_allocations.size() > 0 )
    {
        block = d_allocations.pop();
        d_allocator.release( block.ptr, block.count );
    }
}

// OPERATORS
template <typename T>
CachedAllocator<T>& CachedAllocator<T>::operator=(
        const sgdm::CachedAllocator<T>& allocator )
{
    d_allocator = allocator.d_allocator;
    d_allocations = allocator.d_allocations;
    d_free = allocator.d_free;

    return *this;
}

template <typename T>
CachedAllocator<T>& CachedAllocator<T>::operator=(
        sgdm::CachedAllocator<T>&& allocator )
{
    d_allocator = std::move( allocator.d_allocator );
    d_allocations = std::move( allocator.d_allocations );
    d_free = std::move( allocator.d_free );

    return *this;
}

// MEMBER FUNCTIONS
template <typename T>
T* CachedAllocator<T>::get( int count )
{
    assert( count > 0 );

    reduce();

    T* block = getFreeBlock( count );

    if ( block == nullptr )
    {
        // create the new allocation
        Block info;
        info.ptr = d_allocator.get( count );
        info.count = count;
        d_allocations.push( info );

        block = info.ptr;
    }

    return block;
}

template <typename T>
void CachedAllocator<T>::release( T* pointer, int count )
{
    assert( count > 0 );

    Block block;
    block.ptr = pointer;
    block.count = count;
    d_free.push( block );

    reduce();
}

template <typename T>
inline
void CachedAllocator<T>::construct( T* pointer, const T& copy )
{
    d_allocator.construct( pointer, copy );
}

template <typename T>
inline
void CachedAllocator<T>::construct( T* pointer, T&& copy )
{
    d_allocator.construct( pointer, copy );
}

template <typename T>
inline
void CachedAllocator<T>::destruct( T* pointer )
{
    d_allocator.destruct( pointer );
}

// HELPER FUNCTIONS
template <typename T>
T* CachedAllocator<T>::getFreeBlock( int count )
{
    if ( d_free.size() == 0 || count <= 0 )
    {
        return nullptr;
    }

    Block bestFit;
    bestFit.ptr = nullptr;
    bestFit.count = std::numeric_limits<int>::max();

    Block cur;
    unsigned int i;
    unsigned int best;
    for ( i = best = 0; i < d_free.size() && bestFit.count != count; ++i )
    {
        cur = d_free[i];
        if ( cur.count >= count && cur.count < bestFit.count )
        {
            best = i;
            bestFit = cur;
        }
    }

    if ( bestFit.ptr == nullptr )
    {
        return nullptr;
    }

    if ( bestFit.count > count )
    {
        Block remainder;
        remainder.ptr = bestFit.ptr + count;
        remainder.count = bestFit.count - count;
        d_free.push( remainder );
    }

    d_free.removeAt( best );

    return bestFit.ptr;
}

template <typename T>
void CachedAllocator<T>::reduce()
{
    if ( d_free.size() < 2 )
    {
        return;
    }

    unsigned int i;
    unsigned int j;
    for ( i = 0; i < d_free.size(); i++ )
    {
        for ( j = 0; j < d_free.size(); j++ )
        {
            if ( i == j )
            {
                continue;
            }

            if ( d_free[j].ptr == d_free[i].ptr + d_free[i].count )
            {
                d_free[i].count += d_free[j].count;
                d_free.removeAt( j );

                if ( i > j )
                {
                    --i;
                }
                else
                {
                    --j;
                }
            }
            else if ( d_free[j].ptr + d_free[j].count == d_free[i].ptr )
            {
                d_free[j].count += d_free[i].count;
                d_free.removeAt( i );

                if ( j > i )
                {
                    --i;
                }
                else
                {
                    --j;
                }
            }
        }
    }
}

} // End nspc sgdm

} // End nspc StevensDev

#endif
