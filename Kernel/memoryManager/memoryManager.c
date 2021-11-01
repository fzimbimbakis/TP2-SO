#include "../include/memoryManager.h"
#include "../include/naiveConsole.h"
struct header{
	struct header * ptr;
	unsigned size;
}typedef header;

static header * firstHeader = (header *)BEGIN_MEM;

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

void free(void * ptr){
    int debug = 0;
    if(debug)
    ncPrint("Free\n");
	header * Fptr = ((header *)ptr) - 1;		// Limite inferior
	header * limSuperior = (header *)((size_t)Fptr + Fptr->size + sizeof(header));

	if(limSuperior <= firstHeader){		// Va al principio
		if(limSuperior == firstHeader){
			Fptr->ptr = firstHeader->ptr;
			Fptr->size += firstHeader->size + sizeof(header);
			firstHeader = Fptr;
			return;
		}else{
			Fptr->ptr=firstHeader;
			firstHeader = Fptr;
			return;
		}
	}
	header * Hpointer = firstHeader;
	while(Hpointer!=NULL){

		if(Hpointer->ptr==NULL){		// Va al final
            if(debug)
            ncPrint("Va al final\n");
			if( ((size_t)Hpointer)+Hpointer->size+sizeof(header) == (size_t)Fptr){
                if(debug)
				ncPrint("Los pega\n");
				Hpointer->size += Fptr->size + sizeof(header);
			} else {
                if(debug)
				ncPrint("Estan separados\n");
				Hpointer->ptr = Fptr;
				Fptr->ptr = NULL;
			}
			return;
		}

		if(Hpointer->ptr >= Fptr){		// Va antes del siguiente header
            if(debug)
			ncPrint("Va antes del siguiente header\n");
			if( ((size_t)Hpointer)+Hpointer->size+sizeof(header) == (size_t)Fptr){		// Junto bloque inferior
                if(debug)
				ncPrint("Junto bloque inferior\n");
				Hpointer->size += Fptr->size + sizeof(header);
				// Check bloque superior también
				if(
					((size_t) Hpointer)+Hpointer->size+sizeof(header) == (size_t)Hpointer->ptr
				){
                    if(debug)
					ncPrint("Junto bloque inferior y superior\n");
					Hpointer->size += Hpointer->ptr->size + sizeof(header);
					Hpointer->ptr = Hpointer->ptr->ptr;
				}
				return;
			}

			if(																	// Junto SOLO con bloque superior
					(header*)(((size_t)Fptr) + Fptr->size + sizeof(header)) ==Hpointer->ptr
			) {
                if(debug)
				ncPrint("Junto SOLO bloque superior\n");
				Fptr->size += Hpointer->ptr->size + sizeof(header);
				Fptr->ptr = Hpointer->ptr->ptr;
				Hpointer->ptr = Fptr;
				return;
			}
			// No junto con nada
            if(debug)
			ncPrint("No junto\n");
			Fptr->ptr=Hpointer->ptr;
			Hpointer->ptr = Fptr;
			
			return;
		}
		Hpointer = Hpointer->ptr;

	}
	
	// ERROR
}

unsigned * memoryInfoMM(){
    unsigned * array = alloc(sizeof(unsigned ) * 3);
    unsigned totalMem = END_MEM - BEGIN_MEM;
    unsigned freeMem = 0;
    header * aux = firstHeader;
    while (aux!=NULL){
        freeMem += aux->size;
        aux = aux->ptr;
    }
    unsigned takenMem = totalMem - freeMem;
    array[0] = totalMem;
    array[1] = takenMem;
    array[2] = freeMem;
    return array;
}



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
