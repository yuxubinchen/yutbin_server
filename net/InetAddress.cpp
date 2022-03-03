#include "InetAddress.h"
#include <string.h>
using namespace yb;
using namespace yb::net;
InetAddress::InetAddress(const char *ip, uint16_t port)
{
    explicit_bzero(&addr, sizeof(addr));
    fromIpPort(ip, port, &addr);
}
std::string InetAddress::toIp() const
{
    char buf[64] = "";
    ::toIp(buf, sizeof(buf), getSockAddr());
    return buf;
}
std::string InetAddress::toIpPort() const
{
    char buf[64] = "";
    net::toPortString(buf, sizeof(buf), getSockAddr());
    return buf;
}
std::string InetAddress::toPortString() const
{
    char buf[64] = "";
    net::toIpPort(buf, sizeof(buf), getSockAddr());
    return buf;
}