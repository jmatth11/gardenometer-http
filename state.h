#ifndef GARDENOMETER_STATE_H
#define GARDENOMETER_STATE_H

typedef enum {
  NONE,
  ERROR,
  CLEAR_ERROR,
  NO_CONNECTION,
  SENDING_DATA
} state_t;

typedef struct state_machine {
  state_t state;
  void(*no_connection)(struct state_machine* sm, void* context);
  void(*sending_data)(struct state_machine* sm, void* context);
  void(*error)(struct state_machine* sm, void* context);
} state_machine_t;

void handle_state_machine(state_machine_t* sm, void* context);

#endif
