#include <libnet.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

void usage()
{
    cout << "Usage: \n \
    \t-d dest ip and port \n \
    \t-s src ip and port \n \
    \t-p payload \nExample: ./tcp -d 192.168.232.111:25 -s 192.168.232.222:80 -p hahhhh \n";
}
int main(int argc, char **argv)
{
    int res;
    libnet_t *l;
    libnet_ptag_t tag;
    char *payload;
    u_short payload_s;
    u_long src_ip, dst_ip;
    u_short src_port, dst_port;
    // u_char src_mac[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    // u_char dst_mac[6] = {0x08, 0x00, 0x27, 0x33, 0x41, 0x8f};
    // u_char src_mac[6] = {0x00, 0x15, 0x5d, 0x4b, 0x67, 0x2a};//wsl mac
    // u_char dst_mac[6] = {0x00, 0x15, 0x5d, 0x4b, 0x67, 0x2a};
    u_char src_mac[6] = {0x00, 0x15, 0x5d, 0x4b, 0x67, 0x2a};
    u_char dst_mac[6] = {0x00, 0x15, 0x5d, 0x99, 0x99, 0xe9}; // windows mac
    char errbuf[LIBNET_ERRBUF_SIZE];
    src_ip = libnet_name2addr4(l, "192.168.232.111", LIBNET_DONT_RESOLVE);
    dst_ip = libnet_name2addr4(l, "192.168.232.222", LIBNET_DONT_RESOLVE);
    src_port = 9999;
    dst_port = 9999;
    payload = "hahhhhhhhhhhhhhhhh";
    payload_s = strlen(payload);

    const char *optstring = "d:s:p:";
    int opt;
    char *pos;
    if (argc < 3)
    {
        usage();
        exit(-1);
    }
    while ((opt = getopt(argc, argv, optstring)) != -1)
    {
        switch (opt)
        {
        case 'd': // dest ip and port
            pos = strrchr(optarg, ':');
            if (!pos)
            {
                usage();
                exit(-1);
            }
            *pos++ = 0;
            if ((dst_ip = libnet_name2addr4(l, optarg, LIBNET_DONT_RESOLVE)) == -1)
            {
                cout << "Invail dst ip:" << optarg << endl;
                exit(-1);
            }
            dst_port = (u_short)atoi(pos);

            break;
        case 's': // src ip and port
            pos = strrchr(optarg, ':');
            if (!pos)
            {
                usage();
                exit(-1);
            }
            *pos++ = 0;
            if ((src_ip = libnet_name2addr4(l, optarg, LIBNET_DONT_RESOLVE)) == -1)
            {
                cout << "Invail src ip:" << optarg << endl;
                exit(-1);
            }
            src_port = (u_short)atoi(pos);
            break;
        case 'p': // payload
            payload = optarg;
            payload_s = strlen(payload);
            break;
        default:
            usage();
            exit(-1);
            break;
        }
    }

    if (!src_ip || !dst_ip | !src_port || !dst_port)
    {
        perror("ip or port invaild");
        exit(-1);
    }
    if ((l = libnet_init(LIBNET_LINK, "eth0", errbuf)) == NULL)
    {
        cout << "libnet_init():" << libnet_geterror(l) << endl;
        exit(-1);
    }
    tag = libnet_build_udp(src_port, dst_port, LIBNET_UDP_H + payload_s, 0, (const uint8_t *)payload, payload_s, l, 0);

    if (tag < 0)
    {
        cout << "libnet_build_udp():" << libnet_geterror(l) << endl;
        exit(-1);
    }
    tag = libnet_build_ipv4(LIBNET_IPV4_H + LIBNET_UDP_H + payload_s, 0, (u_short)libnet_get_prand(LIBNET_PRu16), 0, (u_int8_t)libnet_get_prand(LIBNET_PR8), IPPROTO_UDP, 0, src_ip, dst_ip, NULL, 0, l, 0);
    if (tag < 0)
    {
        cout << "libnet_build_ipv4():" << libnet_geterror(l) << endl;
        exit(-1);
    }
    tag = libnet_build_ethernet(dst_mac, src_mac, ETHERTYPE_IP, NULL, 0, l, 0);
    if (tag < 0)
    {
        cout << "libnet_build_ethernet():" << libnet_geterror(l) << endl;
        exit(-1);
    }
    res = libnet_write(l);
    if (res < 0)
    {
        cout << "libnet_write():" << libnet_geterror(l) << endl;
        exit(-1);
    }
    cout << "send udp data" << endl;
    cout
        << "src:" << libnet_addr2name4(src_ip, LIBNET_DONT_RESOLVE) << ":" << src_port << endl
        << "dst:" << libnet_addr2name4(dst_ip, LIBNET_DONT_RESOLVE) << ":" << dst_port << endl
        << "AC 1E 81 2e:" << libnet_addr2name4((u_int32_t)0xac1e812e, LIBNET_DONT_RESOLVE) << endl;
    libnet_destroy(l);
    return 0;
}