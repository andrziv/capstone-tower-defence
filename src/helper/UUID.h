//
// Created by aki on 12/29/24.
//

#ifndef UUID_H
#define UUID_H
#include <random>
#include <string>

inline std::string get_uuid() {
    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::uniform_int_distribution dist(0, 15);

    constexpr bool dash[] = { false, false, false, false, true, false, true, false, true, false, true, false, false, false, false, false };

    std::string res;
    for (int i = 0; i < 16; i++) {
        const auto v = "0123456789abcdef";
        if (dash[i]) res += "-";
        res += v[dist(rng)];
        res += v[dist(rng)];
    }
    return res;
}

#endif //UUID_H
