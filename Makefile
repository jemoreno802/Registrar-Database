CC = gcc
CFLAGS = -g -std=c99 -Wall -Werror

db: db.c csg.c cp.c snap.c cdh.c cr.c crdh.c
	gcc -o db db.c csg.c cp.c snap.c cdh.c cr.c crdh.c -I.
