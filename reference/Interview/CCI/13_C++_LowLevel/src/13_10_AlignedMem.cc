// Q. Write an aligned malloc and free function that supports allocating memory such that the memory
//    address returned is divisible by a specific power of two.

/* 
Checks:
	Typically, with malloc, we do not have control over where the memory is allocated within the heap.
	We just get a pointer to a block of memory which could start at any memory address within the heap.
	
	We need to work with these contraints by requesting enough memory that we can return a memory address
	which is divisible by the desired value.

	Ex) 100-byte chunk of memory, starting at an address that is multiple of 16. 
	    -> Allocate extra 15 bytes
*/

/*
// Pseducode
void *aligned_malloc_ps(size_t n_bytes, size_t alignment) {
	int offset = alignment -1;
	void *p = (void *) malloc(n_bytes+offset);
	void *q = (void *) malloc(( (size_t)(p) + offset) & ~(alignment -1));
	return q;
}
// -> how to free p?
*/

/*
You need an offset if you want to support alignments beyond what your system's malloc() does. For example if your system malloc() aligns to 8 byte boundaries, and you want to align to 16 bytes, you ask for 15 bytes extra so you know for sure you can shift the result around to align it as requested. You also add sizeof(void*) to the size you pass to malloc() to leave room for bookkeeping.

~(alignment - 1) is what guarantees the alignment. For example if alignment is 16, then subtract 1 to get 15, aka 0xF, then negating it makes 0xFF..FF0 which is the mask you need to satisfy the alignment for any returned pointer from malloc(). Note that this trick assumes alignment is a power of 2 (which practically it normally would be, but there really should be a check).

It's a void**. The function returns void*. This is OK because a pointer to void is "A pointer to any type," and in this case that type is void*. In other words, converting void* to and from other pointer types is allowed, and a double-pointer is still a pointer.

The overall scheme here is to store the original pointer before the one that's returned to the caller. Some implementations of standard malloc() do the same thing: stash bookkeeping information before the returned block. This makes it easy to know how much space to reclaim when free() is called.

All that said, this sort of thing is usually not useful, because the standard malloc() returns the largest alignment on the system. If you need alignment beyond that, there may be other solutions, including compiler-specific attributes.
*/

#include <stdio.h>
#include <stdlib.h>


void *aligned_malloc(size_t n_bytes, size_t alignment) {
	void* p1;	// original block
	void** p2;	// aligned block
	size_t offset = alignment -1 + sizeof(void*); 	// sizeof(void*) to store p1's address
	
	if((p1 = (void *) malloc(n_bytes+offset))== NULL) {
		return NULL;
	}

	// p2 as a void** (or an array of void*'s), we can just look at the index-1 to retrieve p1
	p2=(void**)(( (size_t)(p1)+offset) & ~(alignment-1));
	printf("(size_t)(p1)=%lu\n", (size_t)(p1));
	p2[-1]=p1;
	return p2;
}

void aligned_free(void *p2) {
	// for consistency, we use the same names as aligned_malloc
	// p2 as a void** (or an array of void*'s), we can just look at the index-1 to retrieve p1
	void *p1 = ((void**)p2)[-1];
	free(p1);
}

// Aligned (32 bits) memcpy and memmove
void myMemCpy(void *dest, void *src, size_t n)
{
   // Typecast src and dest addresses to (char *)
   char *csrc = (char *)src;
   char *cdest = (char *)dest;
   for (size_t i=0; i<n; i++)		// Copy contents of src[] to dest[]
       cdest[i] = csrc[i];
}

// It assumes that dst and src are misaligned at the same degree
/*
void aligned_memory_copy(void* dst, void* src, unsigned int bytes)
{
  unsigned char* b_dst = (unsigned char*)dst;
  unsigned char* b_src = (unsigned char*)src;

  // Copy bytes to align source pointer
  while ((b_src & 0x3) != 0)  {	// C++ compilation error
    *b_dst++ = *b_src++;
    bytes--;
  }

  unsigned int* w_dst = (unsigned int*)b_dst;
  unsigned int* w_src = (unsigned int*)b_src;
  while (bytes >= 4)  {
    *w_dst++ = *w_src++;
    bytes -= 4;
  }

  if (bytes > 0)  {		// Copy trailing bytes
    b_dst = (unsigned char*)w_dst;
    b_src = (unsigned char*)w_src;
    while (bytes > 0)    {
      *b_dst++ = *b_src++;
      bytes--;
    }
  }
}

*/
void myMemMove(void *dest, void *src, size_t n) // ?
{
   // Typecast src and dest addresses to (char *)
   char *csrc = (char *)src;
   char *cdest = (char *)dest;

   // Create a temporary array to hold data of src, to allow to overlap dest and src
   char *temp = new char[n];

   for (int i=0; i<n; i++)		// Copy data from csrc[] to temp[]
       temp[i] = csrc[i];

   for (int i=0; i<n; i++)		// Copy data from temp[] to cdest[]
       cdest[i] = temp[i];
    delete [] temp;
}

int main() {
	int *p=(int*)aligned_malloc(100*sizeof(int), 16);
	
	for(int i=0; i<100; i++) {
		p[i]=i;
	}
	for(int i=0; i<100; i++) {
		//;
		printf("p[%d]=%d\n", i, p[i]);
		//if(i>0 && i%16==0) printf("\n");
	}

	aligned_free(p);
}
