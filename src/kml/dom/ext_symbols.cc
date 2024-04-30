
#include "kml/dom/ext_symbols.h"

#include "kml/base/xml_namespaces.h"
#include <geometry.h>
#include <iostream>
#include <kml/base/vec3.h>
#include <kml_cast.h>

using kmlbase::Attributes;

namespace kmldom {
ExtCoordinates::ExtCoordinates()
    : Element(Type_ExtCoordinates)
{
    set_xmlns(kmlbase::XMLNS_EXT);
}
// The char_data is everything between <coordinates> elements including
// leading and trailing whitespace.
void ExtCoordinates::Parse(const string &char_data) {
    const char* cstr = char_data.c_str();
    const char* endp = cstr + char_data.size();
    char* next = const_cast<char*>(cstr);
    while (next != endp) {
        kmlbase::Vec3 vec;
        if (Coordinates::ParseVec3(next, &next, &vec)) {
            coordinates_ = vec;
            return;
        }
    }
}

// Coordinates essentially parses itself.
void ExtCoordinates::AddElement(const ElementPtr& element) {
    Parse(get_char_data());
}


ExtSymbolInfo::ExtSymbolInfo() : Feature()
{
    set_xmlns(kmlbase::XMLNS_EXT);
}

void ExtSymbolInfo::AddElement(const ElementPtr &element)
{
    if (!element) {
        return;
    }
    switch (element->Type()) {
    case Type_ExtStandard:
        has_standard_ = element->SetString(&standard_);
        return;
    case Type_ExtCode:
        has_code_ = element->SetString(&code_);
        return;
    case Type_ExtCoordinates: {
        const ExtCoordinatesPtr &coordinates = AsExtCoordinates(element);
        coordinates->AddElement(coordinates);
        set_coordinates(coordinates);
        return;
    }
    default:
        std::cout << ">>>>>>>Unknown element " << kmldom::kmlDomTypeToString(element->Type()) << std::endl;
        Element::AddElement(element);
    }
}
void ExtSymbolInfo::Accept(Visitor *visitor)
{
    visitor->VisitExtSymbolInfo(ExtSymbolInfoPtr(this));
}

ExtStandard::ExtStandard()
    : Element(Type_ExtStandard)
{
    set_xmlns(kmlbase::XMLNS_EXT);
}

bool ExtStandard::SetString(string *val)
{
    bool ret = false;
    if (val) {
        *val = get_char_data();
        ret = true;
    }
    return ret;
}

string ExtStandard::getStandard() const
{
    return get_char_data();
}

ExtCode::ExtCode() : Element(Type_ExtCode)
{
    set_xmlns(kmlbase::XMLNS_EXT);
}

bool ExtCode::SetString(string *val)
{
    bool ret = false;
    if (val) {
        *val = get_char_data();
        ret = true;
    }
    return ret;
}


string ExtCode::getCode()
{
    return get_char_data();
}

} // namespace kmldom
