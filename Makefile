# ********************************************************************
#  Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
#  the Japan Society for the Promotion of Science (JSPS)
#  All Rights Reserved
# ********************************************************************

include ./Makefile.in


default:
	(cd $(LIB_DIR) ; make )
	(cd $(HDDM_DIR) ; make )
	(cd $(TOOLS_DIR) ; make )

single:
	(cd $(LIB_DIR) ; make single )
	(cd $(HDDM_DIR) ; make single )
	(cd $(TOOLS_DIR) ; make single )

parallel:
	(cd $(LIB_DIR) ; make parallel )
	(cd $(HDDM_DIR) ; make parallel )
	(cd $(TOOLS_DIR) ; make parallel )

hierarchical:
	(cd $(LIB_DIR) ; make hierarchical )
	(cd $(HDDM_DIR) ; make hierarchical )
	(cd $(TOOLS_DIR) ; make hierarchical )


install:
	(if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi)
	(if [ ! -d $(INSTALL_BINDIR) ]; then mkdir -p $(INSTALL_BINDIR); fi)
	(if [ ! -d $(INSTALL_DOCDIR) ]; then mkdir -p $(INSTALL_DOCDIR); fi)
	(if [ ! -d $(INSTALL_DOCFWDIR) ]; then mkdir -p $(INSTALL_DOCFWDIR); fi)
	(cp -f $(DOC_DIR)/* $(INSTALL_DOCFWDIR))
	(cd $(LIB_DIR) ; make install )
	(cd $(HDDM_DIR) ; make install )
	(cd $(TOOLS_DIR) ; make install )

install-s:
	(if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi)
	(if [ ! -d $(INSTALL_BINDIR) ]; then mkdir -p $(INSTALL_BINDIR); fi)
	(if [ ! -d $(INSTALL_DOCDIR) ]; then mkdir -p $(INSTALL_DOCDIR); fi)
	(if [ ! -d $(INSTALL_DOCFWDIR) ]; then mkdir -p $(INSTALL_DOCFWDIR); fi)
	(cp -f $(DOC_DIR)/* $(INSTALL_DOCFWDIR))
	(cd $(LIB_DIR) ; make install-s )
	(cd $(HDDM_DIR) ; make install-s )
	(cd $(TOOLS_DIR) ; make install-s )

install-p:
	(if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi)
	(if [ ! -d $(INSTALL_BINDIR) ]; then mkdir -p $(INSTALL_BINDIR); fi)
	(if [ ! -d $(INSTALL_DOCDIR) ]; then mkdir -p $(INSTALL_DOCDIR); fi)
	(if [ ! -d $(INSTALL_DOCFWDIR) ]; then mkdir -p $(INSTALL_DOCFWDIR); fi)
	(cp -f $(DOC_DIR)/* $(INSTALL_DOCFWDIR))
	(cd $(LIB_DIR) ; make install-p )
	(cd $(HDDM_DIR) ; make install-p )
	(cd $(TOOLS_DIR) ; make install-p )

install-h:
	(if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi)
	(if [ ! -d $(INSTALL_BINDIR) ]; then mkdir -p $(INSTALL_BINDIR); fi)
	(if [ ! -d $(INSTALL_DOCDIR) ]; then mkdir -p $(INSTALL_DOCDIR); fi)
	(if [ ! -d $(INSTALL_DOCFWDIR) ]; then mkdir -p $(INSTALL_DOCFWDIR); fi)
	(cp -f $(DOC_DIR)/* $(INSTALL_DOCFWDIR))
	(cd $(LIB_DIR) ; make install-h )
	(cd $(HDDM_DIR) ; make install-h )
	(cd $(TOOLS_DIR) ; make install-h )


clean:
	(cd $(LIB_DIR) ; make clean )
	(cd $(HDDM_DIR) ; make clean )
	(cd $(TOOLS_DIR) ; make clean )
