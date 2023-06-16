#include <include/Bigint.h>

Bigint::Bigint(const std::string& str)
{
    if (!str.size()) {
        /* Если на вход подали пустую строку */
        m_error = Error::EmptyString;
        return;
    }

    m_negative = str[0] == '-';

    if (m_negative && str.size() == 1) {
        /* Если на вход подали только минус */
        m_error = Error::NoDigitsProvided;
        return;
    }

    /* Валидация входной строки на наличие посторонних символов */
    if (!std::all_of(str.begin() + m_negative, str.end(), isdigit)) {
        /* Если встретилась не цифра (не учитывая первый минус в начале) */
        m_error = Error::UnexpectedCharacter;
        return;
    }

    m_data.resize(str.size());
    for (int i = 0; i < str.size() - m_negative; i++)
        m_data[i] = str[str.size() - 1 - i] - '0';

    trimLeadingZeros();
}

std::string Bigint::toString() const
{
    if (isError())
        return "NaN";

    if (!size())
        return "0";

    std::string result;
    result.resize(size() + m_negative);

    if (m_negative)
        result[0] = '-';

    for (int i = 0; i < size(); i++)
        result[i + m_negative] = getFirstDigit(i) + '0';

    return result;
}

bool Bigint::operator<(const Bigint& other) const
{
    if (isError() || other.isError())
        return false;

    if (size() != other.size())
        return size() < other.size();

    for (int i = 0; i < size(); i++)
        if (getFirstDigit(i) < other.getFirstDigit(i))
            return true;

    return false;
}

Bigint Bigint::operator+(const Bigint& op2) const
{
    /* Проверка на наличие ошибок в операндах */
    if (isError() || op2.isError()) {
        return isError() ? Bigint(getError()) : Bigint(op2.getError());
    }

    if (!isNegative() && !op2.isNegative()) {
        /* Положительное + положительное */
        return basicAdd(op2);
    } else if (!isNegative() && op2.isNegative()) {
        /* Положительное + отрицательное */
        return basicSubstract(op2);
    } else if (isNegative() && !op2.isNegative()) {
        /* Отрицательное + положительное */
        return op2.basicSubstract(*this);
    } else {
        /* Отрицательное + отрицательное */
        return -basicAdd(op2);
    }
}

Bigint Bigint::operator-(const Bigint& op2) const
{
    /* Проверка на наличие ошибок в операндах */
    if (isError() || op2.isError()) {
        return isError() ? Bigint(getError()) : Bigint(op2.getError());
    }

    if (!m_negative && !op2.isNegative()) {
        /* Оба операнда положительные */
        return basicSubstract(op2);
    } else if (!m_negative && op2.isNegative()) {
        /* Вычитаем отрицательное из положительного */
        return basicAdd(op2);
    } else if (m_negative && op2.isNegative()) {
        /* Вычитаем отрицательное из отрицательного */
        return op2.basicSubstract(*this);
    } else {
        /* Вычитаем положительное из отрицательного */
        return -basicAdd(op2);
    }
}

void Bigint::trimLeadingZeros()
{
    /* Количество незначащих нулей */
    int leadingZeros = (std::find_if(m_data.rbegin(), m_data.rend(),
                           [](std::bitset<4> d) { return d.to_ulong() != 0; }))
        - m_data.rbegin();

    m_data.resize(size() - leadingZeros);
}

Bigint Bigint::basicSubstract(const Bigint& op2) const
{

    if (*this < op2) {
        /* Если вычитаем большее из меньшего */
        return -(op2.basicSubstract(*this));
    }

    Bigint result;
    result.m_data.reserve(std::max(op2.size(), size()) + 1);

    int carry = 0;
    for (int i = 0; i < std::max(op2.size(), size()); i++) {
        int currentDigitsSum = getLastDigit(i) - op2.getLastDigit(i) - carry;

        /* Перенос из старших разрядов */
        carry = currentDigitsSum < 0;

        if (currentDigitsSum < 0)
            result.m_data.push_back(10 - abs(currentDigitsSum) % 10);
        else
            result.m_data.push_back(currentDigitsSum % 10);
    }

    /* Добавляем новую цифру, если самые старшие разряды дали перенос */
    if (carry) {
        result.m_data.push_back(carry);
    }

    result.trimLeadingZeros();

    return result;
}

Bigint Bigint::basicAdd(const Bigint& op2) const
{
    Bigint result;
    result.m_data.reserve(std::max(op2.size(), size()) + 1);

    int carry = 0;
    for (int i = 0; i < std::max(op2.size(), size()); i++) {
        unsigned int currentDigitsSum = getLastDigit(i) + op2.getLastDigit(i) + carry;

        /* Перенос в старшие разряды */
        carry = currentDigitsSum > 9;

        result.m_data.push_back(currentDigitsSum % 10);
    }

    /* Добавляем новую цифру, если самые старшие разряды дали перенос */
    if (carry) {
        result.m_data.push_back(carry);
    }
    return result;
}

std::string Bigint::getErrorString() const
{
    switch (m_error) {
    case NoError:
        return "No error";

    case UnexpectedCharacter:
        return "Unexpected character";

    case NoDigitsProvided:
        return "No digits provided";

    case EmptyString:
        return "Empty input string";
    }

    return "Unknown error";
}
