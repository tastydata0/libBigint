#include <algorithm>
#include <bitset>
#include <string>
#include <vector>

class Bigint {

public:
    Bigint() = default;

    Bigint(const std::string& str);

    Bigint(const long long num)
        : Bigint(std::to_string(num))
    {
    }

    enum Error { NoError,
        UnexpectedCharacter,
        NoDigitsProvided,
        EmptyString };

    Bigint(const Bigint::Error error)
        : m_error(error)
    {
    }

    inline int size() const
    {
        return m_data.size();
    }

    inline bool isNegative() const
    {
        return m_negative;
    }

    std::string toString() const;

    /// \brief getLastDigit - получение n-ной цифры с конца (с младших разрядов)
    inline unsigned int getLastDigit(int n) const
    {
        if (n < m_data.size())
            return m_data[n].to_ulong();

        return 0;
    }

    /// \brief getFirstDigit - получение n-ной цифры с начала (со старших разрядов)
    /// \return -1, если произошел выход за пределы, иначе нужную цифру
    inline int getFirstDigit(int n) const
    {
        if (n < m_data.size())
            return m_data[size() - n - 1].to_ulong();

        return -1;
    }

    bool operator<(const Bigint& other) const;

    /// В случае наличия ошибки хотя бы в одном из операндов, вернёт Bigint с этой же ошибкой. Если у обоих операндов ошибка, вернет Bigint с ошибкой первого операнда.
    Bigint operator+(const Bigint& op2) const;

    inline Bigint operator-()
    {
        m_negative = !m_negative;
        return *this;
    }

    inline Bigint operator-() const
    {
        return -Bigint(*this);
    }

    /// В случае наличия ошибки хотя бы в одном из операндов, вернёт Bigint с этой же ошибкой. Если у обоих операндов ошибка, вернет Bigint с ошибкой первого операнда.
    Bigint operator-(const Bigint& op2) const;

    inline Bigint::Error getError() const
    {
        return m_error;
    }

    inline bool isError() const
    {
        return m_error != Error::NoError;
    }

    std::string getErrorString() const;

private:
    void trimLeadingZeros();

    /// \brief basicSubstract - функция вычитания числа. Поле m_negative не влияет на работу функции
    /// \param op2 - вычитаемое число
    /// \return результат вычитания
    Bigint basicSubstract(const Bigint& op2) const;

    /// \brief Поле m_negative не влияет на работу функции
    /// \param op2
    /// \return
    Bigint basicAdd(const Bigint& op2) const;

    /// \brief m_data - вектор цифр числа. m_data[0] - самый младший разряд. Не хранит незначащие нули. Если в Bigint хранится 0, этот вектор пуст.
    std::vector<std::bitset<4>> m_data;

    bool m_negative = false;

    Error m_error = Error::NoError;
};
