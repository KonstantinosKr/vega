//
// objs/dynamics_ispc_sse2.h
// (Header automatically generated by the ispc compiler.)
// DO NOT EDIT THIS FILE.
//

#ifndef ISPC_OBJS_DYNAMICS_ISPC_SSE2_H
#define ISPC_OBJS_DYNAMICS_ISPC_SSE2_H

#include <stdint.h>



#ifdef __cplusplus
namespace ispc { /* namespace */
#endif // __cplusplus

#ifndef __ISPC_ALIGN__
#if defined(__clang__) || !defined(_MSC_VER)
// Clang, GCC, ICC
#define __ISPC_ALIGN__(s) __attribute__((aligned(s)))
#define __ISPC_ALIGNED_STRUCT__(s) struct __ISPC_ALIGN__(s)
#else
// Visual Studio
#define __ISPC_ALIGN__(s) __declspec(align(s))
#define __ISPC_ALIGNED_STRUCT__(s) __ISPC_ALIGN__(s) struct
#endif
#endif

#ifndef __ISPC_STRUCT_master_conpnt__
#define __ISPC_STRUCT_master_conpnt__
struct master_conpnt {
    int32_t master[8];
    int32_t slave[2][8];
    int32_t color[2][8];
    double point[3][8];
    double normal[3][8];
    double depth[8];
    double force[3][8];
    int32_t size;
    struct master_conpnt * next;
    int32_t lock;
};
#endif

#ifndef __ISPC_STRUCT_slave_conpnt__
#define __ISPC_STRUCT_slave_conpnt__
struct slave_conpnt {
    int32_t master[2][8];
    double point[3][8];
    double force[3][8];
    int32_t size;
    struct slave_conpnt * next;
    int32_t lock;
};
#endif


///////////////////////////////////////////////////////////////////////////
// Functions exported from ispc code
///////////////////////////////////////////////////////////////////////////
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
extern "C" {
#endif // __cplusplus
    extern void dynamics(int32_t threads, struct master_conpnt * master, struct slave_conpnt * slave, int32_t parnum, double *  * angular, double *  * linear, double *  * rotation, double *  * position, double *  * inertia, double *  * inverse, double * mass, double * invm, double *  * force, double *  * torque, double * gravity, double step);
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
} /* end extern C */
#endif // __cplusplus


#ifdef __cplusplus
} /* namespace */
#endif // __cplusplus

#endif // ISPC_OBJS_DYNAMICS_ISPC_SSE2_H
