all:
	$(MAKE) -C common
	$(MAKE) -C corebroker_oldcpp
	$(MAKE) -C simpleserver
	$(MAKE) -C exampleclient  
	$(MAKE) -C canrouter
	$(MAKE) -C corebroker
	
clean:
	$(MAKE) clean -C common
	$(MAKE) clean -C corebroker_oldcpp
	$(MAKE) clean -C simpleserver
	$(MAKE) clean -C exampleclient  
	$(MAKE) clean -C canrouter
	$(MAKE) clean -C corebroker
