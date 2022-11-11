#ifndef __CAPDUMP_H_
#define __CAPDUMP_H_

#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <pcap.h>
// #include "pcap-bpf.h"
// #include "pcap-int.h"

typedef enum cap_state{
    cap_start,
    cap_stop,
    cap_suspend
}cap_state;

typedef struct cap_dev{
    uint8_t dev[32];
    int32_t snaplen;
    int32_t promisc;
    int32_t to_ms;
}cap_dev;

typedef struct cap_userdata{
    cap_dev dev;
    pcap_t *pcap_handle;
    pcap_dumper_t *pcap_dumper;
    uint8_t fname[64];
    uint8_t filter_ruler[1024];
    pthread_t thread_id;
    cap_state state;
}cap_userdata;

void *cap_proc_thread(void *args);

#endif