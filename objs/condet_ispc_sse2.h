//
// objs/condet_ispc_sse2.h
// (Header automatically generated by the ispc compiler.)
// DO NOT EDIT THIS FILE.
//

#ifndef ISPC_OBJS_CONDET_ISPC_SSE2_H
#define ISPC_OBJS_CONDET_ISPC_SSE2_H

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

#ifndef __ISPC_STRUCT_partitioning__
#define __ISPC_STRUCT_partitioning__
struct partitioning {
    double coord;
    int32_t dimension;
    int32_t left;
    int32_t right;
    struct leaf_data * data;
};
#endif

#ifndef __ISPC_STRUCT_leaf_data__
#define __ISPC_STRUCT_leaf_data__
struct leaf_data {
    int32_t size;
    int32_t color[96];
    int32_t part[96];
    int32_t ell[96];
    double center[3][96];
    double radii[3][96];
    double orient[9][96];
};
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
    extern void condet(int32_t threads, struct partitioning * tree, struct master_conpnt * master, int32_t parnum, int32_t ellnum, int32_t * ellcol, int32_t * part, double *  * center, double *  * radii, double *  * orient, int32_t trinum, int32_t * tricol, int32_t * triobs, double * tri[][3]);
    extern struct master_conpnt *  master_alloc(struct master_conpnt * old, int32_t nold, int32_t size);
    extern void master_free(struct master_conpnt * con, int32_t size);
    extern struct slave_conpnt *  slave_alloc(struct slave_conpnt * old, int32_t nold, int32_t size);
    extern void slave_free(struct slave_conpnt * con, int32_t size);
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
} /* end extern C */
#endif // __cplusplus


#ifdef __cplusplus
} /* namespace */
#endif // __cplusplus

#endif // ISPC_OBJS_CONDET_ISPC_SSE2_H
