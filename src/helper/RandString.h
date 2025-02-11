#ifndef RANDSTRING_H
#define RANDSTRING_H
#include <cstdint>
#include <string>
#include <bits/random.h>

inline std::string randomString() {
    static constexpr char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(10);

    for (int i = 0; i < 10; ++i) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(1, INT32_MAX);
        const unsigned long roll = dist(rng);
        tmp_s += alphanum[roll % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

#endif //RANDSTRING_H
