// all bounds checking/regex validation
// should occur in the main module.
// if something goes wrong here we just panic.

module db

import os
import sqlite
import regex

pub struct Charmap {
pub:
	path string
	db   sqlite.DB
}

pub fn new(fpath string) Charmap {
	if !os.file_exists(fpath) {
		panic("lcharmap: unable to open character database at '$fpath'.")
	}
	
	return Charmap {
		path: fpath,
		db:   sqlite.connect(fpath)
	}
}

pub fn (c Charmap) get_desc(ch u32) string {
	// TODO: panic if:
	//    - codepoint > 32841
	//    - return code is error
	return c.db.q_string("SELECT description FROM map WHERE id=$ch")
}

pub fn (c Charmap) search(re mut regex.RE) []u32 {
	mut matches := []u32

	// TODO: panic on error code
	rows, ex := c.db.exec("SELECT description FROM map")
	for i, row in rows {
		start, end := re.match_string(row.vals[0])
		if start >= 0 && end > start {
			matches << i
		}
	}

	return matches
}
