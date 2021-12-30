
# ********************************************************************
#  Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
#  the Japan Society for the Promotion of Science (JSPS)
#  All Rights Reserved
# ********************************************************************

include ../../Makefile.in


TARGET   = $(LIBFW_S) $(LIBFW_P) $(LIBFW_H)


SRCS     = struct.c mem.c timefunc.c iofunc.c comm_mpi.c fdname.c fileio.c func_main.c sort.c real.c complex_adv.c errormessage.c
OBJS     = struct.o mem.o timefunc.o iofunc.o comm_mpi.o fdname.o fileio.o func_main.o sort.o real.o complex_adv.o errormessage.o

SRCS_S   = $(SRCS)
SRCS_P   = $(SRCS)
SRCS_H   = $(SRCS)

OBJS_S   = $(OBJS)
OBJS_P   = $(OBJS)
OBJS_H   = $(OBJS)


INCLUDES = -I./ $(INCLUDE_ADV)


all:$(TARGET)
$(LIBFW_S):$(SRCS_S)
	rm -f $(OBJS_S) $(LIBFW_S)
	make single CC="$(CC)" AR="$(AR)" ARFLAGS="$(ARFLAGS)" CFLAGS="$(CFLAGS) -D$(MODE_SINGLE)"
$(LIBFW_P):$(SRCS_P)
	rm -f $(OBJS_P) $(LIBFW_P)
	make parallel CC="$(MPI_CC)" AR="$(AR)" ARFLAGS="$(ARFLAGS)" CFLAGS="$(CFLAGS) -D$(MODE_PARALLEL)"
$(LIBFW_H):$(SRCS_H)
	rm -f $(OBJS_H) $(LIBFW_H)
	make hierarchical CC="$(MPI_CC)" AR="$(AR)" ARFLAGS="$(ARFLAGS)" CFLAGS="$(CFLAGS) -D$(MODE_HIERARCHICAL)"

single:$(OBJS_S)
	$(AR) $(ARFLAGS) $(LIBFW_S) $(OBJS_S)
parallel:$(OBJS_P)
	$(AR) $(ARFLAGS) $(LIBFW_P) $(OBJS_P)
hierarchical:$(OBJS_H)
	$(AR) $(ARFLAGS) $(LIBFW_H) $(OBJS_H)

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -D$(OPERATING_SYSTEM) -c $<
clean:
	rm -f $(OBJS_H) $(OBJS_P) $(OBJS_S) $(LIBFW_S) $(LIBFW_P) $(LIBFW_H)
