#ifndef SIMPLE_FSM_V1_H
#define SIMPLE_FSM_V1_H

// Simple fsm variant 1: A bit slow (state processing takes min. O(n) where n - current fsm state), consumes moderate memory and reliable.

#include <fsm_common.h>
#include <stddef.h>
#include <stdbool.h>

struct state_machine_s;
typedef struct state_machine_s state_machine_t;

typedef struct fsm_transition_s
{
    fsm_state_t from_state;
    fsm_event_t event;
    fsm_state_t to_state;
    transition_action_t transition_action;
} fsm_transition_t;

struct state_machine_s
{
    fsm_state_t current_state;
    const fsm_transition_t *transitions;
    size_t transitions_size;
};

void fsm_process_event(state_machine_t *fsm, fsm_event_t event, void *event_data)
{
    if (fsm)
    {
        size_t i = 0;
        for(; i < fsm->transitions_size; i++)
        {
            const fsm_transition_t *transition = fsm->transitions + i;

            if ((transition->from_state == fsm->current_state) && (transition->event == event))
            {
                if (transition->transition_action)
                    transition->transition_action(event_data);

                fsm->current_state = transition->to_state;

                break;
            }
        }
    }
}

#endif