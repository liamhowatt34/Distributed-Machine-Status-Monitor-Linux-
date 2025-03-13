all:
	$(MAKE) -C Common
	$(MAKE) -C DC
	$(MAKE) -C DR
	$(MAKE) -C DX

clean:
	$(MAKE) -C Common clean
	$(MAKE) -C DC clean
	$(MAKE) -C DR clean
	$(MAKE) -C DX clean
	rm -f ./tmp/*

