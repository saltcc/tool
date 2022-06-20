#include <execinfo.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define FILEPATHSIZE 1024

typedef void (*sigaction_t)(int, siginfo_t *, void *);
typedef void (*sighandler_t)(int);

static void print_signal_info(int signo, siginfo_t *info, void *ptr)
{
    char *env_path = (char *)getenv("HOME");
    if (env_path == NULL){
        return;
    }

    char command[1024] = {0};
    sprintf(command, "echo \"`date`, Process signal %d caught\" >> %s/log/monitor.log", 
        signo, env_path);
    system(command);

    snprintf(command, sizeof(command),
        "echo \"\tsiginfo:signo %d, errno %d, code %d, pid %d, uid %d, status %d, value %d, addr %lx\" >> %s/log/monitor.log",
        info->si_signo, info->si_errno, info->si_code, 
        info->si_pid, info->si_uid, info->si_status, 
        info->si_value.sival_int, (unsigned long)info->si_addr, env_path);

    system(command);
}

static void trace_stack_info(const unsigned char *process_name, pid_t pid)
{
    unsigned char command[1024] = {0};
    char *env_path = (char *)getenv("HOME");
    if (env_path == NULL){
        return;
    }

    snprintf(command, sizeof(command),
        "echo btexit >> %s/log/%sstacktrace.log;"
        "echo \"====current time:`date`====\" >> %s/log/%sstacktrace.log;"
        "echo \"====gstack process pid:%d====\" >> %s/log/%sstacktrace.log;"
        "gstack %d >> %s/log/%sstacktrace.log 2>&1;"
        "echo \"=============================\" >> %s/log/%sstacktrace.log;",
        env_path, process_name,
        env_path, process_name,
        pid, env_path, process_name,
        pid, env_path, process_name,
        env_path, process_name);

    system(command);
}

static void backtrace_exit()
{
    char command[1024] = {0};
    const unsigned char *process_name = NULL;
    
    char process_path[FILEPATHSIZE];

    pid_t pid = getpid();
    int result = readlink("/proc/self/exe", process_path, FILEPATHSIZE - 1);
    if (result <= 0 || result > FILEPATHSIZE - 1){
        return;
    }
    process_path[result] = '\0';

    process_name = strrchr(process_path, '/');

    if (NULL != process_name){
        process_name++;
        trace_stack_info(process_name, pid);
    }
}

static void signal_core_handle(int signo, siginfo_t *info, void *ptr)
{
    backtrace_exit();

    print_signal_info(signo, info, ptr);

    signal(signo, SIG_DFL);

    raise(signo);
}

static sigaction_t signal_act(int signo, sigaction_t func)
{
    struct sigaction act;
    struct sigaction oact;

    act.sa_handler = (sighandler_t)func;
    act.sa_sigaction = func;

    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;

#ifdef SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;
#endif

    if (sigaction(signo, &act, &oact) < 0)
        return (sigaction_t)(SIG_ERR);
    return (sigaction_t)(oact.sa_handler);
}

void moniter_signal()
{
    setsid();

    signal_act(SIGABRT,     signal_core_handle);
    signal_act(SIGPOLL,     signal_core_handle);
    signal_act(SIGPROF,     signal_core_handle);
    signal_act(SIGSYS,      signal_core_handle);
    signal_act(SIGTRAP,     signal_core_handle);
    signal_act(SIGXCPU,     signal_core_handle);
    signal_act(SIGXFSZ,     signal_core_handle);
    signal_act(SIGIOT,      signal_core_handle);
    signal_act(SIGSTKFLT,   signal_core_handle);
    signal_act(SIGIO,       signal_core_handle);
    signal_act(SIGPWR,      signal_core_handle);
    // signal_act(SIGUNUSED,    signal_core_handle);
    signal_act(SIGSYS,      signal_core_handle);
    signal_act(SIGBUS,      signal_core_handle);
    signal_act(SIGSEGV,     signal_core_handle);
    signal_act(SIGFPE,      signal_core_handle);
    signal_act(SIGILL,      signal_core_handle);
    
    signal(SIGINT,  SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGHUP,  SIG_IGN);
    signal(SIGCLD,  SIG_DFL);

    // signal(SIGTERM,);
}