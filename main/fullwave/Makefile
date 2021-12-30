# ********************************************************************
#  Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
#  the Japan Society for the Promotion of Science (JSPS)
#  All Rights Reserved
# ********************************************************************

include ../../Makefile.in


TARGET = $(HDDM_FULL_WAVE_S) $(HDDM_FULL_WAVE_P) $(HDDM_FULL_WAVE_H)

SRCS   = main.c options.c fullwave.c
OBJS   = main.o options.o fullwave.o

SRCS_S = $(SRCS)
SRCS_P = $(SRCS)
SRCS_H = $(SRCS)

OBJS_S = $(OBJS)
OBJS_P = $(OBJS)
OBJS_H = $(OBJS)

LOCAL_LIBHDDM_DIR = ../../$(LIBHDDM_DIR)
LOCAL_LIBHDDM_S   = $(LOCAL_LIBHDDM_DIR)/$(LIBHDDM_S)
LOCAL_LIBHDDM_P   = $(LOCAL_LIBHDDM_DIR)/$(LIBHDDM_P)
LOCAL_LIBHDDM_H   = $(LOCAL_LIBHDDM_DIR)/$(LIBHDDM_H)

LOCAL_LIBFEM_DIR = ../../$(LIBFEM_DIR)
LOCAL_LIBFEM     = $(LOCAL_LIBFEM_DIR)/$(LIBFEM)

LOCAL_SOLVER_DIR = ../../$(SOLVER_DIR)
LOCAL_SOLVER     = $(LOCAL_SOLVER_DIR)/$(SOLVER)

LOCAL_LIBFW_DIR  = ../../$(LIBFW_DIR)
LOCAL_LIBFW_S    = $(LOCAL_LIBFW_DIR)/$(LIBFW_S)
LOCAL_LIBFW_P    = $(LOCAL_LIBFW_DIR)/$(LIBFW_P)
LOCAL_LIBFW_H    = $(LOCAL_LIBFW_DIR)/$(LIBFW_H)


INCLUDES = $(INCLUDE_ADV) -I./ -I$(LOCAL_LIBHDDM_DIR) -I$(LOCAL_LIBFEM_DIR) -I$(LOCAL_SOLVER_DIR) -I$(LOCAL_LIBFW_DIR)

LIBLARIES_S =  $(LOCAL_LIBHDDM_S) $(LOCAL_LIBFEM) $(LOCAL_SOLVER) $(LOCAL_LIBFW_S) $(LIBS)
LIBLARIES_P =  $(LOCAL_LIBHDDM_P) $(LOCAL_LIBFEM) $(LOCAL_SOLVER) $(LOCAL_LIBFW_P) $(LIBS)
LIBLARIES_H =  $(LOCAL_LIBHDDM_H) $(LOCAL_LIBFEM) $(LOCAL_SOLVER) $(LOCAL_LIBFW_H) $(LIBS)


all:$(TARGET)
$(HDDM_FULL_WAVE_S):$(SRCS_S)
	rm -f $(OBJS_S) $(HDDM_FULL_WAVE_S)
	make single CC="$(CC)" LINKER="$(LINKER)" MFLAGS="$(CFLAGS) -D$(MODE_SINGLE)"
$(HDDM_FULL_WAVE_P):$(SRCS_P)
	rm -f $(OBJS_P) $(HDDM_FULL_WAVE_P)
	make parallel CC="$(MPI_CC)" LINKER="$(MPI_LINKER)" MFLAGS="$(CFLAGS) -D$(MODE_PARALLEL)"
$(HDDM_FULL_WAVE_H):$(SRCS_H)
	rm -f $(OBJS_H) $(HDDM_FULL_WAVE_H)
	make hierarchical CC="$(MPI_CC)" LINKER="$(MPI_LINKER)" MFLAGS="$(CFLAGS) -D$(MODE_HIERARCHICAL)"

single:$(OBJS_S)
	(if [ ! -f $(LOCAL_LIBHDDM_S) ]; then cd $(LOCAL_LIBHDDM_DIR) && make $(LIBHDDM_S); fi)
	(if [ ! -f $(LOCAL_LIBFEM) ]; then cd $(LOCAL_LIBFEM_DIR) && make $(LIBFEM); fi)
	(if [ ! -f $(LOCAL_SOLVER) ]; then cd $(LOCAL_SOLVER_DIR) && make $(SOLVER); fi)
	(if [ ! -f $(LOCAL_LIBFW_S) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBFW_S); fi)
	$(LINKER) $(OBJS) -o $(HDDM_FULL_WAVE_S) $(LIBLARIES_S)

parallel:$(OBJS_P)
	(if [ ! -f $(LOCAL_LIBHDDM_P) ]; then cd $(LOCAL_LIBHDDM_DIR) && make $(LIBHDDM_P); fi)
	(if [ ! -f $(LOCAL_LIBFEM) ]; then cd $(LOCAL_LIBFEM_DIR) && make $(LIBFEM); fi)
	(if [ ! -f $(LOCAL_SOLVER) ]; then cd $(LOCAL_SOLVER_DIR) && make $(SOLVER); fi)
	(if [ ! -f $(LOCAL_LIBFW_P) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBFW_P); fi)
	$(LINKER) $(OBJS) -o $(HDDM_FULL_WAVE_P) $(LIBLARIES_P)

hierarchical:$(OBJS_H)
	(if [ ! -f $(LOCAL_LIBHDDM_H) ]; then cd $(LOCAL_LIBHDDM_DIR) && make $(LIBHDDM_H); fi)
	(if [ ! -f $(LOCAL_LIBFEM) ]; then cd $(LOCAL_LIBFEM_DIR) && make $(LIBFEM); fi)
	(if [ ! -f $(LOCAL_SOLVER) ]; then cd $(LOCAL_SOLVER_DIR) && make $(SOLVER); fi)
	(if [ ! -f $(LOCAL_LIBFW_H) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBFW_H); fi)
	$(LINKER) $(OBJS) -o $(HDDM_FULL_WAVE_H) $(LIBLARIES_H)


.c.o:
	$(CC) $(INCLUDES) $(MFLAGS) -c $<


clean:
	rm -f $(OBJS_S) $(OBJS_P) $(OBJS_H) $(TARGET)
#	(cd $(LOCAL_LIBHDDM_DIR) ; make clean )
#	(cd $(LOCAL_LIBFEM_DIR) ; make clean )
#	(cd $(LOCAL_SOLVER_DIR) ; make clean )
#	(cd $(LOCAL_LIBFW_DIR) ; make clean )
