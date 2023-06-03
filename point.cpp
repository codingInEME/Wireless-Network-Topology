
#include <cmath>

class point
{
    // attributes
private:
    double x, y;

public:
    // default constructor
    point()
    {
        x = 0;
        y = 0;
    }

    // parameterized constructor
    point(double x, double y)
    {
        this->x = x;
        this->y = y;
    }

    // setters
    void setX(double x)
    {
        this->x = x;
    }

    void setY(double y)
    {
        this->y = y;
    }

    // getters
    double getX() const
    {
        return x;
    }

    double getY() const
    {
        return y;
    }

    bool equals(point a)
    {
        return x == a.x && y == a.y;
    }

    // member functions
    double distance(point p)
    {
        // distance between two points p1(x1, y1) and p2(x2, y2) --> sqrt[(x2 - x1)^2 + (y2 - y1)^2]
        return sqrt(((p.x - x) * (p.x - x)) + ((p.y - y) * (p.y - y)));
    }

    bool isZero()
    { // checks if point is on origin
        // if both x and y are zero then point is zero
        return x == 0 && y == 0;
    }

    point middlePoint(point p)
    {
        return point((x + p.x) / 2, (y + p.y) / 2);
    }

    bool isEqualTo(point p) const
    { // checks equality for both x and y of two points
        return x == p.x && y == p.y;
    }

    bool isGreaterThan(point p)
    { // compares two points based upon their distance from origin
        // distance of a point from origin (distFromOrigin) --> sqrt(x^2 + y^2)
        // if p1(x1, y1) p2(x2, y2) are two points then distFromOrigin is sqrt(x1^2 + y1^2) sqrt(x2^2 + y2^2)
        // if x1^2 + y1^2 > x2^2 + y2^2 then it means distFromOrigin(p1) > distFromOrigin(p2)
        // which means sqrt is not necessary to implement
        return ((x * x) + (y * y)) > ((p.x * p.x) + (p.y * p.y));
    }

    bool operator==(const point &p) const
    {
        return isEqualTo(p);
    }

    bool operator!=(const point &p) const
    {
        return !isEqualTo(p);
    }
};
