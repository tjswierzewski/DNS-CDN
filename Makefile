
dnsserver: libJSON
	$(MAKE) -C DNS -f DNS.mk

httpserver:
	$(MAKE) -C HTTP -f HTTP.mk

libJSON:
	$(MAKE) -C JSON -f JSON.mk

clean:
	$(MAKE) -C DNS -f DNS.mk clean
	$(MAKE) -C HTTP -f HTTP.mk clean
	$(MAKE) -C JSON -f JSON.mk clean