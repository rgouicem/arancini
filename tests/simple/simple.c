#include <stdio.h>

#include "libtcg.h"
#include "queue.h"

int main(int argc, char **argv, char **envp)
{
    int ret;
    /* target_ulong *code = {}; */
    libtcg_ctx *ctx = init_libtcg(argc, argv, envp);

    ret = translate_tb_to_tcg(ctx);
    if (ret) {
	return -1;
    }

    printf("tcg_icount=%lu\n",
	   ctx->tcg_icount);
    printf("tcg_insn:\n");
    for (int i = 0; i < ctx->tcg_icount; i++) {
	    tcg_dump_op(&ctx->tcg_insn[i], false);
    }

    return 0;
}
