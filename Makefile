liner:
	make -C ./lib/mgx
	gcc src/liner.c -L./lib/mgx -lmgx -I./lib/mgx/inc -framework OpenGL -framework AppKit