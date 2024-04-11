#include "state.h"

void handle_state_machine(state_machine_t* sm, void* context) {
  switch(sm->state) {
    case NO_CONNECTION:
      sm->no_connection(sm, context);
      break;
    case SENDING_DATA:
      sm->sending_data(sm, context);
      break;
    case ERROR:
      sm->error(sm, context);
      break;
    case CLEAR_ERROR:
      sm->error(sm, context);
      break;
    default:
      break;
  }
}
