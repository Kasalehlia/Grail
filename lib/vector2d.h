
#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <stdint.h>
#include <SDL/SDL.h>


/**
 * A 2-element vector of type T.
 * The N is so you can make real typedefs (instead of the alias function the
 * "typedef" keyword provides)
 *
 * Example:
 * Vector2d<int32_t, 0> is a different type than Vector2d<int32_t, 1> 
 * although they are both 2d-vectors of int32_t.
 * This can be useful for dispatching.
 */
template <typename T, int N=0>
class Vector2d {

  public:
    typedef T X;
    typedef T Y;

  private:
    X _x; Y _y;

  public:
    Vector2d<T, N>() : _x(0), _y(0) { }
    Vector2d<T, N>(T x, T y) : _x(x), _y(y) { }

    X x() { return _x; }
    Y y() { return _y; }

    Vector2d<T, N> operator+(Vector2d<T, N> other);
    
    /**
     * Partition the unit circle into $directions equal pie slices,
     * return the number of the slice this vector (when drawn from the circles
     * center) lies in.
     */
    uint8_t nearestDirection(uint8_t directions);
};

typedef Vector2d<int32_t, 0> VirtualPosition;
typedef VirtualPosition VirtualSize;

typedef Vector2d<int32_t, 1> PhysicalPosition;
typedef PhysicalPosition PhysicalSize;

/*
 * conv
 */

template <typename A, typename B> B conv(A a);

template <> SDL_Rect conv<VirtualPosition, SDL_Rect>(VirtualPosition p);
template <> VirtualPosition conv<PhysicalPosition, VirtualPosition>(PhysicalPosition p);
template <> PhysicalPosition conv<VirtualPosition, PhysicalPosition>(VirtualPosition p);

#endif // VECTOR2D_H

