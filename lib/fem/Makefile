# ********************************************************************
#  Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
#  the Japan Society for the Promotion of Science (JSPS)
#  All Rights Reserved
# ********************************************************************

include ../../Makefile.in


TARGET   = $(LIBFEM)


SRCS     = make_MESH.c tetrahedral.c make_elem.c  make_elem_em.c post.c
OBJS     = make_MESH.o tetrahedral.o make_elem.o  make_elem_em.o post.o


LOCAL_LIBFW_DIR = ../../$(LIBFW_DIR)

LOCAL_SOLVER_DIR = ../../$(SOLVER_DIR)

INCLUDES = $(INCLUDE_ADV) -I./ -I$(LOCAL_LIBFW_DIR) -I$(LOCAL_SOLVER_DIR)


all:$(TARGET)
$(LIBFEM):$(OBJS)
	$(AR) $(ARFLAGS) $(LIBFEM) $(OBJS)


.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -D$(OPERATING_SYSTEM) -c $<
clean:
	rm -f $(OBJS) $(LIBFEM)
