/*
 * upddb: update lcharmap character database (chars.db)
 * (c) Kiëd Llaentenn
 * see the LICENSE for more information.
*/

import os

fn main() {
	lines := os.read_lines("./unicode.txt")?

	for line in lines {
		data := line.split(";")
		mut desc := data[1]

		if desc == "<control>" {
			desc = data[11]
		}

		print(desc)

		for c := 0; c < (86 - desc.len); c++ {
			print(rune(0))
		}
	}
}
