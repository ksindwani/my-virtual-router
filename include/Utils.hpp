#pragma once
#include <string>
#include <arpa/inet.h>
#include <stdexcept>

struct IPPrefix {
    uint32_t addr;
    int prefix_len;

    static IPPrefix fromString(const std::string& cidr) {
        size_t pos = cidr.find('/');
        std::string ip = (pos == std::string::npos) ? cidr : cidr.substr(0, pos);
        int len = (pos == std::string::npos) ? 32 : std::stoi(cidr.substr(pos + 1));
        
        struct in_addr sa;
        if (inet_pton(AF_INET, ip.c_str(), &sa) != 1) throw std::runtime_error("Invalid IP");
        return { ntohl(sa.s_addr), len };
    }

    static uint32_t getMask(int len) {
        return (len == 0) ? 0 : (~0U << (32 - len));
    }
};