 #include <iostream>
#include <random>
#include <bitset>

void Generate(uint8_t *mask_array, uint8_t array_length, uint8_t number_of_bits)
{
    std::random_device rd;
    std::mt19937 generate(rd());
    std::uniform_int_distribution<uint8_t> distr(0u, number_of_bits - 1);

    for(int i{0}; i < array_length; ++i)
    {
        mask_array[i] = distr(generate);
    }
}

template <typename Type>
void bit_inversion(Type *data, uint8_t number_of_bytes)
{
    if(number_of_bytes == 0 || data == nullptr)
    {
        std::cout << "Errors\n";
        return;
    }

    const uint8_t number_of_bits {number_of_bytes << 3};
    const uint8_t length {number_of_bits >> 1}; // изменяю половину рандомных битов
    uint8_t array_of_shifts[length];

    Generate(array_of_shifts, length, number_of_bits);

    for(int iteration{0}; iteration < length; ++iteration)
    {
        // std::cout << "The original bit representation - " << std::bitset<64>(*data) << '\n';

        uint64_t mask {1ul << array_of_shifts[iteration]};

        // std::cout << "Bit representation of the mask -  " << std::bitset<64>(mask) << '\n';

        // Так как не знаю какой установлен бит, проверяю какой установлен и вношу изменения

        if((*data | mask) == *data)
        {
            *data &= ~(mask);
        }
        else
        {
            *data |= mask;
        }
        // std::cout << "Result -                          " << std::bitset<64>(*data) << '\n';
        // std::cout << "\n";
    }
}


int main()
{
    uint64_t num1 {17446744073709551615};
    uint8_t length {8};

    std::cout << std::bitset<64>(num1) <<'\n';
    bit_inversion(&num1, length);
    std::cout << std::bitset<64>(num1) << '\n';

    uint16_t num2 {64535};
    length = 2;

    std::cout << std::bitset<16>(num2) <<'\n';
    bit_inversion(&num2, length);
    std::cout << std::bitset<16>(num2) << '\n';

    uint8_t num3 {250};
    length = 1;

    std::cout << std::bitset<8>(num3) <<'\n';
    bit_inversion(&num3, length);
    std::cout << std::bitset<8>(num3) << '\n';


    return 0;
}
