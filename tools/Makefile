# ********************************************************************
#  Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
#  the Japan Society for the Promotion of Science (JSPS)
#  All Rights Reserved
# ********************************************************************

include ../Makefile.in


LOCAL_TOOLS_MAKEFEM  = $(MAKEFEM)/$(TOOLS_MAKEFEM)
LOCAL_TOOLS_MAKEUCD  = $(MAKEUCD)/$(TOOLS_MAKEUCD)
LOCAL_TOOLS_MAKEPML  = $(MAKEPML)/$(TOOLS_MAKEPML)

default:
	(cd $(MAKEFEM) ; make )
	(cd $(MAKEUCD) ; make )
	(cd $(MAKEPML) ; make )

single:
	(cd $(MAKEFEM) ; make single )
	(cd $(MAKEUCD) ; make single )
	(cd $(MAKEPML) ; make single )			

parallel:

hierarchical:


install:
	(if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi)
	(if [ ! -d $(INSTALL_BINDIR) ]; then mkdir -p $(INSTALL_BINDIR); fi)
	(if [ ! -f $(LOCAL_TOOLS_MAKEFEM) ]; then cd $(MAKEFEM) && make; fi)
	(cp -f $(LOCAL_TOOLS_MAKEFEM) $(INSTALL_BINDIR))
	(if [ ! -f $(LOCAL_TOOLS_MAKEUCD) ]; then cd $(MAKEUCD) && make; fi)
	(cp -f $(LOCAL_TOOLS_MAKEUCD) $(INSTALL_BINDIR))
	(if [ ! -f $(LOCAL_TOOLS_MAKEPML) ]; then cd $(MAKEPML) && make; fi)
	(cp -f $(LOCAL_TOOLS_MAKEPML) $(INSTALL_BINDIR))


install-s:
	(if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi)
	(if [ ! -d $(INSTALL_BINDIR) ]; then mkdir -p $(INSTALL_BINDIR); fi)
	(if [ ! -f $(LOCAL_TOOLS_MAKEFEM) ]; then cd $(MAKEFEM) && make single; fi)
	(cp -f $(LOCAL_TOOLS_MAKEFEM) $(INSTALL_BINDIR))
	(if [ ! -f $(LOCAL_TOOLS_MAKEUCD) ]; then cd $(MAKEUCD) && make single; fi)
	(cp -f $(LOCAL_TOOLS_MAKEUCD) $(INSTALL_BINDIR))
	(if [ ! -f $(LOCAL_TOOLS_MAKEPML) ]; then cd $(MAKEPML) && make single; fi)
	(cp -f $(LOCAL_TOOLS_MAKEPML) $(INSTALL_BINDIR))



install-p:
	(if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi)
	(if [ ! -d $(INSTALL_BINDIR) ]; then mkdir -p $(INSTALL_BINDIR); fi)

install-h:
	(if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi)
	(if [ ! -d $(INSTALL_BINDIR) ]; then mkdir -p $(INSTALL_BINDIR); fi)


clean:
	(cd $(MAKEFEM) ; make clean )
	(cd $(MAKEUCD) ; make clean )
	(cd $(MAKEPML) ; make clean )
