// all bounds checking/regex validation
// should occur in the main module.
// if something goes wrong here we just panic.

module db

import sqlite
import regex

struct charmap {
	path string
	db   sqlite.DB
}

pub fn new(fpath string) charmap {
	if !os.file_exists(fpath) {
		panic("lcharmap: unable to open character database at '$fpath'.")
	}
	
	return DB {
		path: fpath,
		db:   sqlite.connect(fpath)
	}
}

pub fn (self charmap) get_desc(ch u32) string {
	// TODO: panic if:
	//    - codepoint > 32841
	//    - return code is error
	return self.db.q_string("SELECT description FROM map WHERE id=$ch")
}

pub fn (self charmap) search(re regex.RE) []u32 {
	matches := []u32([])

	rows := self.db.exec("SELECT description FROM map")
	for i, row in rows {
		start, end := re.match_string(row[0])
		if start >= 0 && end > start {
			matches << i
		}
	}

	return matches
}
