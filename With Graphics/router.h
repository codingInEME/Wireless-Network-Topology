#ifndef ROUTER_H
#define ROUTER_H
#include<iostream>
#include"point.h"

struct router
{
    std::string name;
    point location;
    double range = 1;

    friend std::ostream &operator<<(std::ostream &os, router &r)
    {
        std::cout << r.name;
        return os;
    }

    bool operator==(const router &r) const
    {
        return name == r.name;
    }

    bool operator!=(const router &r) const
    {
        return !(name == r.name);
    }
};

#endif // ROUTER_H
