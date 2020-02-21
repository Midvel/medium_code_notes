#include <string>
#include <vector>
#include <utility>
#include <iostream>


class SimpleStorage
{
public:
    using SimpleElement_t = std::pair<std::string, std::string>;
    using SimpleStorage_t = std::vector<SimpleElement_t>;

    SimpleStorage() = default;

    // Interface
    void addProperty( std::string propertyName )
    {
        createPair( propertyName );
    }

    void addNumber( std::string propertyName, double number )
    {
        createPair( propertyName, number );
    }

    void addString( std::string propertyName, std::string content )
    {
        createPair( propertyName, content );
    }

    void addPair( std::string propertyName, int contentNumber, std::string subContent )
    {
        createPair( propertyName, contentNumber, subContent );
    }

    friend std::ostream& operator<<( std::ostream& out, const SimpleStorage& obj )
    {
        for ( auto element : obj.m_storage )
        {
            out << element.first << " " << element.second << "\n";
        }
        return out;
    }

private:
    // Intermediate collector
    template<typename... Args>
    void createPair( std::string propertyName, Args&&... args )
    {
        // Place for the preprocessing logic
        SimpleElement_t newPair( propertyName, "" );

        // Call redirection
        storeElement( std::move( newPair ), std::forward<Args>( args )... );
    }

    // Internal implementations
    void storeElement( SimpleElement_t newPair )
    {
        newPair.second = "Empty";
        m_storage.emplace_back( newPair );
    }

    void storeElement( SimpleElement_t newPair, double number )
    {
        newPair.second = std::to_string( number );
        m_storage.emplace_back( newPair );
    }

    void storeElement( SimpleElement_t newPair, std::string content )
    {
        newPair.second = content;
        m_storage.emplace_back( newPair );
    }

    void storeElement( SimpleElement_t newPair, int contentNumber, std::string subContent )
    {
        newPair.second = std::to_string( contentNumber ) + ": " + subContent;
        m_storage.emplace_back( newPair );
    }

private:
    SimpleStorage_t m_storage;
};


int main()
{
    SimpleStorage storage;

    storage.addProperty( "someEmptyProperty" );
    storage.addNumber( "numericalProperty", 5.5 );
    storage.addString( "textProperty", "Some string" );
    storage.addPair( "complexProperty", 3, "Complex string" );

    std::cout << storage;

    return 0;
}
