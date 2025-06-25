#ifndef POOLA_H
#define POOLA_H

/// Parameters
/// ONLY CHANGE DEFINITIONS MARKED WITH 'PARAM'

/// @brief The number of threads to run
#define POOLA_PARAM_THREAD_COUNT 8

/// @brief The structure that the thread has access to
struct poola_PARAM_thread_output {
  int index;
};

/// Defines
#define POOLA_THREAD_COUNT POOLA_PARAM_THREAD_COUNT

/// Data Structures
typedef struct poola_PARAM_thread_output poola_thread_output;

/// Functions

/**
 * The first function you should run after you made sure all of the configurable parameters are to your liking
 * @param poola_thread_function here you provide the function that you want each thread to run, the argument is the output interface of each thread
 * @return 0 for success and -1 for error; more information about the error is going to be printed in the 'stderr' output
 */
int poola_init(void (*poola_thread_function)(poola_thread_output *));

/**
 * The function to call to run / rerun the threads
 * @return 0 for success and -1 for error; more information about the error is going to be printed in the 'stderr' output
 */
int poola_run(void);

/**
 * The function to call to get a freshly allocated/reallocated output; it grows every time you run this function by copying the latest outputs in this new consolidated output
 * @return anything for success and NULL for error; more information about the error is going to be printed in the 'stderr' output
 */
poola_thread_output *poola_out(void);

/**
 * The last function you should run, to cleanup after the program
 */
void poola_free(void);

#endif // POOLA_H