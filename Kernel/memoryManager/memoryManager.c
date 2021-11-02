// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../include/memoryManager.h"
#include "../include/naiveConsole.h"

struct header {
    struct header *ptr;
    unsigned size;
} typedef header;

static header *firstHeader = (header *) BEGIN_MEM;

header *resizeFreeBlock(header *Header, unsigned size);

void init() {
    header base;
    base.ptr = NULL;
    base.size = END_MEM - BEGIN_MEM - sizeof(header);
    (*firstHeader) = base;
}

void *alloc(unsigned size) {

    if (firstHeader == NULL)
        return NULL;

    // Check firstheader
    if ((firstHeader->size) >= size) {
        if ((firstHeader->size) == size) {
            header *aux = firstHeader + 1;
            firstHeader = firstHeader->ptr;
            return aux;
        } else {
            return resizeFreeBlock(firstHeader, size);
        }
    }


    header *Hpointer = firstHeader;
    while (Hpointer->ptr != NULL) {
        // ptr tiene size
        if (Hpointer->ptr->size >= size) {

            if (Hpointer->ptr->size == size) {        // Saco el bloque
                header *aux = Hpointer->ptr;
                Hpointer->ptr = Hpointer->ptr->ptr;
                return aux + 1;
            } else {                                // Achico el bloque
                header *Header = Hpointer->ptr;
                return resizeFreeBlock(Header, size);
            }

        } else Hpointer = Hpointer->ptr;
    }


    return NULL;
}

header *resizeFreeBlock(header *Header, unsigned size) {
    Header->size -= (size + sizeof(header));
    header *newHeader =
            (
                    (header *)
                            ((size_t)(Header) +
                             (Header->size) +
                             sizeof(header))
            );
    newHeader->ptr = NULL;
    newHeader->size = size;
    return newHeader + 1;
}

void free(void *ptr) {

    header *Fptr = ((header *) ptr) - 1;        // Limite inferior
    header *limSuperior = (header *) ((size_t) Fptr + Fptr->size + sizeof(header));

    if (limSuperior <= firstHeader) {        // Va al principio
        if (limSuperior == firstHeader) {
            Fptr->ptr = firstHeader->ptr;
            Fptr->size += firstHeader->size + sizeof(header);
            firstHeader = Fptr;
            return;
        } else {
            Fptr->ptr = firstHeader;
            firstHeader = Fptr;
            return;
        }
    }
    header *Hpointer = firstHeader;
    while (Hpointer != NULL) {

        if (Hpointer->ptr == NULL) {        // Va al final

            if (((size_t) Hpointer) + Hpointer->size + sizeof(header) == (size_t) Fptr) {

                Hpointer->size += Fptr->size + sizeof(header);
            } else {

                Hpointer->ptr = Fptr;
                Fptr->ptr = NULL;
            }
            return;
        }

        if (Hpointer->ptr >= Fptr) {        // Va antes del siguiente header

            if (((size_t) Hpointer) + Hpointer->size + sizeof(header) ==
                (size_t) Fptr) {        // Junto bloque inferior

                Hpointer->size += Fptr->size + sizeof(header);
                // Check bloque superior también
                if (
                        ((size_t) Hpointer) + Hpointer->size + sizeof(header) == (size_t) Hpointer->ptr
                        ) {

                    Hpointer->size += Hpointer->ptr->size + sizeof(header);
                    Hpointer->ptr = Hpointer->ptr->ptr;
                }
                return;
            }

            if (                                                                    // Junto SOLO con bloque superior
                    (header *) (((size_t) Fptr) + Fptr->size + sizeof(header)) == Hpointer->ptr
                    ) {

                Fptr->size += Hpointer->ptr->size + sizeof(header);
                Fptr->ptr = Hpointer->ptr->ptr;
                Hpointer->ptr = Fptr;
                return;
            }
            // No junto con nada
            Fptr->ptr = Hpointer->ptr;
            Hpointer->ptr = Fptr;

            return;
        }
        Hpointer = Hpointer->ptr;

    }

    // ERROR
}

unsigned *memoryInfoMM() {
    unsigned *array = alloc(sizeof(unsigned) * 3);
    if (array == 0)
        return 0;
    unsigned totalMem = END_MEM - BEGIN_MEM;
    unsigned freeMem = 0;
    header *aux = firstHeader;
    while (aux != NULL) {
        freeMem += aux->size;
        aux = aux->ptr;
    }
    unsigned takenMem = totalMem - freeMem;
    array[0] = totalMem;
    array[1] = takenMem;
    array[2] = freeMem;
    return array;
}

