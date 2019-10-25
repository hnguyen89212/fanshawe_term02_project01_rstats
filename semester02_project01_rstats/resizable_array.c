#include "resizable_array.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _M_X64
void* const DBG_PTR = (void* const)(0xcccccccccccccccc);
#else
void* const DBG_PTR = (void* const)(0xcccccccc);
#endif

// Constructor
resizable_array res_array()
{
	// initially, set the pointer to NULL pointer
	// set both size and capacity to 0
	resizable_array arr = { NULL, 0, 0 };
	return arr;
}

bool push(resizable_array* pArr, array_value_type value)
{
	// assert that there is no abnormality
	assert(pArr != DBG_PTR && pArr != NULL && "Error: there is no resizable_array object");
	assert(pArr->data != DBG_PTR && "array_push_back: array has not been initialized");
	assert(pArr->nSize <= pArr->nCapacity && "array size > array capacity");
	// if the current size = the current capacity
	// we need to increment the capacity to double to old capacity
	if (pArr->nSize == pArr->nCapacity)
	{
		size_t newCapacity = (pArr->nCapacity) * 2;
		// we handle the initial state, where the nCapacity = 0 --> newCapacity = 2 * 0 = 0
		// if that holds true, increment 1
		if (newCapacity == 0)
		{
			newCapacity++;
		}
		// dynamically resizes a memory block previously created (pArr->data)
		array_value_type* newData = realloc(pArr->data, newCapacity * (sizeof(array_value_type)));
		// handles the case where there is no memory block large enough as requested
		if (newData == NULL)
		{
			printf("Error: there is no enough memory");
			// short circuit, return false to signal failure
			return false;
		}
		// at this point, everything is fine
		// reassigns the underlying pointer data to newData
		pArr->data = newData;
		// sets the capacity to new one
		pArr->nCapacity = newCapacity;
	}
	// here, add the new value to the underlying array and increments the size
	pArr->data[pArr->nSize] = value;
	pArr->nSize++;
	// at this point, everything works, return true to signal success
	return true;
}

void free_mem(resizable_array* pArr)
{
	// assert that there is no abnormality
	assert(pArr != DBG_PTR && pArr != NULL && "No array_t object");
	assert(pArr->data != DBG_PTR && "array_free: array has NOT been initialized");
	assert(pArr->data != NULL && "memory block already deleted");
	free(pArr->data);
	*pArr = res_array();
}
