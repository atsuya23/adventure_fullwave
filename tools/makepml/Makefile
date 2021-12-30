# ********************************************************************
#  Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
#  the Japan Society for the Promotion of Science (JSPS)
#  All Rights Reserved
# ********************************************************************

include ../../Makefile.in


TARGET = $(TOOLS_MAKEPML)

SRCS   = pml.c common.c io.o setup.c elem_vertex.c elem_edge.c elem_face.c
OBJS   = pml.o common.o io.o setup.o elem_vertex.o elem_edge.o elem_face.o


all:$(TARGET)
$(TARGET):$(OBJS)
#	(if [ ! -f $(LOCAL_LIBFEM) ]; then cd $(LOCAL_LIBFEM_DIR) && make $(LIBFEM); fi)
#	(if [ ! -f $(LOCAL_SOLVER) ]; then cd $(LOCAL_SOLVER_DIR) && make $(SOLVER); fi)
#	(if [ ! -f $(LOCAL_LIBFW_S) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBFW_S); fi)
	$(LINKER) $(OBJS) -lm -o $(TARGET) $(LIBLARIES)

single:
	make $(TARGET)

parallel:

hierarchical:


.c.o:
	$(CC) $(CFLAGS_G) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
#	(cd $(LOCAL_LIBFEM_DIR) ; make clean )
#	(cd $(LOCAL_SOLVER_DIR) ; make clean )
#	(cd $(LOCAL_LIBFW_DIR) ; make clean )
