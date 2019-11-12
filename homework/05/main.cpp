#include <iostream>
#include <sstream>
#include <cassert>
#include "serializer.h"
#include <string>

struct DataUint
{
    uint64_t a;
    uint64_t b;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b);
    }        
};

void test_serialize_uint() {
    DataUint x { 1, 2 };
    
    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    
    assert(std::string(stream.str()) == "1 2 ");
}

void test_deserialize_uint() {
    DataUint x { 1, 2 };
    
    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    
    Deserializer deserializer(stream);
    
    DataUint y;
    deserializer.load(y);
    
    assert(x.a == y.a);
    assert(x.b == y.b);
}

struct DataBool
{
    bool a;
    bool b;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b);
    }        
};

void test_serialize_bool() {
    DataBool x { false, true };
    
    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    
    assert(std::string(stream.str()) == "false true ");
}

void test_deserialize_bool() {
    DataBool x { false, true };
    
    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    
    Deserializer deserializer(stream);
    
    DataBool y;
    deserializer.load(y);
    
    assert(x.a == y.a);
    assert(x.b == y.b);
}

struct Data
{
    bool a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }        
};

void test_serialize() {
    Data x { false, true, 239 };
    
    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    
    assert(std::string(stream.str()) == "false true 239 ");
}

void test_deserialize() {
    Data x { false, true, 239 };
    
    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    
    Deserializer deserializer(stream);
    
    Data y;
    deserializer.load(y);
    
    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);
}

int main() {
    test_serialize_uint();
    test_deserialize_uint();
    test_serialize_bool();
    test_deserialize_bool();
    test_serialize();
    test_deserialize();
    return 0;
}

