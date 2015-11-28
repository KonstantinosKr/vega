//
// objs/partition_ispc_sse4.h
// (Header automatically generated by the ispc compiler.)
// DO NOT EDIT THIS FILE.
//

#ifndef ISPC_OBJS_PARTITION_ISPC_SSE4_H
#define ISPC_OBJS_PARTITION_ISPC_SSE4_H

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


///////////////////////////////////////////////////////////////////////////
// Functions exported from ispc code
///////////////////////////////////////////////////////////////////////////
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
extern "C" {
#endif // __cplusplus
    extern struct partitioning *  partitioning_create(int32_t threads, int32_t ellnum, double *  * center);
    extern void partitioning_destroy(struct partitioning * tree);
    extern int32_t partitioning_store(int32_t threads, struct partitioning * tree, int32_t ellnum, int32_t * ellcol, int32_t * part, double *  * center, double *  * radii, double *  * orient);
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
} /* end extern C */
#endif // __cplusplus


#ifdef __cplusplus
} /* namespace */
#endif // __cplusplus

#endif // ISPC_OBJS_PARTITION_ISPC_SSE4_H
