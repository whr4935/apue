
SUBDIRS = helloworld scull

DEBUGSUBDIRS = $(addsuffix .debug, $(SUBDIRS))
RELEASESUBDIRS = $(addsuffix .release, $(SUBDIRS))
CLEANSUBDIRS = $(addsuffix .clean, $(SUBDIRS))
INSTALLSUBDIRS = $(addsuffix .install, $(SUBDIRS))
UNINSTALLSUBDIRS = $(addsuffix .install, $(SUBDIRS))

.PHONY: $(SUBDIRS) $(DEBUGSUBDIRS) $(RELEASESUBDIRS) $(INSTALLSUBDIRS) \
	$(CLEANSUBDIRS) $(UNINSTALLSUBDIRS)

all:	$(SUBDIRS)

$(SUBDIRS):
	@echo
	@echo Making all in subdirectory $@...
	@$(MAKE) -C $@

	
debug:	$(DEBUGSUBDIRS)
	
$(DEBUGSUBDIRS):
	@echo
	@echo Executing make debug in subdirectory $(basename $@)...
	@cd $(basename $@) ; $(MAKE) debug


release:	$(RELEASESUBDIRS)

$(RELEASESUBDIRS):
	@echo
	@echo Executing make release in subdirectory $(basename $@)...
	@cd $(basename $@) ; $(MAKE) release

	
install: $(INSTALLSUBDIRS)

$(INSTALLSUBDIRS):
	@echo
	@echo Executing make install in subdirectory $(basename $@)...
	@cd $(basename $@) ; $(MAKE) install


uninstall: $(UNINSTALLSUBDIRS)

$(UNINSTALLSUBDIRS):
	@echo
	@echo Executing make uninstall in subdirectory $(basename $@)...
	@cd $(basename $@) ; $(MAKE) uninstall
	
	
clean:	$(CLEANSUBDIRS)

$(CLEANSUBDIRS):
	@cd $(basename $@) ; $(MAKE) clean


