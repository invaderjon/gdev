// test_data_factory.h
#ifndef INCLUDED_TEST_DATA_FACTORY
#define INCLUDED_TEST_DATA_FACTORY

#include <engine/assets/idata_factory.h>
#include <engine/containers/map.h>

namespace StevensDev
{

namespace sgdt
{

class TestDataFactory : public sgda::IDataFactory<std::string>
{
  private:
    // MEMBERS
    sgdc::Map<std::string> d_map;
      // Maps test paths to test values.

    sgdc::Map<bool> d_loadedMap;
      // Maps test values to whether or not they are "loaded".

  public:
    // CONSTRUCTORS
    TestDataFactory();
      // Constructs a new test data factory.

    TestDataFactory( const TestDataFactory& factory );
      // Constructs a copy of the given factory.

    ~TestDataFactory();
      // Destructs the factory.

    // OPERATORS
    TestDataFactory& operator=( const TestDataFactory& factory );
      // Makes this a copy of the other factory.

    // MEMBER FUNCTIONS
    void addTest( const std::string& path, const std::string& value );
      // Add a new test path with the given test value.

    virtual std::string* get( const std::string& path );
      // Gets the test data associated with the path.

    virtual void release( std::string* data );
      // Releases the given test data.

    bool isLoaded( const std::string& data ) const;
      // Checks if the given data is "loaded".
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream,
                          const TestDataFactory& factory )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
TestDataFactory::TestDataFactory() : d_map(), d_loadedMap()
{

}

inline
TestDataFactory::TestDataFactory( const TestDataFactory& factory )
    : d_map( factory.d_map ), d_loadedMap( factory.d_loadedMap )
{

}

inline
TestDataFactory::~TestDataFactory()
{

}

// OPERATORS
inline
TestDataFactory& TestDataFactory::operator=( const TestDataFactory& factory )
{
    d_map = factory.d_map;
    d_loadedMap = factory.d_loadedMap;

    return *this;
}

// MEMBER FUNCTIONS
inline
void TestDataFactory::addTest( const std::string& path,
                               const std::string& value )
{
    d_map[path] = value;
    d_loadedMap[value] = false;
}

inline
std::string* TestDataFactory::get( const std::string& path )
{
    std::string* value = &d_map[path];
    d_loadedMap[*value] = true;
    return value;
}

inline
void TestDataFactory::release( std::string* data )
{
    d_loadedMap[*data] = false;
}

inline
bool TestDataFactory::isLoaded( const std::string& data ) const
{
    return d_loadedMap[data];
}

} // End nspc sgdt

} // End nspc StevensDev


#endif
