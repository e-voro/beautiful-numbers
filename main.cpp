#include <iostream>
#include <vector>
#include <cassert>
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------

constexpr uint64_t binpow(uint32_t a, uint32_t n) noexcept
{
  int res = 1;
  while(n)
  {
    if (n & 1)
      res *= a;

    a *= a;
    n >>= 1;
  }
  return res;
}
//-----------------------------------------------------------------------------

constexpr int digits_sum(uint32_t a, uint32_t b) noexcept
{
  int res = 0;
  while(a > 0)
  {
    res += a % b;
    a /= b;
  }
  return res;
}
//-----------------------------------------------------------------------------

uint64_t beauty_quantity(uint32_t num_len, uint32_t beauty_len, uint32_t base) noexcept(false)
{
  if(num_len < 2 * beauty_len)
    throw std::runtime_error( "Wrong combination of num_len and beauty_len");

  vector<uint32_t> digits_sum_counters(base*beauty_len);

  for(int i = 0; i < binpow(base, beauty_len); i++)
    digits_sum_counters[ digits_sum(i, base) ]++;

  uint64_t quantity = 0;
  for(auto counter: digits_sum_counters)
    quantity += (uint64_t)counter * (uint64_t)counter * binpow(base, num_len - 2 * beauty_len);

  return quantity;
}
//-----------------------------------------------------------------------------

int main()
{
  //Some tests
  assert(beauty_quantity(2, 1, 2) == 2);
  assert(beauty_quantity(4, 2, 2) == 6);
  assert(beauty_quantity(5, 2, 2) == 12);
  assert(beauty_quantity(6, 2, 2) == 24);
  assert(beauty_quantity(7, 2, 2) == 48);
  assert(beauty_quantity(6, 3, 2) == 20); //000 -> 1; 001, 010, 100 -> 9; 110, 101, 011 -> 9; 111 -> 1. 1 + 9 + 9 + 1 = 20

  assert(beauty_quantity(2, 1, 3) == 3);
  assert(beauty_quantity(3, 1, 3) == 9);
  assert(beauty_quantity(4, 1, 3) == 27);

  //Main task processing
  constexpr uint32_t base = 13;
  constexpr uint32_t num_len = 13;
  constexpr uint32_t beauty_len = 6;

  // Checking that binpow is invoked at a compile time. Static_assert
  // does not compile if binpow is not calculated on a compiling time)
  static_assert(binpow(13, 5) != 0, "Binpow invocation at run-time!");
  static_assert(binpow(base, beauty_len) != 0, "Binpow invocation at run-time!");

  try
  {
    cout << "Data: \n"
            "      base = " << base << endl <<
            "      num_len = "<< num_len << endl <<
            "      beauty_len = " << beauty_len << endl <<
            "Result: \n"
            "      beautiful numbers quantity = " << beauty_quantity(num_len, beauty_len, base) << endl;
    return 0;
  }
  catch(std::runtime_error& e)
  {
    cout << endl << e.what() << endl;
  }
  return -1;
}
