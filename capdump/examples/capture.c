#include "capdump.h"
#include <unistd.h>

int main()
{
    cap_dev dev = {"any", 65535, 1, 2};
    cap_userdata userdata = {0};
    userdata.dev = dev;
    snprintf(userdata.fname, sizeof(userdata.fname), "%s", "./test.pcap");
    snprintf(userdata.filter_ruler, sizeof(userdata.filter_ruler), "%s", "tcp port 22");
    userdata.state = cap_start;
    pthread_create(&userdata.thread_id, 0, cap_proc_thread, &userdata);
    usleep(1000000*3);
    userdata.state = cap_stop;
    printf("capdump 1000000*3/us pcap files\n");
    pthread_join(userdata.thread_id, NULL);
    return 0;
}