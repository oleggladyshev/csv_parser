#pragma once

#include <chrono>
#include <cstdio>
#include <string>

void print_timediff(const std::chrono::steady_clock::duration& d, 
    const std::string& annotation) {
    printf(annotation.c_str());
    printf("%ld\n", std::chrono::duration_cast<
        std::chrono::microseconds>(d).count());
}

template <class Cont>
void print_container(const Cont& cont) {
    for (const auto& r : cont){
        printf("%ld, %s\n", r.score, r.descr);
    }
}