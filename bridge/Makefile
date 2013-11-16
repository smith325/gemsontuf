.PHONY: all tuf rubygems bridge setup msg env

msg:
	@echo "** Welcome to Gems on TUF installer **\n"
	@echo "1) To install necessary environment type:"
	@echo "\tmake env\n"
	@echo "2) To download and install TUF type:"
	@echo "\tmake tuf\n"
	@echo "3) To download the rubygems source using git type:"
	@echo "\tmake rubygems\n"
	@echo "4) To compile the Ruby to TUF bridge C library type:"
	@echo "\tmake bridge\n"
	@echo "5) To install Gems on TUF to /tmp dir, type:"
	@echo "\tmake setup\n"
	@echo "Thank you for trying Gems on TUF"

all: tuf rubygems bridge setup

env:
	sudo apt-get install python python-dev curl git ruby rubygems

tuf:
	pip install tuf

rubygems:
	git clone https://github.com/rubygems/rubygems.git

bridge:
	cd interface; ruby extconf.rb; make; cp GemsOnTuf.so ..; make clean; rm Makefile; cd ..

setup:
	@echo "Overwriting remote_fetcher.rb inside installation directory"
	cp remote_fetcher.rb rubygems/lib/rubygems
	@echo "Running gem setup"
	cd rubygems; ruby setup.rb; cd ..
	@echo "Creating .gemtuf directory in /tmp directory"
	mkdir -p /tmp/.gemtuf
	@echo "Saving gem_tuf.json and GemsOnTuf.so inside /tmp/.gemtuf directory"
	cp gem_tuf.json GemsOnTuf.so /tmp/.gemtuf
	@echo "Saving ./client directory inside /tmp/.gemtuf directory"
	cp -r client /tmp/.gemtuf

clean:
	rm -rf rubygems
	rm -f tuf.log GemsOnTuf.so
	rm -rf /tmp/.gemtuf

