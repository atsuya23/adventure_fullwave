# *************************************************************************
#  Copyright (C) 2000, 2001, 2002 Shinobu Yoshimura,
#                The University of Tokyo,
#                the Japan Society for the Promotion of Science (JSPS)
#  Copyright (C) 2003, 2005, 2007, 2014, 2015, 2016, 2017, 2018, 2019
#                ADVENTURE Project,
#  All Rights Reserved
# *************************************************************************

SRC_DIR = ../


include $(SRC_DIR)Makefile.in


default:template


template:
	(if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi)
	(if [ ! -d $(INSTALL_DOCDIR) ]; then mkdir -p $(INSTALL_DOCDIR); fi)
	(if [ ! -d $(INSTALL_DOCFWDIR) ]; then mkdir -p $(INSTALL_DOCFWDIR); fi)
	(cp -f $(MANUAL_JP) $(INSTALL_DOCFWDIR))

