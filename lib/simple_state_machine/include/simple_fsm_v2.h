#ifndef SIMPLE_FSM_V2_H
#define SIMPLE_FSM_V2_H

// Simple fsm variant 2: fast, but consumes more memory than and less reliable than v1. The user must define all event reactions (even no reaction) otherwise crashes possible.

#include <fsm_common.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#define FSM_NO_TRANSITION {FSM_NULL_EVENT, NULL}

typedef struct fsm_transition_s
{
    fsm_state_t next_state;
    transition_action_t action;
} fsm_transition_t;

typedef struct state_machine_s
{
    fsm_state_t current_state;
    int events_count;
    const fsm_transition_t *transitions;
} state_machine_t;

const fsm_transition_t fsm_no_transition = FSM_NO_TRANSITION;

bool fsm_process_event(state_machine_t *fsm, fsm_event_t event, void *event_data)
{
    bool res = false;

    if (fsm && (event >= 0))
    {
        const fsm_transition_t *transition = fsm->transitions + (fsm->current_state * fsm->events_count) + event;
        if (memcmp(transition, &fsm_no_transition, sizeof(fsm_transition_t)))
        {
            if (transition->action)
                transition->action(event_data);

            fsm->current_state = transition->next_state;
            
            res = true;
        }
    }

    return res;
}
// main.c: example of usage ------------------------------------------------------
// typedef enum
// {
//     STATE_0 = 0,
//     STATE_1,
//     STATE_2,
//     STATES_COUNT
// } test_state_t;

// typedef enum
// {
//     EVENT_0 = 0,
//     EVENT_1,
//     EVENT_2,
//     EVENTS_COUNT
// } test_event_t;

// static void from_0_to_1()
// {
//     printf("Gone from 0 to 1\n");
// }

// static void from_0_to_2()
// {
//     printf("Gone from 0 to 2\n");
// }

// static void from_1_to_0()
// {
//     printf("Gone from 1 to 0\n");
// }

// static void from_1_to_2()
// {
//     printf("Gone from 1 to 2\n");
// }

// static void from_2_to_0()
// {
//     printf("Gone from 2 to 0\n");
// }

// static void from_2_to_1()
// {
//     printf("Gone from 2 to 1\n");
// }

// static const transition_t fsm_transitions[STATES_COUNT][EVENTS_COUNT] =
// {
//     [STATE_0] =
//     {
//         [EVENT_0] = no_transition,
//         [EVENT_1] = {STATE_1, &from_0_to_1},
//         [EVENT_2] = {STATE_2, &from_0_to_2}
//     },
//     [STATE_1] =
//     {
//         [EVENT_0] = {STATE_0, &from_1_to_0},
//         [EVENT_1] = no_transition,
//         [EVENT_2] = {STATE_2, &from_1_to_2}
//     },
//     [STATE_2] =
//     {
//         [EVENT_0] = {STATE_0, &from_2_to_0},
//         [EVENT_1] = {STATE_1, &from_2_to_1},
//         [EVENT_2] = no_transition
//     }
// };

// int main()
// {
//     static fsm_t fsm = {STATE_0, EVENTS_COUNT, (const transition_t*)fsm_transitions};

//     static const event_t events_to_process[] = {EVENT_0, EVENT_1, EVENT_1, EVENT_0, EVENT_2, EVENT_2, EVENT_1, EVENT_2, EVENT_0};
//     size_t i = 0;
//     for(;i < sizeof(events_to_process) / sizeof(events_to_process[0]); i++)
//         if (!fsm_process_event(&fsm, events_to_process[i]))
//             printf("No transition from %d by event %d :(\n", fsm.curr_state, events_to_process[i]);

//     return 0;
// }

#endif