# ********************************************************************
#  Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
#  the Japan Society for the Promotion of Science (JSPS)
#  All Rights Reserved
# ********************************************************************

include ../../Makefile.in


TARGET = $(TOOLS_MAKEFEM)

SRCS   = main.c options.c
OBJS   = main.o options.o

INCLUDES = $(INCLUDE_ADV) -I./ -I$(LOCAL_LIBFW_DIR)

LOCAL_LIBFW_DIR = ../../$(LIBFW_DIR)
LOCAL_LIBFW_S   = $(LOCAL_LIBFW_DIR)/$(LIBFW_S)

LIBLARIES =  $(LOCAL_LIBFW_S) $(LIBS)

MFLAGS   = $(CFLAGS) -D$(MODE_SINGLE)


all:$(TARGET)
$(TARGET):$(OBJS)
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
#	(cd $(LOCAL_LIBFW_DIR) ; make clean )
