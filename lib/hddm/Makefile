# ********************************************************************
#  Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
#  the Japan Society for the Promotion of Science (JSPS)
#  All Rights Reserved
# ********************************************************************

include ../../Makefile.in


TARGET   = $(LIBHDDM_S) $(LIBHDDM_P) $(LIBHDDM_H)


SRCS     = make_PartMesh.c hddm_solver_rs.c hddm_solver_cs.c set_hddm_func.c advlas_skyline.c
OBJS     = make_PartMesh.o hddm_solver_rs.o hddm_solver_cs.o set_hddm_func.o advlas_skyline.o

SRCS_S   = $(SRCS) make_PartMesh_s.c
SRCS_P   = $(SRCS) make_PartMesh_p.c
SRCS_H   = $(SRCS) make_PartMesh_h.c

OBJS_S   = $(OBJS) make_PartMesh_s.o
OBJS_P   = $(OBJS) make_PartMesh_p.o
OBJS_H   = $(OBJS) make_PartMesh_h.o


LOCAL_LIBFW_DIR = ../../$(LIBFW_DIR)

LOCAL_SOLVER_DIR = ../../$(SOLVER_DIR)

LOCAL_LIBFEM_DIR = ../../$(LIBFEM_DIR)

INCLUDES = $(INCLUDE_ADV) -I./ -I$(LOCAL_LIBFW_DIR) -I$(LOCAL_SOLVER_DIR) -I$(LOCAL_LIBFEM_DIR)


all:$(TARGET)
$(LIBHDDM_S):$(SRCS_S)
	rm -f $(OBJS_S) $(LIBHDDM_S)
	make single CC="$(CC)" AR="$(AR)" ARFLAGS="$(ARFLAGS)" CFLAGS="$(CFLAGS) -D$(MODE_SINGLE)"
$(LIBHDDM_P):$(SRCS_P)
	rm -f $(OBJS_P) $(LIBHDDM_P)
	make parallel CC="$(MPI_CC)" AR="$(AR)" ARFLAGS="$(ARFLAGS)" CFLAGS="$(CFLAGS) -D$(MODE_PARALLEL)"
$(LIBHDDM_H):$(SRCS_H)
	rm -f $(OBJS_H) $(LIBHDDM_H)
	make hierarchical CC="$(MPI_CC)" AR="$(AR)" ARFLAGS="$(ARFLAGS)" CFLAGS="$(CFLAGS) -D$(MODE_HIERARCHICAL)"

single:$(OBJS_S)
	$(AR) $(ARFLAGS) $(LIBHDDM_S) $(OBJS_S)
parallel:$(OBJS_P)
	$(AR) $(ARFLAGS) $(LIBHDDM_P) $(OBJS_P)
hierarchical:$(OBJS_H)
	$(AR) $(ARFLAGS) $(LIBHDDM_H) $(OBJS_H)

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -D$(OPERATING_SYSTEM) -c $<
clean:
	rm -f $(OBJS_H) $(OBJS_P) $(OBJS_S) $(LIBHDDM_S) $(LIBHDDM_P) $(LIBHDDM_H)
