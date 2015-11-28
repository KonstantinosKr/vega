#ifndef HEADER_FILE
#define HEADER_FILE

#include "bf_ispc.h"

#define CONBUF 8

/* master contact points; global array is used because
 * constitutive data at contacts can be persistent */
struct master_conpnt
{
  int master[CONBUF]; /* ellipsoid */
  int slave[2][CONBUF]; /* particle, ellipsoid or -(triangle+1), unused */
  int color[2][CONBUF];
  iREAL point[3][CONBUF];
  iREAL normal[3][CONBUF];
  iREAL depth[CONBUF];
  iREAL force[3][CONBUF];
  int size;

  struct master_conpnt * next; /* local list */
  int lock; /* list update lock */
};

/* slave contact points; they are created by
 * symmetrically coppying master contact points */
struct slave_conpnt
{
  int master[2][CONBUF]; /* particle, ellipsoid */
  iREAL point[3][CONBUF];
  iREAL force[3][CONBUF];
  int size;

  struct slave_conpnt * next; /* local list */
  int lock; /* list update lock */
};

/* calculate distances */
void contact_detection (unsigned int s1, unsigned int e1, unsigned int s2, unsigned int e2,  unsigned int size, iREAL *t[3][3], iREAL *p[3], iREAL *q[3], iREAL *distance);

#endif
