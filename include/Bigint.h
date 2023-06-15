#include <algorithm>
#include <bitset>
#include <string>
#include <vector>

class Bigint {

public:
    Bigint() = default;

    ///
    /// \brief Bigint - основной конструктор, создающий Bigint из строки. Допускается наличие одного символа "-" в начале. Все остальные символы - цифры
    /// \details В случае ошибки парсинга строки число будет иметь значение 0. Значение ошибки можно получить с помощью функции getError()
    /// \param str - строковое представление числа
    ///
    Bigint(const std::string& str);

    ///
    /// \brief Bigint - перегрузка основного конструктора для удобства использования. Преобразует входное число в строку и вызывает основной конструктор.
    /// \param num - число для инициализации Bigint
    ///
    Bigint(const long long num)
        : Bigint(std::to_string(num))
    {
    }

    ///
    /// \brief Error - перечисление возможных ошибок, возникающих при создании Bigint. Описание ошибок см. в документации к getError()
    ///
    enum Error { NoError,
        UnexpectedCharacter,
        NoDigitsProvided,
        EmptyString };

    ///
    /// \brief Bigint - конструктор, создающий число 0 с заданной ошибкой
    /// \param error - ошибка, которая сразу присвоится создаваемому числу
    ///
    Bigint(const Bigint::Error error)
        : m_error(error)
    {
    }

    ///
    /// \brief size - функция, возвращающая размер числа
    /// \return количество значащих цифр в числе. Возвращает 0, если isError() == true
    ///
    inline int size() const
    {
        return m_data.size();
    }

    ///
    /// \brief isNegative - функция проверки знака числа
    /// \return true, если число отрицательное, иначе false. Возвращает false, если isError() == true
    ///
    inline bool isNegative() const
    {
        return m_negative;
    }

    ///
    /// \brief toString - функция, возвращающая строковое представление числа
    /// \return число, преобразованное в строку. Если оно отрицателное, в начале будет символ "-". Возвращает "NaN", если isError() == true
    ///
    std::string toString() const;

    ///
    /// \brief getLastDigit - получение n-ной цифры с конца (с младших разрядов)
    /// \param n - индекс цифры с конца, которую нужно получить
    /// \return n-ная цифра с конца. В случае выхода за пределы будет возвращено значение 0 (т.е. незначащий 0 числа). Возвращает 0, если isError() == true
    ///
    inline unsigned int getLastDigit(int n) const
    {
        if (n < m_data.size())
            return m_data[n].to_ulong();

        return 0;
    }

    ///
    /// \brief getFirstDigit - получение n-ной цифры с начала (со старших разрядов)
    /// \param n - индекс цифры с начала, которую нужно получить
    /// \return -1, если произошел выход за пределы, иначе нужную цифру. Возвращает -1, если isError() == true
    ///
    inline int getFirstDigit(int n) const
    {
        if (n < m_data.size())
            return m_data[size() - n - 1].to_ulong();

        return -1;
    }

    ///
    /// \brief operator < - оператор сравнения чисел Bigint
    /// \param other - число для сравнения с текущим
    /// \return Результат операции <. Возвращает false, если isError() == true хотя бы у одного из операндов
    ///
    bool operator<(const Bigint& other) const;

    ///
    /// \brief operator+ - оператор сложения чисел. Оба операнда могут иметь любой знак.
    /// \param op2 - второй операнд
    /// \return Сумма чисел. В случае наличия ошибки хотя бы в одном из операндов, вернёт Bigint с этой же ошибкой. Если у обоих операндов ошибка, вернет Bigint с ошибкой первого операнда.
    ///
    Bigint operator+(const Bigint& op2) const;

    ///
    /// \brief operator- - унарный минус, меняющий знак в текущем неконстантном числе
    /// \return Этот же объект с противоположным знаком
    ///
    inline Bigint operator-()
    {
        m_negative = !m_negative;
        return *this;
    }

    ///
    /// \brief operator- - унарный минус, создающий копию этого числа с противоположным знаком
    /// \return Копия объекта с противоположным знаком
    ///
    inline Bigint operator-() const
    {
        return -Bigint(*this);
    }

    ///
    /// \brief operator- - оператор вычитания чисел. Оба операнда могут иметь любой знак.
    /// \param op2 - второй операнд
    /// \return Разность чисел. В случае наличия ошибки хотя бы в одном из операндов, вернёт Bigint с этой же ошибкой. Если у обоих операндов ошибка, вернет Bigint с ошибкой первого операнда.
    ///
    Bigint operator-(const Bigint& op2) const;

    ///
    /// \brief getError - функция получения ошибки из Bigint
    /// \details Список возможных ошибок:
    /// NoError - отсутствие ошибки;
    /// UnexpectedCharacter - наличие неподходящего символа во входной строке;
    /// NoDigitsProvided - наличие знака "-", но отсутствие цифр во входной строке;
    /// EmptyString - передана пустая строка.
    /// \return Ошибка в этом числе. Если ошибки нет, возвращает Error::NoError
    ///
    inline Bigint::Error getError() const
    {
        return m_error;
    }

    ///
    /// \brief isError - функция проверки на наличие ошибки в числе
    /// \return false, если текущая ошибка == Error::NoError. В других случаях возвращает true.
    ///
    inline bool isError() const
    {
        return m_error != Error::NoError;
    }

    ///
    /// \brief getErrorString - функция получения ошибки в строковом формате, например "Empty input string"
    /// \return Строковое описание ошибки в числе
    ///
    std::string getErrorString() const;

private:
    void trimLeadingZeros();

    /// \brief basicSubstract - функция вычитания чисел. Поле m_negative не влияет на работу функции, т.е. функция возвращает разность модулей операндов. Тем не менее, результат может быть отрицательным, если второй операнд больше первого.
    /// \param op2 - вычитаемое число
    /// \return результат вычитания
    Bigint basicSubstract(const Bigint& op2) const;

    /// \brief basicAdd - функция сложения чисел. Поле m_negative не влияет на работу функции, т.е. функция возвращает сумму модулей операндов.
    /// \param op2 - вычитаемое число
    /// \return результат вычитания
    Bigint basicAdd(const Bigint& op2) const;

    ///
    /// \brief m_data - вектор цифр числа. m_data[0] - самый младший разряд. Не хранит незначащие нули. Если в Bigint хранится 0, этот вектор пуст.
    /// \details Каждый std::bitset<4> (4 бита) принимает значения 0 - 15. Этого достсточно, чтобы хранить одну цифру.
    ///
    std::vector<std::bitset<4>> m_data;

    ///
    /// \brief m_negative - знак числа. Если это поле истинно, значит число отрицательно
    ///
    bool m_negative = false;

    ///
    /// \brief m_error - ошибка числа. Если ошибок нет, хранит Error::NoError.
    ///
    Error m_error = Error::NoError;
};
