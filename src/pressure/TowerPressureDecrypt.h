#ifndef TOWERPRESSUREDECRPT_H
#define TOWERPRESSUREDECRPT_H

#include <future>
#include <mutex>
#include <regex>
#include <queue>
#include "../helper/Accumulator.h"

inline Accumulator additionRate;
inline std::queue<std::pair<std::string, std::string>> toDecrypt;
inline std::queue<std::string> decryptedPins;
inline int activeCores = 1;
inline int currentOperations = 0;
inline std::mutex job_mutex;
inline std::mutex queue_mutex;

template<typename R>
bool is_ready(std::future<R> const& f) {
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

inline void setActiveCoresTo(const int active) {
    activeCores = active;
}

inline void addToAccumRate(const int pressureAdded) {
    additionRate.accumulate(pressureAdded);
}

inline void addToDecrypt(const std::string& toHash, const std::string& pattern) {
    queue_mutex.lock();
    toDecrypt.emplace(toHash, pattern);
    queue_mutex.unlock();
}

inline std::string consumeDecrypted() {
    job_mutex.lock();
    if (decryptedPins.empty()) {
        return "";
    }
    std::string entry = decryptedPins.front();
    decryptedPins.pop();
    job_mutex.unlock();
    return entry;
}

inline bool anyCompletedJobs() {
    job_mutex.lock();
    const bool anyCompleted = !decryptedPins.empty();
    job_mutex.unlock();
    return anyCompleted;
}

inline std::pair<std::string, std::string> decryptNext() {
    queue_mutex.lock();
    if (toDecrypt.empty() || currentOperations >= activeCores) {
        queue_mutex.unlock();
        return { "", "" };
    }

    const auto [decrStr, pattern] = toDecrypt.front();
    toDecrypt.pop();
    queue_mutex.unlock();

    return { decrStr, pattern };
}

#endif //TOWERPRESSUREDECRPT_H
