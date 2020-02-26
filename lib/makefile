all: chars.db

clean:
	rm -f upddb chars.db chars.db-journal

upddb:
	v build -prod upddb.v

chars.db: upddb unicode.txt
	./upddb

unicode.txt:
	@echo Oops, looks like unicode.txt is missing.
	@echo You'll have to retrieve it before you can
	@echo build lcharmap.
	@false
