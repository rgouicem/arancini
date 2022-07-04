#ifndef LIBTCG_H
#define LIBTCG_H

#include <stdint.h>
#include <stdbool.h>

#include "queue.h"


/* We hardcode these for x86_64 target for the time being. */
typedef int64_t  target_long;
typedef uint64_t target_ulong;
typedef int64_t tcg_target_long;
typedef uint64_t tcg_target_ulong;
#define TCG_TARGET_REG_BITS  64
#define TCG_TARGET_NB_REGS   32


#define HOST_LONG_BITS (__SIZEOF_POINTER__ * 8) /* from include/qemu/compiler.h */

/* Has to match the values in include/tcg/tcg.h in QEMU. */
#if HOST_LONG_BITS == 32
#define MAX_OPC_PARAM_PER_ARG 2
#else
#define MAX_OPC_PARAM_PER_ARG 1
#endif
#define MAX_OPC_PARAM_IARGS 7
#define MAX_OPC_PARAM_OARGS 1
#define MAX_OPC_PARAM_ARGS (MAX_OPC_PARAM_IARGS + MAX_OPC_PARAM_OARGS)
/* A Call op needs up to 4 + 2N parameters on 32-bit archs,
 * and up to 4 + N parameters on 64-bit archs
 * (N = number of input arguments + output arguments).  */
#define MAX_OPC_PARAM (4 + (MAX_OPC_PARAM_PER_ARG * MAX_OPC_PARAM_ARGS))

#if TCG_TARGET_NB_REGS <= 32
typedef uint32_t TCGRegSet;
#elif TCG_TARGET_NB_REGS <= 64
typedef uint64_t TCGRegSet;
#else
#error unsupported
#endif


/* Generate the TCG opcodes from the QEMU submodule. */
typedef enum TCGOpcode {
#define DEF(name, oargs, iargs, cargs, flags) INDEX_op_ ## name,
#include "../qemu/include/tcg/tcg-opc.h"
#undef DEF
    NB_OPS,
} TCGOpcode;

typedef tcg_target_ulong TCGArg;
/*
 * The layout here is designed to avoid a bitfield crossing of
 * a 32-bit boundary, which would cause GCC to add extra padding.
 *
 * Has to match the structure layout in include/tcg/tcg.h of QEMU
 */
typedef struct TCGOp {
    TCGOpcode opc   : 8;        /*  8 */

    /* Parameters for this opcode.  See below.  */
    unsigned param1 : 4;        /* 12 */
    unsigned param2 : 4;        /* 16 */

    /* Lifetime data of the operands.  */
    unsigned life   : 16;       /* 32 */

    /* Next and previous opcodes.  */
    QTAILQ_ENTRY(TCGOp) link;

    /* Arguments for the opcode.  */
    TCGArg args[MAX_OPC_PARAM];

    /* Register preferences for the output(s).  */
    TCGRegSet output_pref[2];
} TCGOp;


/*
 * Has to match the structure layout in include/tcg/libtcg.h in QEMU,
 * except for the flexible array at the end for private library data.
 */
struct libtcg_ctx {
    /* Source (guest) */
    uint64_t guest_size;
    uint64_t guest_icount;
    target_ulong guest_pc;
    target_ulong *guest_insn_buffer;
    /* TCG IR */
    uint64_t tcg_size;
    uint64_t tcg_icount;
    TCGOp   *tcg_insn;
    /* Target (host) */
    uint64_t host_size;
    uint64_t host_icount;

    /* Private */
    char private[];
};
typedef struct libtcg_ctx libtcg_ctx;


libtcg_ctx *init_libtcg(int argc, char **argv, char **envp);
int translate_tb_to_tcg(libtcg_ctx *ctx);
void tcg_dump_op(TCGOp *op, bool have_prefs);

#endif  /* LIBTCG_H */
