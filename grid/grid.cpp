#include "grid.h"

grid::grid(int dimension)
{
    dim = dimension;
    arr = new int[dim*dim];

    parent = nullptr;
    for(int i = 0; i < 4; i++)
        child[i] = nullptr;
}

grid::grid(const grid &other)
{
    dim = other.dim;
    arr = new int[dim*dim];
    for(int i = 0; i < dim*dim; i++)
        arr[i] = other.arr[i];

    parent = nullptr;
    for(int i = 0; i < 4; i++)
        child[i] = nullptr;
}

grid& grid::operator =(const grid& other)
{
    dim = other.dim;
    if(arr)
        delete[] arr;
    arr = new int[dim*dim];
    for(int i = 0; i < dim*dim; i++)
        arr[i] = other.arr[i];

    parent = nullptr;
    for(int i = 0; i < 4; i++)
        child[i] = nullptr;
}

grid::~grid()
{
    if(arr)
        delete[] arr;
    arr = nullptr;
    dim = 0;

    parent = nullptr;
    for(int i = 0; i < 4; i++)
        child[i] = nullptr;
}

int& grid::operator [](int i)
{
    return arr[i];
}

void grid::setDim(int dimension)
{
    dim = dimension;
    if(arr)
        delete[] arr;
    arr = new int[dim*dim];
}

bool operator ==(const grid& lhs, const grid& rhs)
{
    if(lhs.dim == rhs.dim)
    {
        for(int i = 0; i < lhs.dim*lhs.dim; i++)
            if(lhs.arr[i] != rhs.arr[i])
                return false;
        return true;
    }
    return false;
}

bool operator !=(const grid& lhs, const grid& rhs)
{
    if(lhs.dim == rhs.dim)
    {
        int count = 0;
        for(int i = 0; i < lhs.dim*lhs.dim; i++)
            if(lhs.arr[i] == rhs.arr[i])
                count++;
        return count < lhs.dim*lhs.dim;
    }
    return true;
}
