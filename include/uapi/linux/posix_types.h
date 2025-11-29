/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _LINUX_POSIX_TYPES_H
#define _LINUX_POSIX_TYPES_H

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_STDDEF_H
#define _LINUX_STDDEF_H

/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_STDDEF_H
#define _UAPI_LINUX_STDDEF_H

#ifdef __KERNEL__
#include <linux/compiler_types.h>
#endif

#ifndef __always_inline
#define __always_inline inline
#endif

/* Not all C++ standards support type declarations inside an anonymous union */
#ifndef __cplusplus
#define __struct_group_tag(TAG)		TAG
#else
#define __struct_group_tag(TAG)
#endif

/**
 * __struct_group() - Create a mirrored named and anonyomous struct
 *
 * @TAG: The tag name for the named sub-struct (usually empty)
 * @NAME: The identifier name of the mirrored sub-struct
 * @ATTRS: Any struct attributes (usually empty)
 * @MEMBERS: The member declarations for the mirrored structs
 *
 * Used to create an anonymous union of two structs with identical layout
 * and size: one anonymous and one named. The former's members can be used
 * normally without sub-struct naming, and the latter can be used to
 * reason about the start, end, and size of the group of struct members.
 * The named struct can also be explicitly tagged for layer reuse (C only),
 * as well as both having struct attributes appended.
 */
#define __struct_group(TAG, NAME, ATTRS, MEMBERS...) \
	union { \
		struct { MEMBERS } ATTRS; \
		struct __struct_group_tag(TAG) { MEMBERS } ATTRS NAME; \
	} ATTRS

#ifdef __cplusplus
/* sizeof(struct{}) is 1 in C++, not 0, can't use C version of the macro. */
#define __DECLARE_FLEX_ARRAY(T, member)	\
	T member[0]
#else
/**
 * __DECLARE_FLEX_ARRAY() - Declare a flexible array usable in a union
 *
 * @TYPE: The type of each flexible array element
 * @NAME: The name of the flexible array member
 *
 * In order to have a flexible array member in a union or alone in a
 * struct, it needs to be wrapped in an anonymous struct with at least 1
 * named member, but that member can be empty.
 */
#define __DECLARE_FLEX_ARRAY(TYPE, NAME)	\
	struct { \
		struct { } __empty_ ## NAME; \
		TYPE NAME[]; \
	}
#endif

#ifndef __counted_by
#define __counted_by(m)
#endif

#ifndef __counted_by_le
#define __counted_by_le(m)
#endif

#ifndef __counted_by_be
#define __counted_by_be(m)
#endif

#ifdef __KERNEL__
#define __kernel_nonstring	__nonstring
#else
#define __kernel_nonstring
#endif

#endif /* _UAPI_LINUX_STDDEF_H */

#undef NULL
#define NULL ((void *)0)

enum {
	false	= 0,
	true	= 1
};

#undef offsetof
#define offsetof(TYPE, MEMBER)	__builtin_offsetof(TYPE, MEMBER)

/**
 * sizeof_field() - Report the size of a struct field in bytes
 *
 * @TYPE: The structure containing the field of interest
 * @MEMBER: The field to return the size of
 */
#define sizeof_field(TYPE, MEMBER) sizeof((((TYPE *)0)->MEMBER))

/**
 * offsetofend() - Report the offset of a struct field within the struct
 *
 * @TYPE: The type of the structure
 * @MEMBER: The member within the structure to get the end offset of
 */
#define offsetofend(TYPE, MEMBER) \
	(offsetof(TYPE, MEMBER)	+ sizeof_field(TYPE, MEMBER))

/**
 * struct_group() - Wrap a set of declarations in a mirrored struct
 *
 * @NAME: The identifier name of the mirrored sub-struct
 * @MEMBERS: The member declarations for the mirrored structs
 *
 * Used to create an anonymous union of two structs with identical
 * layout and size: one anonymous and one named. The former can be
 * used normally without sub-struct naming, and the latter can be
 * used to reason about the start, end, and size of the group of
 * struct members.
 */
#define struct_group(NAME, MEMBERS...)	\
	__struct_group(/* no tag */, NAME, /* no attrs */, MEMBERS)

