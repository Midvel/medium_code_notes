#include <cmath>
#include <functional>

enum Operation
{
    SUM = 0,
    MULTI,
    DIV,
    SUBSTR,
    EXP
};

class Calculator
{
public:
    Calculator() : m_func( [](double a, double b) -> double { return a;} )
    {}

    Calculator& operator[](Operation op)
    {
        switch (op)
        {
            case SUM:
                m_func = [](double a, double b) -> double { return a + b;};
                break;
            case MULTI:
                m_func = [](double a, double b) -> double { return a * b;};
                break;
            case DIV:
                m_func = [](double a, double b) -> double { return a / b;};
                break;
            case SUBSTR:
                m_func = [](double a, double b) -> double { return a - b;};
                break;
            case EXP:
                m_func = pow;
                break;
            default:
                break;
        }
        return *this;
    }
    double calculate(double val1, double val2)
    {
        return m_func(val1, val2);
    }
private:
    std::function<double(double, double)> m_func;
};


class AccumulatorMap
{
public:
    AccumulatorMap() = delete;

    explicit AccumulatorMap(double initialValue) : m_value(initialValue)
    {}

    AccumulatorMap& operator()(Operation op, double value)
    {
       m_value = m_calc[op].calculate(m_value, value);
       return *this;
    }

    operator double()
    {
        return m_value;
    }

private:
    double m_value;
    Calculator m_calc;
};


double CircleArea(double radius)
{
    return AccumulatorMap(radius)
                       (EXP, 2)
                       (MULTI, 3.14);
}

double SphereVolume(double radius)
{
    return AccumulatorMap(radius)
                       (EXP, 3)
                       (MULTI, 4)
                       (DIV, 3)
                       (MULTI, 3.14);
}

int main()
{
    double value1 = CircleArea(4);
    double value2 = SphereVolume(7);

    return 0;
}
