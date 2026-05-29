#include "ItemArray.h"
#include "Item.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>

void push(ItemArray* array, Item element)
{
    if (array->size + 1 > array->capacity)
    {
        array->capacity *= 2;
        Item* data {(Item*)std::realloc(array->data, array->capacity*8)};

        if (data == nullptr)
        {
            std::cout << "Realloc failed, exiting!";
            std::exit(-1);
        }

        array->data = data;
    }

    array->data[array->size] = element;
    array->size++;
}

Item pop(ItemArray* array)
{
    if (array->size == 0)
        return Item{0, 0};

    array->size--;

    return array->data[array->size];
}

size_t find(ItemArray* array, Item target)
{
    for(size_t i{0}; i < array->size; i++)
    {
        if (array->data[i].item_id == target.item_id)
            return i;
    }

    return array->size;
}

void reserve(ItemArray* array, size_t size)
{
    array->capacity = size;

    Item* data {(Item*)std::realloc(array->data, size*8)};

    if (data == nullptr)
    {
        std::cout << "Failed to reallocate. Exiting!";
        std::exit(-1);
    }

    array->data = data;
}

ItemArray initArray(size_t size)
{
    ItemArray array;

    if (size > 0)
        array = {(Item*)malloc(size*8), size, size};
    else
        array = {(Item*)malloc(16), 0, 2};

    if (array.data == nullptr)
    {
        std::cout << "Malloc failed, what the fuck!";
        std::exit(-1);
    }

    return array;
}

void freeArray(ItemArray* array)
{
    free(array->data);

    array->data = nullptr;
    array->size = 0;
    array->capacity = 0;
}
