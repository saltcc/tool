#include "capdump.h"

#define PKTS_COUNT_IS_UNLIMITED(count)  ((count) <= 0)

void cap_callback(unsigned char *userdata, const struct pcap_pkthdr *packet_header, const unsigned char *packet_content)
{
    cap_userdata *cudata = (cap_userdata *)userdata;

    if (!cudata || !cudata->pcap_dumper || !cudata->pcap_handle){
        return;
    }

    if (cudata->state == cap_stop){
        return;
    }

    if (cudata->state == cap_suspend){
        return;
    }

    pcap_dump((u_char *)cudata->pcap_dumper, packet_header, packet_content);

    return;
}

int cap_pcap_loop(pcap_t *p, int cnt, pcap_handler callback, u_char *user)
{
    register int n;

    cap_userdata *cudata = (cap_userdata *)user;

    if (!cudata){
        return 0;
    }

    for (;;) {

        do{
            if (cudata->state == cap_stop){
                return 0;
            }

            n = pcap_dispatch(cudata->pcap_handle, cnt, callback, user);

        }while (n == 0);
        
        if (n <= 0)
            return (n);

        if (!PKTS_COUNT_IS_UNLIMITED(cnt)) {
            cnt -= n;
            if (cnt <= 0)
                return (0);
        }
    }
}

void *cap_proc_thread(void *args)
{
    cap_userdata *cudata = (cap_userdata *)args;

    if (!cudata){
        return cudata;
    }
    char ebuf[1024] = {0};
    cudata->pcap_handle = pcap_open_live((const char *)cudata->dev.dev, cudata->dev.snaplen, cudata->dev.promisc, cudata->dev.to_ms, ebuf);

    if (!cudata->pcap_handle){
        printf("pcap_open_live fail : %s\n", ebuf);
        return cudata;
    }
    
    struct bpf_program filter;
    if (pcap_compile(cudata->pcap_handle, &filter, (const char *)cudata->filter_ruler, 1, 0) < 0){
        printf("pcap_compile fail\n");
        return cudata;
    }

    pcap_setfilter(cudata->pcap_handle, &filter);

    cudata->pcap_dumper = pcap_dump_open(cudata->pcap_handle, (const char *)cudata->fname);

    if (!cudata->pcap_dumper){
        printf("pcap_dump_open fail, filename:%s\n", cudata->fname);
        return cudata;
    }

    if (pcap_setnonblock(cudata->pcap_handle, 1, ebuf) == -1){
        printf("pcap_setnonblock fail : %s\n", ebuf);
        return cudata;
    }

    cap_pcap_loop(cudata->pcap_handle, -1, cap_callback, (unsigned char *)cudata);

    if (cudata->pcap_handle){
        pcap_close(cudata->pcap_handle);
    }

    if (cudata->pcap_dumper){
        pcap_dump_close(cudata->pcap_dumper);
    }

    return cudata;
}