#include <iostream>
#include "Pattern.h"

using namespace std;

int main()
{
    Pattern pattern = Pattern(256);

    pattern.leastFrequent();
    pattern.mostFrequent();

    return 0;
}