#pragma once

//Conv Include
#include "ConvJsonWriter.h"


class ConvMaterialJsonWriter : public ConvJsonWriter
{
public:
    ConvMaterialJsonWriter(const FbxString& filePath, const ConvMaterial* importedMaterial);

protected:
    static void CreateMaterialPropertyNode(const ConvMaterialProperty* materialProperty, TDocument& doc, TValue& root);
};