// ----------------------------------------------
// CREDIT
// All the codes here IN "resizable_array.h"
// and "resizable_array.c" are inherited from
// Prof. Garth Santor and Prof. Han Trinh
// Just names are altered, logic and algorithm are
// exactly the same as the "arrayADT" solution in class.
// ----------------------------------------------

// ----------------------------------------------
// IMPORT LIBRARIES
// ----------------------------------------------
#include <crtdbg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ----------------------------------------------
// DEFINE THE RESIZABLE ARRAY
// ----------------------------------------------
typedef double array_value_type;
typedef struct dynamic_array
{
	// pointer to the first element in underlying array
	array_value_type* data;
	// current size, # of elements greater than 0
	size_t nSize;
	// current capacity
	size_t nCapacity;
} resizable_array;

// ----------------------------------------------
// FUNCTION PROTOTYPES FOR RESIZABLE ARRAY
// ----------------------------------------------

// Constructor
resizable_array res_array();

// Adds extra elements
bool push(resizable_array* pArr, array_value_type value);

// Frees memory
void free_mem(resizable_array* pArr);
