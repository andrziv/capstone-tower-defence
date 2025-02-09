#ifndef TOWERPRESSUREDECRPT_H
#define TOWERPRESSUREDECRPT_H
#include <iostream>
#include <iomanip>
#include <ios>
#include <mutex>
#include <queue>
#include <regex>
#include <string>
#include <openssl/sha.h>
#include <random>
#include <unistd.h>

inline std::queue<std::pair<std::string, std::string>> toDecrypt;
inline int activeCores = 1;
inline int currentOperations = 0;
inline std::mutex job_mutex;
inline std::mutex queue_mutex;

inline void setActiveCoresTo(const int active) {
    activeCores = active;
}

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
        const int roll = dist(rng);
        tmp_s += alphanum[roll % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

inline void addToDecrypt(const std::string& toHash, const std::string& pattern) {
    queue_mutex.lock();
    toDecrypt.emplace(toHash, pattern);
    queue_mutex.unlock();
}

inline std::string decrypt(const std::string& decrStr, const std::string& pattern) {
    job_mutex.lock();
    currentOperations++;
    job_mutex.unlock();

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    std::stringstream ss;
    std::string rStr;

    while (!std::regex_search(ss.str().c_str(), std::regex(pattern))) {
        ss.str(std::string());
        rStr = randomString();
        std::string toHash = rStr + decrStr;

        SHA256_Update(&sha256, toHash.c_str(), toHash.size());
        SHA256_Final(hash, &sha256);

        for(const unsigned char i : hash) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
        }
    }

    job_mutex.lock();
    printf("Hashed: %s\n", ss.str().c_str());
    std::cout << std::flush;
    currentOperations--;
    job_mutex.unlock();
    return rStr;
}

inline void decryptNext() {
    queue_mutex.lock();
    if (toDecrypt.empty() || currentOperations >= activeCores) {
        queue_mutex.unlock();
        return;
    }

    const auto [decrStr, pattern] = toDecrypt.front();
    toDecrypt.pop();
    queue_mutex.unlock();

    decrypt(decrStr, pattern);
}

#endif //TOWERPRESSUREDECRPT_H
