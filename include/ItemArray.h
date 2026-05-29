#ifndef ITEM_ARRAY
#define ITEM_ARRAY

#include <cstddef>

#include "Item.h"

struct ItemArray
{
    Item* data;
    size_t size;
    size_t capacity;
};

void push(ItemArray* array, Item element);
Item pop(ItemArray* array);

size_t find(ItemArray* array, Item target);

void reserve(ItemArray* array, size_t size);

ItemArray initArray(size_t size);
void freeArray(ItemArray* array);

#endif
