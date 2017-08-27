BIN    = .
CC     = gcc
CPLP   =
#-----------------------------------------------------------------------------
CFLAGS = -O3 -Wall $(CPLP)
#-----------------------------------------------------------------------------
LIBS   = -lm
DEPS   = defs.h
PROGS  = $(BIN)/XC # $(BIN)/XD
OBJS   = mem.o msg.o common.o alphabet.o buffer.o levels.o cmcache.o \
         pmodels.o mclass.o template.o array.o hash.o tolerant.o cm.o \
         bitio.o arith.o arith_aux.o 	
#-----------------------------------------------------------------------------
all:
	$(MAKE) progs
progs: $(PROGS)
$(BIN)/XC: xc.c $(DEPS) $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN)/XC xc.c $(OBJS) $(LIBS)
$(BIN)/XD: xd.c $(DEPS) $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN)/XD xd.c $(OBJS) $(LIBS)
mem.o: mem.c mem.h $(DEPS)
	$(CC) -c $(CFLAGS) mem.c
msg.o: msg.c msg.h $(DEPS)
	$(CC) -c $(CFLAGS) msg.c
buffer.o: buffer.c buffer.h $(DEPS)
	$(CC) -c $(CFLAGS) buffer.c
common.o: common.c common.h $(DEPS)
	$(CC) -c $(CFLAGS) common.c
template.o: template.c template.h $(DEPS)
	$(CC) -c $(CFLAGS) template.c
array.o: array.c array.h $(DEPS)
	$(CC) -c $(CFLAGS) array.c
hash.o: hash.c hash.h $(DEPS)
	$(CC) -c $(CFLAGS) hash.c
pmodels.o: pmodels.c pmodels.h $(DEPS)
	$(CC) -c $(CFLAGS) pmodels.c
tolerant.o: tolerant.c tolerant.h $(DEPS)
	$(CC) -c $(CFLAGS) tolerant.c
levels.o: levels.c levels.h $(DEPS)
	$(CC) -c $(CFLAGS) levels.c
cm.o: cm.c cm.h $(DEPS)
	$(CC) -c $(CFLAGS) cm.c
cmcache.o: cmcache.c cmcache.h $(DEPS)
	$(CC) -c $(CFLAGS) cmcache.c
mclass.o: mclass.c mclass.h $(DEPS)
	$(CC) -c $(CFLAGS) mclass.c
alphabet.o: alphabet.c alphabet.h $(DEPS)
	$(CC) -c $(CFLAGS) alphabet.c
bitio.o: bitio.c bitio.h
	$(CC) -c $(CFLAGS) bitio.c
arith.o: arith.c arith.h
	$(CC) -c $(CFLAGS) arith.c
arith_aux.o: arith_aux.c arith_aux.h
	$(CC) -c $(CFLAGS) arith_aux.c

clean:
	/bin/rm -f *.o
cleanall:
	/bin/rm -f *.o $(PROGS)
#-----------------------------------------------------------------------------
