#ifndef GRID_H
#define GRID_H

struct grid
{
        grid(int dimension = 3);
        grid(const grid &other);
        grid& operator =(const grid& other);
        ~grid();

        int& operator [](int i);
        void setDim(int dimension);

        friend bool operator ==(const grid& lhs, const grid& rhs);
        friend bool operator !=(const grid& lhs, const grid& rhs);

        int *arr, dim;
        grid* parent;
        grid* child[4];
};


#endif // GRID_H
