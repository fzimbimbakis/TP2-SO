#include "memoryManager.h"

struct header{
	struct header * ptr;
	unsigned size;
}typedef header;

static header * firstHeader = BEGIN_MEM;

header * resizeFreeBlock(header * Header, unsigned size);

void init(){
	header base;
	base.ptr = NULL;
	base.size = END_MEM-BEGIN_MEM-sizeof(header);
	(* firstHeader) = base;
}

void * alloc(unsigned size){
	
	if(firstHeader==NULL)
		return NULL;

	// Check firstheader
	if((firstHeader->size) >= size){
		if((firstHeader->size)==size){
			header * aux = firstHeader + 1;
			firstHeader = firstHeader->ptr;
			return aux;
		}else{
			return resizeFreeBlock(firstHeader, size);
		}
	}




	header * Hpointer = firstHeader;
	while(Hpointer->ptr != NULL){
		// ptr tiene size
		if(Hpointer->ptr->size >= size){
			
			if(Hpointer->ptr->size==size){		// Saco el bloque
				header * aux = Hpointer->ptr;
				Hpointer->ptr = Hpointer->ptr->ptr;
				return aux + 1;
			}else{								// Achico el bloque
				header * Header = Hpointer->ptr;
				return resizeFreeBlock(Header, size);
			}

		} else Hpointer = Hpointer->ptr;
	}



	return NULL;
}

header * resizeFreeBlock(header * Header, unsigned size){
				Header->size -= (size + sizeof(header));
				header * newHeader =
				(
					(header *)
				((size_t)(Header) +
			 	(Header->size) + 
			 	sizeof(header))
				);
				newHeader->ptr=NULL;
				newHeader->size=size;
				return newHeader + 1;
}

// void * alloc(unsigned size){

// 	if((current-size) < BEGIN_MEM)
// 		return NULL;


// 	current-=size;

// 	return current;
// }

void free(void * ptr){}



// typedef long Align;
// // For alignment for long values ???????

// union header {
// 	struct
// 	{
// 		union header * ptr;
// 		unsigned size;
// 	} s;
// 	Align x;
// } ;
// typedef union header Header;

// static Header * freeList = 0x600000;

// void initial(){
// 	Header base;
// 	base.s.ptr = 0;
// 	base.s.size = (END_MEM - BEGIN_MEM)/sizeof(Header);
// 	*freeList = base;
// }

// void * alloc( unsigned nbytes){
// 	Header * Hpointer, * prevHpointer;
// 	unsigned nunits;

// 	nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;
// 	Hpointer = freeList;
// 	prevHpointer = freeList;
// 	if(freeList==NULL)
// 		return NULL;
// 	do
// 	{
// 		if(Hpointer->s.size >= nunits){
// 			if(Hpointer->s.size == nunits){

// 				if(prevHpointer != freeList)
// 				prevHpointer->s.ptr = Hpointer->s.ptr;
// 				else freeList = Hpointer->s.ptr;

// 			} else {

// 				Hpointer->s.size -= nunits;
// 				Hpointer += Hpointer->s.size;			
// 				Hpointer->s.size = nunits;
				
// 			}
// 			return (void *) (Hpointer+1);
// 		}
// 		if(Hpointer->s.ptr==NULL)
// 			return NULL;
// 		prevHpointer = Hpointer;
// 		Hpointer = Hpointer->s.ptr;
// 	} while (1);

// }


// void free(void * block){
// 	Header * blockPointer;

// 	blockPointer = (Header *) block - 1; // point to block header

// 	// No other free space
// 	if(freeList==NULL){
// 		freeList = blockPointer;
// 		return;
// 		}

// 	// Block before freeList
// 	if(blockPointer < freeList){
// 		if(blockPointer + blockPointer->s.size == freeList){
// 			blockPointer->s.ptr=freeList->s.ptr;
// 			blockPointer->s.size+=freeList->s.size;
// 			freeList = blockPointer;
// 		}else{
// 			blockPointer->s.ptr = freeList;
// 			freeList = blockPointer;
// 		}
// 		return;
// 	}

// 	Header * freeListPointer = freeList;

// 	// Block after freeList
// 	for (;!(blockPointer>freeListPointer && (freeListPointer->s.ptr==NULL || blockPointer<freeListPointer->s.ptr)); 
// 	freeListPointer=freeListPointer->s.ptr);

// 	// Join upper block
// 	if(freeListPointer->s.ptr!=NULL && blockPointer + blockPointer->s.size == freeListPointer->s.ptr){
// 		blockPointer->s.size += freeListPointer->s.ptr->s.size;
// 		blockPointer->s.size = freeListPointer->s.ptr->s.ptr;
// 	}else{
// 		blockPointer->s.ptr = freeListPointer->s.ptr;
// 	}

// 	// Join lower block
// 	if(freeListPointer + freeListPointer->s.size == blockPointer){
// 		freeListPointer->s.size += blockPointer->s.size;
// 		freeListPointer->s.ptr = blockPointer->s.ptr;
// 	}else{
// 		freeListPointer->s.ptr = blockPointer;
// 	}

// }
