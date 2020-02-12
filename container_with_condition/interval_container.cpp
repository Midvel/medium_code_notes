#include <iostream>
#include <vector>
#include <algorithm>

class IntervalContainer
{
public:
    // class to wrap the write access to the elements
    // container content can be changed only if condition is passed
    class ConditionProxy
    {
    public:
        ConditionProxy() = delete;
        ConditionProxy( IntervalContainer& owner, double& obj) : m_owner( owner ), m_obj( obj ) {};
        
        //This is the main logic, when returned proxy instance get the value to be saved
        ConditionProxy& operator=( double val )
        {
            m_obj = m_owner.convertValue( val );
            return *this;
        }

        // Arithmetical helpers
        ConditionProxy& operator*=( double val )
        {
            m_obj = m_owner.convertValue( m_obj*val );
            return *this;
        }
        
        double operator*( double val ) { return m_obj * val;}

        operator double() { return m_obj; }
    private:
        // The references to the main container and wrapped element
        IntervalContainer& m_owner;
        double& m_obj;
    };
    
    class Iterator
    {
    public:
        Iterator() = delete;
        
        Iterator( IntervalContainer& owner, int index ) : m_owner( owner ), m_index( index ) {}
        
        // Wrapped method
        ConditionProxy operator*() { return m_owner[m_index]; }
        
        bool operator==(const Iterator& right)
        {
            return &right.m_owner == &this->m_owner && right.m_index == this->m_index;
        }
        
        bool operator!=(const Iterator& right)
        {
            return !(*this == right);
        }
        
        Iterator operator++()
        {
            Iterator i = *this;
            m_index == m_index++;
            return i;
        }
        Iterator operator++(int junk) { m_index++; return *this; }
        
    private:
       IntervalContainer& m_owner;
       int m_index;
    };

    IntervalContainer() = delete;

    IntervalContainer(double left, double right) : m_left( left ), m_right( right ) {}
    
    void append( double val )
    {
        m_vault.emplace_back( convertValue( val ) );
    }
    
    bool checkValue( double val ) {return val >= m_left && val <= m_right; }
    
    double convertValue( double val )
    {
        if ( checkValue( val ) )
            return val;
        return ( val < m_left ? 0 : 1 );
    }
    
    Iterator begin() { return Iterator( *this, 0 ); }
    
    Iterator end() { return Iterator( *this, m_vault.size() ); }
    
    // Read-only indexator
    double const& operator[]( int index ) const
    {
        return m_vault[index];
    }
    // Wrapped indexator with conditional access
    ConditionProxy operator[]( int index )
    {
        return ConditionProxy( *this, m_vault[index] );
    }
    
private:
    std::vector<double> m_vault;
    double m_left;
    double m_right;
};


int main()
{
    // Keeps values from the interval [0, 1]
    IntervalContainer intervalCont(0, 1);
    
    intervalCont.append( 0 );
    intervalCont.append( 0.1 );
    intervalCont.append( 0.2 );
    intervalCont.append( 0.5 );
    
    // Compiles but rejects the value
    intervalCont[2] = 5;
    intervalCont[3] = 0.3;
    
    for (auto elem : intervalCont)
        std::cout << elem << " ";
    std::cout << "\n";

    // All values out of the allowed interval will be rejected
    for (auto iter = intervalCont.begin(); iter != intervalCont.end(); iter++)
        *iter *= 3.0;

    for (auto elem : intervalCont)
        std::cout << elem << " ";
    std::cout << "\n";
    
    std::transform( intervalCont.begin(), intervalCont.end(), intervalCont.begin(),
                    [](auto elem){ return elem * 3.0;} );

    for (auto iter = intervalCont.begin(); iter != intervalCont.end(); iter++)
        std::cout << *iter << " ";

    return 0;
}
