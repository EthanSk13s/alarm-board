#include "ambiance.h"

static void default_loop_setup(AmbiancePtr amb)
{

}

void amb_default_init(AmbiancePtr amb)
{
    amb->setup = default_loop_setup;
}