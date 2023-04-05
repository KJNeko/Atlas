//
// Created by kj16609 on 4/4/23.
//

#ifdef TRACY_ENABLE

#include <tracy/Tracy.hpp>

void* operator new( std::size_t count )
{
	if(count == 0)
		++count;

	if(void* ptr = std::malloc(count))
	{
		TracySecureAlloc(ptr, count);
		return ptr;
	}

	throw std::bad_alloc();
}

void operator delete( void* ptr ) noexcept
{
	TracySecureFree( ptr );
	std::free( ptr );
}
#endif
