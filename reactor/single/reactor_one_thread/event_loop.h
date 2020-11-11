//
// Created by Sikey Liu on 2020/11/11.
//

#ifndef NETWORKACTION_EVENT_LOOP_H
#define NETWORKACTION_EVENT_LOOP_H

typedef struct {
    bool finish_flag;

    const event_dispatcher_st *event_dispatcher_ptr;
    void *event_dispatcher_data;
} event_loop_st;

#endif //NETWORKACTION_EVENT_LOOP_H
