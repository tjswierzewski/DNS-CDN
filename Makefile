
dnsserver:
	$(MAKE) -C DNS -f DNS.mk

clean:
	$(MAKE) -C DNS -f DNS.mk clean
	rm dnsserver