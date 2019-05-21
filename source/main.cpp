//SVk Include
#include "General/Entity/SWorld.h"


int main()
{
    SWorld world;

    while (true)
    {
        if (!world.Loop()) break;
    }

    return 0;
}