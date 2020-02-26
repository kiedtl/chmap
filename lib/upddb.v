/*
 * upddb: update lcharmap character database (chars.db)
 * (c) Kiëd Llaentenn
 * see the LICENSE for more information.
*/

import os
import sqlite

fn main() {
	lines := os.read_lines("./unicode.txt")?
	len := lines.len
	db := sqlite.connect("./chars.db")

	// we're assuming that the makefile has already
	// purged the file
	db.exec_none("CREATE TABLE map (id int, description varchar(255));")

	for i, line in lines {
		print("\rstoring description for character $i/$len")

		data := line.split(";")
		mut desc := data[1]

		// the descriptions for control characters
		// aren't really descriptions
		if i < 32 {
			desc = data[10]
		}

		// descriptions in unicode.txt are uppercase
		desc = desc.to_lower()

		db.exec_none("INSERT INTO map VALUES ($i, '$desc');")
	}

	print("\n")
}
