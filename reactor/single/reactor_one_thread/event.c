#include "event.h"

#include <stdlib.h>

event_st *event_new(int fd, int type,
                    event_read_callback_fn event_read_callback,
                    event_read_callback_fn event_write_callback, void *data) {
    event_st *event_ptr = malloc(sizeof(event_st));
    event_ptr->fd = fd;
    event_ptr->type = type;
    event_ptr->event_read_callback = event_read_callback;
    event_ptr->event_write_callback = event_write_callback;
    event_ptr->data = data;
    return event_ptr;
}

int write_event_is_enabled(event_st *event_ptr) {
    return event_ptr->type & EVENT_WRITE;
}

int write_event_enable(event_st *event_ptr) { return 0; }

int write_event_disable(event_st *event_ptr) { return 0; }