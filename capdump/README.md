# capdump
capture NIC pcap file



# 使用说明

## 接口参数

```cpp
//设置抓包设备名称、snaplen、是否混杂模式promisc、超时时间.cap_dev结构配置抓包参数;
typedef struct cap_dev{
    uint8_t dev[32];
    int32_t snaplen;
    int32_t promisc;
    int32_t to_ms;
}cap_dev;

//抓包上下文设置
typedef struct cap_userdata{
    cap_dev dev;
    pcap_t *pcap_handle;
    pcap_dumper_t *pcap_dumper;
    uint8_t fname[64];
    uint8_t filter_ruler[1024];
    pthread_t thread_id;
    cap_state state;
}cap_userdata;

//捕获控制状态
typedef enum cap_state{
    cap_start,
    cap_stop,
    cap_suspend
}cap_state;

//捕获网卡报文处理接口
void *cap_proc_thread(void *args)
```

## 示例

```cpp
int main()
{
    //capdump参数配置
    cap_dev dev = {"any", 65535, 1, 2};
    cap_userdata userdata = {0};
    userdata.dev = dev;
    snprintf(userdata.fname, sizeof(userdata.fname), "%s", "./test.pcap");
    //捕获过滤条件
    snprintf(userdata.filter_ruler, sizeof(userdata.filter_ruler), "%s", "tcp port 22");
    //开始捕获状态设置
    userdata.state = cap_start;
    //开始捕获
    pthread_create(&userdata.thread_id, 0, cap_proc_thread, &userdata);
    //捕获3秒网卡pcap
    usleep(1000000*3);
    //停止捕获
    userdata.state = cap_stop;
    printf("capdump 1000000*3/us pcap files\n");
    pthread_join(userdata.thread_id, NULL);
    return 0;
}
```



