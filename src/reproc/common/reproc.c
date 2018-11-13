#include "reproc/reproc.h"

#include "assert.h"

REPROC_ERROR reproc_stop(reproc_type *process, REPROC_CLEANUP c1,
                         unsigned int t1, REPROC_CLEANUP c2, unsigned int t2,
                         REPROC_CLEANUP c3, unsigned int t3,
                         unsigned int *exit_status)
{
  assert(process);

  REPROC_CLEANUP operations[3] = { c1, c2, c3 };
  unsigned int timeouts[3] = { t1, t2, t3 };

  // We don't set error to REPROC_SUCCESS so we can check if
  // wait/terminate/kill succeeded (in which case error is set to
  // REPROC_SUCCESS).
  REPROC_ERROR error = REPROC_WAIT_TIMEOUT;

  for (int i = 0; i < 3; i++) {
    REPROC_CLEANUP operation = operations[i];
    unsigned int timeout = timeouts[i];

    switch (operation) {
    case REPROC_NOOP: break;
    case REPROC_WAIT: error = reproc_wait(process, timeout, exit_status); break;
    case REPROC_TERMINATE:
      error = reproc_terminate(process, timeout, exit_status);
      break;
    case REPROC_KILL: error = reproc_kill(process, timeout, exit_status); break;
    }

    if (error != REPROC_WAIT_TIMEOUT) { break; }
  }

  return error;
}