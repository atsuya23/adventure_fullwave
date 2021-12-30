# ********************************************************************
#  Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
#  the Japan Society for the Promotion of Science (JSPS)
#  All Rights Reserved
# ********************************************************************

include ../../Makefile.in


TARGET = $(TOOLS_MAKEUCD)

SRCS   = main.c options.c
OBJS   = main.o options.o

LOCAL_LIBFEM_DIR = ../../$(LIBFEM_DIR)
LOCAL_LIBFEM     = $(LOCAL_LIBFEM_DIR)/$(LIBFEM)

LOCAL_SOLVER_DIR = ../../$(SOLVER_DIR)
LOCAL_SOLVER     = $(LOCAL_SOLVER_DIR)/$(SOLVER)

LOCAL_LIBFW_DIR = ../../$(LIBFW_DIR)
LOCAL_LIBFW_S   = $(LOCAL_LIBFW_DIR)/$(LIBFW_S)

INCLUDES = $(INCLUDE_ADV) -I./ -I$(LOCAL_LIBFEM_DIR) -I$(LOCAL_SOLVER_DIR) -I$(LOCAL_LIBFW_DIR)

LIBLARIES =  $(LOCAL_LIBFEM) $(LOCAL_SOLVER) $(LOCAL_LIBFW_S) $(LIBS)

MFLAGS   = $(CFLAGS) -D$(MODE_SINGLE)


all:$(TARGET)
$(TARGET):$(OBJS)
	(if [ ! -f $(LOCAL_LIBFEM) ]; then cd $(LOCAL_LIBFEM_DIR) && make $(LIBFEM); fi)
	(if [ ! -f $(LOCAL_SOLVER) ]; then cd $(LOCAL_SOLVER_DIR) && make $(SOLVER); fi)
	(if [ ! -f $(LOCAL_LIBFW_S) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBFW_S); fi)
	$(LINKER) $(OBJS) -o $(TARGET) $(LIBLARIES)

single:
	make $(TARGET)

parallel:

hierarchical:


.c.o:
	$(CC) $(INCLUDES) $(MFLAGS) -c $<


clean:
	rm -f $(OBJS) $(TARGET)
#	(cd $(LOCAL_LIBFEM_DIR) ; make clean )
#	(cd $(LOCAL_SOLVER_DIR) ; make clean )
#	(cd $(LOCAL_LIBFW_DIR) ; make clean )
