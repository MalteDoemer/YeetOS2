
extern "C" {

unsigned long long __udivdi3(unsigned long long a, unsigned long long b)
{
    // TODO: implement efficient algorithm for divison

    unsigned long long result = 0;

    while (a > b) {
        a -= b;
        result++;
    }

    return result;
}

unsigned long long __umoddi3(unsigned long long a, unsigned long long b)
{
    // TODO: implement efficient algorithm for modulo

    while (a > b) {
        a -= b;
    }

    return a;
}

}