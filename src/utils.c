#include "utils.h"

int compare_color(Color x, Color y)
{
    return x.r == y.r && x.b == y.b && x.g == y.g;
}