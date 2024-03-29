MAIN_MANS = pdns_server.1 \
	pdns_control.1 \
	pdnsutil.1 \
	zone2json.1 \
	zone2sql.1

MANPAGES_INSTALL = $(MAIN_MANS)

MANPAGES_DIST = $(MAIN_MANS) zone2ldap.1

if LDAP
MANPAGES_INSTALL += zone2ldap.1
endif

MANPAGES_TARGET_TOOLS = calidns.1 \
	dnsgram.1 \
	dnsreplay.1 \
	dnsscan.1 \
	dnsscope.1 \
	dnswasher.1 \
	dumresp.1 \
	ixplore.1 \
	nproxy.1 \
	nsec3dig.1 \
	pdns_notify.1 \
	saxfr.1 \
	sdig.1

MANPAGES_DIST += $(MANPAGES_TARGET_TOOLS) \
	dnsbulktest.1 \
	dnstcpbench.1 \
	dnspcap2protobuf.1

if HAVE_BOOST_GE_148
MANPAGES_INSTALL += dnsbulktest.1 \
	dnstcpbench.1
endif

if HAVE_PROTOBUF
if HAVE_PROTOC
MANPAGES_INSTALL += dnspcap2protobuf.1
endif
endif

if TOOLS
MANPAGES_INSTALL += $(MANPAGES_TARGET_TOOLS)
endif

man_MANS = $(MANPAGES_INSTALL)

EXTRA_DIST = $(MANPAGES_DIST)

if HAVE_VIRTUALENV
if !HAVE_MANPAGES
$(MANPAGES_DIST): %: manpages/%.rst .venv
	.venv/bin/python -msphinx -b man . mans $<
	mv mans/$@ $@
endif # if !HAVE_MANPAGES

.venv: requirements.txt
	virtualenv .venv
	.venv/bin/pip install -U pip setuptools setuptools-git
	.venv/bin/pip install -r requirements.txt

html-docs: common/** manpages/** .venv *.rst
	.venv/bin/python -msphinx -b html . html-docs

latex/PowerDNS-Authoritative.pdf: common/** manpages/** .venv *.rst
	.venv/bin/python -msphinx -M latexpdf . .

PowerDNS-Authoritative.pdf: latex/PowerDNS-Authoritative.pdf
	mv $< $@

html-docs.tar.bz2: html-docs
	tar cjf $@ $<

all-docs: PowerDNS-Authoritative.pdf html-docs html-docs.tar.bz2

upload-docs: all-docs
	rsync -crv --delete --no-p --chmod=g=rwX --exclude '*~' ./html-docs/ web1.powerdns.com:/srv/www/doc.powerdns.com/authoritative/
	rsync -crv --no-p --chmod=g=rwX --exclude '*~' ./html-docs.tar.bz2 web1.powerdns.com:/srv/www/doc.powerdns.com/authoritative/
	rsync -crv --no-p --chmod=g=rwX --exclude '*~' ./PowerDNS-Authoritative.pdf web1.powerdns.com:/srv/www/doc.powerdns.com/authoritative/

else # if HAVE_VIRTUALENV
$(MANPAGES_DIST):
	echo "You need virtualenv to generate the manpages"
	exit 1

PowerDNS-Authoritative.pdf:
	echo "You need virtualenv to generate the PDF"
	exit 1

html-docs:
	echo "You need virtualenv to generate the HTML docs"
	exit 1
endif


