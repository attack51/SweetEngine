#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Math/STransform.h"

//C++ Include
#include <vector>


class SHierarchyTransform : STransform
{
public:
    SHierarchyTransform();
    SHierarchyTransform(SHierarchyTransform* parent);
    SHierarchyTransform(const SHierarchyTransform& original);

    ~SHierarchyTransform();

private:
    SHierarchyTransform* parent;
    vector<SHierarchyTransform*> child;
};
