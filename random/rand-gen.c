/* File xxx-gen.xc is a template file
 *
 * This is either the template file xxx-gen.xc or else is generated
 * automatically from it.  If you want to make changes to this file,
 * make sure that the changes are to the template file xxx-gen.xc;
 * you can then rebuild the files rand-switch.c, rand-gen.c, and rand.h by
 * running the script 'makesrc rand' If you run the script with no
 * arguments, it makes all built-sources
 */


#include "rand.h"             /* defines gsl_ran_rand_ prototypes */
#include "gsl_ran.h"         /* defines gsl_ran_ prototypes */

inline unsigned long gsl_ran_random_wstate(void *vState) {
    return gsl_ran_rand_random_wstate(vState);
}
inline double gsl_ran_uniform_wstate(void *vState) {
    return gsl_ran_rand_uniform_wstate(vState);
}
inline double gsl_ran_max() {
    return gsl_ran_rand_max();
}
inline void gsl_ran_seed_wstate(void *vState, int seed) {
    gsl_ran_rand_seed_wstate(vState,seed);
}
inline unsigned long gsl_ran_random() {
    return gsl_ran_rand_random();
}
inline double gsl_ran_uniform() {
    return gsl_ran_rand_uniform();
}
inline void gsl_ran_seed(int seed) {
    gsl_ran_rand_seed(seed);
}
inline void *gsl_ran_getRandomState() {
    return gsl_ran_rand_getRandomState();
}
inline void gsl_ran_setRandomState(void *vState) {
    gsl_ran_rand_setRandomState(vState);
}
inline void gsl_ran_printState(void *vState) {
     gsl_ran_rand_printState(vState);
}




