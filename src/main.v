import db
import filepath
import os
import vargs

const (
	LCHARMAP_VERSION = 0.2.0
	ESCAPE = 0x1B
)

struct Options {
	format_long bool
}

fn main() {
	// load database
	cachedir := os.cachedir()
	dbpath   := filepath.join(cachedir, "lcharmap/", "chars.db")
	chardb := db.new(dbpath)

	// set default options
	opts := Options {
		format_long: false
	}
}
