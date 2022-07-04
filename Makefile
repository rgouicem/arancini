all: tests

libtcg:
	mkdir -p libtcg
	cd libtcg && ../qemu/configure  --target-list=x86_64-linux-user && make -j $(shell nproc)

tests: libtcg
	make -C tests

clean:
	make -C libtcg clean

mrproper:
	rm -rf libtcg

.PHONY: tests libtcg clean mrproper all
