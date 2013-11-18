.PHONY: clean stage

stage:
	@mkdir keys
	@chmod 700 keys
	python keyGen.py
	python repoGen.py

clean:
	rm -rf *~ tuf.log keys
