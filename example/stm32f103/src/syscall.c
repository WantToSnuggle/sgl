#include <unistd.h>
#include <errno.h>

// _exit 是必须的
void _exit(int status) {
    (void)status;
    while (1);
}

// _close 实现
int _close(int file) {
    (void)file;
    return -1; // 表示失败
}

int _isatty(int fd) {
    return 1; // 假设是终端设备
}

int _lseek(int fd, int ptr, int dir) {
    (void)fd;
    (void)ptr;
    (void)dir;
    return -1;
}

int _read(int fd, char *ptr, int len) {
    (void)fd;
    (void)ptr;
    (void)len;
    return -1;
}

int _write(int fd, char *ptr, int len) {
    (void)fd;
    // 可以在这里添加串口输出等逻辑
    return len;
}

int _fstat(int fd, void *st) {
    (void)fd;
    (void)st;
    return 0;
}

void *_sbrk(int incr) {
    return NULL;
}
