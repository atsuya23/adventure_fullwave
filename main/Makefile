# ********************************************************************
#  Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
#  the Japan Society for the Promotion of Science (JSPS)
#  All Rights Reserved
# ********************************************************************

include ../Makefile.in


LOCAL_HDDM_FULL_WAVE_S = $(FULL_WAVE)/$(HDDM_FULL_WAVE_S)
LOCAL_HDDM_FULL_WAVE_P = $(FULL_WAVE)/$(HDDM_FULL_WAVE_P)
LOCAL_HDDM_FULL_WAVE_H = $(FULL_WAVE)/$(HDDM_FULL_WAVE_H)


default:
	(cd $(FULL_WAVE) ; make )

single:
	(cd $(FULL_WAVE) ; make single )

parallel:
	(cd $(FULL_WAVE) ; make parallel )

hierarchical:
	(cd $(FULL_WAVE) ; make hierarchical )


install:
	(if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi)
	(if [ ! -d $(INSTALL_BINDIR) ]; then mkdir -p $(INSTALL_BINDIR); fi)
	(if [ ! -f $(LOCAL_HDDM_FULL_WAVE_S) ]; then cd $(FULL_WAVE) && make single; fi)
	(cp -f $(LOCAL_HDDM_FULL_WAVE_S) $(INSTALL_BINDIR))
	(if [ ! -f $(LOCAL_HDDM_FULL_WAVE_P) ]; then cd $(FULL_WAVE) && make parallel; fi)
	(cp -f $(LOCAL_HDDM_FULL_WAVE_P) $(INSTALL_BINDIR))
	(if [ ! -f $(LOCAL_HDDM_FULL_WAVE_H) ]; then cd $(FULL_WAVE) && make hierarchical; fi)
	(cp -f $(LOCAL_HDDM_FULL_WAVE_H) $(INSTALL_BINDIR))

install-s:
	(if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi)
	(if [ ! -d $(INSTALL_BINDIR) ]; then mkdir -p $(INSTALL_BINDIR); fi)
	(if [ ! -f $(LOCAL_HDDM_FULL_WAVE_S) ]; then cd $(FULL_WAVE) && make single; fi)
	(cp -f $(LOCAL_HDDM_FULL_WAVE_S) $(INSTALL_BINDIR))

install-p:
	(if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi)
	(if [ ! -d $(INSTALL_BINDIR) ]; then mkdir -p $(INSTALL_BINDIR); fi)
	(if [ ! -f $(LOCAL_HDDM_FULL_WAVE_P) ]; then cd $(FULL_WAVE) && make parallel; fi)
	(cp -f $(LOCAL_HDDM_FULL_WAVE_P) $(INSTALL_BINDIR))

install-h:
	(if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi)
	(if [ ! -d $(INSTALL_BINDIR) ]; then mkdir -p $(INSTALL_BINDIR); fi)
	(if [ ! -f $(LOCAL_HDDM_FULL_WAVE_H) ]; then cd $(FULL_WAVE) && make hierarchical; fi)
	(cp -f $(LOCAL_HDDM_FULL_WAVE_H) $(INSTALL_BINDIR))


clean:
	(cd $(FULL_WAVE) ; make clean )