/**
 * struct_group_attr() - Create a struct_group() with trailing attributes
 *
 * @NAME: The identifier name of the mirrored sub-struct
 * @ATTRS: Any struct attributes to apply
 * @MEMBERS: The member declarations for the mirrored structs
 *
 * Used to create an anonymous union of two structs with identical
 * layout and size: one anonymous and one named. The former can be
 * used normally without sub-struct naming, and the latter can be
 * used to reason about the start, end, and size of the group of
 * struct members. Includes structure attributes argument.
 */
#define struct_group_attr(NAME, ATTRS, MEMBERS...) \
	__struct_group(/* no tag */, NAME, ATTRS, MEMBERS)

/**
 * struct_group_tagged() - Create a struct_group with a reusable tag
 *
 * @TAG: The tag name for the named sub-struct
 * @NAME: The identifier name of the mirrored sub-struct
 * @MEMBERS: The member declarations for the mirrored structs
 *
 * Used to create an anonymous union of two structs with identical
 * layout and size: one anonymous and one named. The former can be
 * used normally without sub-struct naming, and the latter can be
 * used to reason about the start, end, and size of the group of
 * struct members. Includes struct tag argument for the named copy,
 * so the specified layout can be reused later.
 */
#define struct_group_tagged(TAG, NAME, MEMBERS...) \
	__struct_group(TAG, NAME, /* no attrs */, MEMBERS)

/**
 * DECLARE_FLEX_ARRAY() - Declare a flexible array usable in a union
 *
 * @TYPE: The type of each flexible array element
 * @NAME: The name of the flexible array member
 *
 * In order to have a flexible array member in a union or alone in a
 * struct, it needs to be wrapped in an anonymous struct with at least 1
 * named member, but that member can be empty.
 */
#define DECLARE_FLEX_ARRAY(TYPE, NAME) \
	__DECLARE_FLEX_ARRAY(TYPE, NAME)

/**
 * __TRAILING_OVERLAP() - Overlap a flexible-array member with trailing
 *			  members.
 *
 * Creates a union between a flexible-array member (FAM) in a struct and a set
 * of additional members that would otherwise follow it.
 *
 * @TYPE: Flexible structure type name, including "struct" keyword.
 * @NAME: Name for a variable to define.
 * @FAM: The flexible-array member within @TYPE
 * @ATTRS: Any struct attributes (usually empty)
 * @MEMBERS: Trailing overlapping members.
 */
#define __TRAILING_OVERLAP(TYPE, NAME, FAM, ATTRS, MEMBERS)			\
	union {									\
		TYPE NAME;							\
		struct {							\
			unsigned char __offset_to_FAM[offsetof(TYPE, FAM)];	\
			MEMBERS							\
		} ATTRS;							\
	}

/**
 * TRAILING_OVERLAP() - Overlap a flexible-array member with trailing members.
 *
 * Creates a union between a flexible-array member (FAM) in a struct and a set
 * of additional members that would otherwise follow it.
 *
 * @TYPE: Flexible structure type name, including "struct" keyword.
 * @NAME: Name for a variable to define.
 * @FAM: The flexible-array member within @TYPE
 * @MEMBERS: Trailing overlapping members.
 */
#define TRAILING_OVERLAP(TYPE, NAME, FAM, MEMBERS)				\
	__TRAILING_OVERLAP(TYPE, NAME, FAM, /* no attrs */, MEMBERS)

#endif // _LINUX_STDDEF_H

/*
 * This allows for 1024 file descriptors: if NR_OPEN is ever grown
 * beyond that you'll have to change this too. But 1024 fd's seem to be
 * enough even for such "real" unices like OSF/1, so hopefully this is
 * one limit that doesn't have to be changed [again].
 *
 * Note that POSIX wants the FD_CLEAR(fd,fdsetp) defines to be in
 * <sys/time.h> (and thus <linux/time.h>) - but this is a more logical
 * place for them. Solved by having dummy defines in <sys/time.h>.
 */

/*
 * This macro may have been defined in <gnu/types.h>. But we always
 * use the one here.
 */
#undef __FD_SETSIZE
#define __FD_SETSIZE	1024

typedef struct {
	unsigned long fds_bits[__FD_SETSIZE / (8 * sizeof(long))];
} __kernel_fd_set;

/* Type of a signal handler.  */
typedef void (*__kernel_sighandler_t)(int);

/* Type of a SYSV IPC key.  */
typedef int __kernel_key_t;
typedef int __kernel_mqd_t;

#include <asm/posix_types.h>

#endif /* _LINUX_POSIX_TYPES_H */
