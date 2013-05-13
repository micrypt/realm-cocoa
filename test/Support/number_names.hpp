#ifndef __NUMBER_NAMES__
#define __NUMBER_NAMES__

#include <string>

std::string number_name(std::size_t n)
{
    static const char* ones[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
                                 "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen",
                                 "eighteen", "nineteen"};
    static const char* tens[] = {"", "ten", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};

    std::string txt;
    if (n >= 1000) {
        txt = number_name(n/1000) + " thousand ";
        n %= 1000;
    }
    if (n >= 100) {
        txt += ones[n/100];
        txt += " hundred ";
        n %= 100;
    }
    if (n >= 20) {
        txt += tens[n/10];
        n %= 10;
    }
    else {
        txt += " ";
        txt += ones[n];
    }

    return txt;
}

#endif //__NUMBER_NAMES__