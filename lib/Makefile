# ********************************************************************
#  Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
#  the Japan Society for the Promotion of Science (JSPS)
#  All Rights Reserved
# ********************************************************************

include ../Makefile.in


LOCAL_LIBHDDM_DIR = ../$(LIBHDDM_DIR)
LOCAL_LIBHDDM     = $(LOCAL_LIBHDDM_DIR)/$(LIBHDDM)
LOCAL_LIBHDDM_S   = $(LOCAL_LIBHDDM_DIR)/$(LIBHDDM_S)
LOCAL_LIBHDDM_P   = $(LOCAL_LIBHDDM_DIR)/$(LIBHDDM_P)
LOCAL_LIBHDDM_H   = $(LOCAL_LIBHDDM_DIR)/$(LIBHDDM_H)

LOCAL_LIBFEM_DIR  = ../$(LIBFEM_DIR)
LOCAL_LIBFEM      = $(LOCAL_LIBFEM_DIR)/$(LIBFEM)

LOCAL_SOLVER_DIR  = ../$(SOLVER_DIR)
LOCAL_SOLVER      = $(LOCAL_SOLVER_DIR)/$(SOLVER)

LOCAL_LIBFW_DIR  = ../$(LIBFW_DIR)
LOCAL_LIBFW_S    = $(LOCAL_LIBFW_DIR)/$(LIBFW_S)
LOCAL_LIBFW_P    = $(LOCAL_LIBFW_DIR)/$(LIBFW_P)
LOCAL_LIBFW_H    = $(LOCAL_LIBFW_DIR)/$(LIBFW_H)


default:
	(cd $(LOCAL_LIBHDDM_DIR) ; make )
	(cd $(LOCAL_LIBFEM_DIR) ; make )
	(cd $(LOCAL_SOLVER_DIR) ; make )
	(cd $(LOCAL_LIBFW_DIR) ; make )

single:
	(cd $(LOCAL_LIBHDDM_DIR) ; make $(LIBHDDM_S) )
	(cd $(LOCAL_LIBFEM_DIR) ; make )
	(cd $(LOCAL_SOLVER_DIR) ; make )
	(cd $(LOCAL_LIBFW_DIR) ; make $(LIBFW_S) )

parallel:
	(cd $(LOCAL_LIBHDDM_DIR) ; make $(LIBHDDM_P) )
	(cd $(LOCAL_LIBFEM_DIR) ; make )
	(cd $(LOCAL_SOLVER_DIR) ; make )
	(cd $(LOCAL_LIBFW_DIR) ; make $(LIBFW_P) )

hierarchical:
	(cd $(LOCAL_LIBHDDM_DIR) ; make $(LIBHDDM_H) )
	(cd $(LOCAL_LIBFEM_DIR) ; make )
	(cd $(LOCAL_SOLVER_DIR) ; make )
	(cd $(LOCAL_LIBFW_DIR) ; make $(LIBFW_H) )


install:
	(if [ ! -f $(LOCAL_LIBHDDM_S) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBHDDM_S); fi)
	(if [ ! -f $(LOCAL_LIBHDDM_P) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBHDDM_P); fi)
	(if [ ! -f $(LOCAL_LIBHDDM_H) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBHDDM_H); fi)
	(if [ ! -f $(LOCAL_LIBFEM) ]; then cd $(LOCAL_LIBFEM_DIR) && make $(LIBFEM); fi)
	(if [ ! -f $(LOCAL_SOLVER) ]; then cd $(LOCAL_SOLVER_DIR) && make $(SOLVER); fi)
	(if [ ! -f $(LOCAL_LIBFW_S) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBFW_S); fi)
	(if [ ! -f $(LOCAL_LIBFW_P) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBFW_P); fi)
	(if [ ! -f $(LOCAL_LIBFW_H) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBFW_H); fi)

install-s:
	(if [ ! -f $(LOCAL_LIBHDDM_S) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBHDDM_S); fi)
	(if [ ! -f $(LOCAL_LIBFEM) ]; then cd $(LOCAL_LIBFEM_DIR) && make $(LIBFEM); fi)
	(if [ ! -f $(LOCAL_SOLVER) ]; then cd $(LOCAL_SOLVER_DIR) && make $(SOLVER); fi)
	(if [ ! -f $(LOCAL_LIBFW_S) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBFW_S); fi)

install-p:
	(if [ ! -f $(LOCAL_LIBHDDM_P) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBHDDM_P); fi)
	(if [ ! -f $(LOCAL_LIBFEM) ]; then cd $(LOCAL_LIBFEM_DIR) && make $(LIBFEM); fi)
	(if [ ! -f $(LOCAL_SOLVER) ]; then cd $(LOCAL_SOLVER_DIR) && make $(SOLVER); fi)
	(if [ ! -f $(LOCAL_LIBFW_P) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBFW_P); fi)

install-h:
	(if [ ! -f $(LOCAL_LIBHDDM_H) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBHDDM_H); fi)
	(if [ ! -f $(LOCAL_LIBFEM) ]; then cd $(LOCAL_LIBFEM_DIR) && make $(LIBFEM); fi)
	(if [ ! -f $(LOCAL_SOLVER) ]; then cd $(LOCAL_SOLVER_DIR) && make $(SOLVER); fi)
	(if [ ! -f $(LOCAL_LIBFW_H) ]; then cd $(LOCAL_LIBFW_DIR) && make $(LIBFW_H); fi)


clean:
	(cd $(LOCAL_LIBHDDM_DIR) ; make clean )
	(cd $(LOCAL_LIBFEM_DIR) ; make clean )
	(cd $(LOCAL_SOLVER_DIR) ; make clean )
	(cd $(LOCAL_LIBFW_DIR) ; make clean )
