#ifndef EVENT_H
#define EVENT_H

#define EVENT_TIMEOUT 0x01
#define EVENT_READ 0x02
#define EVENT_WRITE 0x04
#define EVENT_SIGNAL 0x08

typedef int (*event_read_callback_fn)(void *data);

typedef int (*event_write_callback_fn)(void *data);

typedef struct {
    int fd;

    // 事件类型
    int type;

    // 读事件回调函数
    event_read_callback_fn event_read_callback;

    // 写事件回调函数
    event_read_callback_fn event_write_callback;

    // 回调函数需要的参数
    void *data;
} event_st;

event_st *event_new(int fd, int type,
                    event_read_callback_fn event_read_callback,
                    event_read_callback_fn event_write_callback, void *data);

int write_event_is_enabled(event_st *event_ptr);

int write_event_enable(event_st *event_ptr);

int write_event_disable(event_st *event_ptr);

#endif