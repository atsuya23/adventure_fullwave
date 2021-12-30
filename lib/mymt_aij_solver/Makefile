# ********************************************************************
#  Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
#  the Japan Society for the Promotion of Science (JSPS)
#  All Rights Reserved
# ********************************************************************

include Makefile.in


TARGET   = $(SOLVER)


SRCS_RS  = mymt_aij_rs.c
SRCS_RA  = mymt_aij_ra.c
SRCS_CS  = mymt_aij_cs.c
SRCS_CA  = mymt_aij_ca.c
SRCS_ASC = advlas_skyline_c.c
SRCS     = $(SRCS_RS) $(SRCS_RA) $(SRCS_CS) $(SRCS_CA) $(SRCS_ASC)

OBJS_RS  = mymt_aij_rs.o
OBJS_RA  = mymt_aij_ra.o
OBJS_CS  = mymt_aij_cs.o
OBJS_CA  = mymt_aij_ca.o
OBJS_ASC = advlas_skyline_c.o
OBJS     = $(OBJS_RS) $(OBJS_RA) $(OBJS_CS) $(OBJS_CA) $(OBJS_ASC)

LOCAL_LIBFW_DIR = ../../$(LIBFW_DIR)

INCLUDES = $(INCLUDE_ADV) -I./ -I$(LOCAL_LIBFW_DIR)


all:$(TARGET)
$(SOLVER):$(OBJS)
	$(AR) $(ARFLAGS) $(SOLVER) $(OBJS)

rs:$(OBJS_RS)
	$(AR) $(ARFLAGS) $(SOLVER_RS) $(OBJS_RS)
ra:$(OBJS_RA)
	$(AR) $(ARFLAGS) $(SOLVER_RA) $(OBJS_RA)
cs:$(OBJS_CS)
	$(AR) $(ARFLAGS) $(SOLVER_CS) $(OBJS_CS)
ca:$(OBJS_CA)
	$(AR) $(ARFLAGS) $(SOLVER_CA) $(OBJS_CA)
c:$(OBJS_ASC)
	$(AR) $(ARFLAGS) $(SOLVER_ASC) $(OBJS_ASC)

$(SOLVER_RS):$(OBJS_RS)
	$(AR) $(ARFLAGS) $(SOLVER_RS) $(OBJS_RS)
$(SOLVER_RA):$(OBJS_RA)
	$(AR) $(ARFLAGS) $(SOLVER_RA) $(OBJS_RA)
$(SOLVER_CS):$(OBJS_CS)
	$(AR) $(ARFLAGS) $(SOLVER_CS) $(OBJS_CS)
$(SOLVER_CA):$(OBJS_CA)
	$(AR) $(ARFLAGS) $(SOLVER_CA) $(OBJS_CA)
$(SOLVER_ASC):$(OBJS_ASC)
	$(AR) $(ARFLAGS) $(SOLVER_ASC) $(OBJS_ASC)

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -D$(OPERATING_SYSTEM) -c $<
clean:
	rm -f $(OBJS) $(SOLVER) $(SOLVER_RS) $(SOLVER_RA) $(SOLVER_CS) $(SOLVER_CA) $(SOLVER_ASC)
