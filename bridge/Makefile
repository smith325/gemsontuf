.PHONY: all prereqs tuf rubygems bridge setup

all: prereqs setup

prereqs: tuf rubygems bridge


tuf:
	pip install tuf

rubygems:
	git clone https://github.com/rubygems/rubygems.git

bridge:
	cd interface; ruby extconf.rb; make; cp GemsOnTuf.so ..; make clean; rm Makefile; cd ..

setup:
	@echo "Overwriging remote_fetcher.rb inside installation directory"
	cp remote_fetcher.rb rubygems/lib/rubygems
	sed -i.bak 's/HOME/\/home\/$(USER)/g' rubygems/lib/rubygems/remote_fetcher.rb
	@echo "Running gem setup"
	cd rubygems; ruby setup.rb; cd ..
	@echo "Creating .gemtuf directory in ~/ directory"
	mkdir -p ~/.gemtuf
	@echo "Saving gem_tuf.json and GemsOnTuf.so inside ~/.gemtuf directory"
	cp gem_tuf.json GemsOnTuf.so ~/.gemtuf
	sed -i.bak 's/HOME/\/home\/$(USER)/g' ~/.gemtuf/gem_tuf.json
	@echo "Saving ./client directory inside ~/.gemtuf directory"
	cp -r client ~/.gemtuf

clean:
	rm -rf rubygems
	rm -f tuf.log GemsOnTuf.so
	rm -rf ~/.gemtuf

