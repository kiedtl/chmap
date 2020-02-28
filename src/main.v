module main

import db
import filepath
import os
import regex
import terminfo
import vargs

const (
	LCHARMAP_VERSION = "0.3.0"
	ESCAPE = 0x1B
	LONG_FORMAT_PADDING = 20
)

struct Options {
mut:
	format_long bool
	ttywidth    u16
}

fn main() {
	// load database
	cachedir := os.cachedir()
	dbpath   := filepath.join(cachedir, "lcharmap/", "chars.db")
	chardb := db.new(dbpath)

	// set default options
	mut opts := Options {
		format_long: false
		ttywidth:    terminfo.retrieve().width
	}

	// parse arguments with nedpals/vargs
	args := vargs.parse(os.args, 1)
	if 'l' in args.options || 'long' in args.options {
		opts.format_long = true
	}

	// --- help ---
	if 'h' in args.options || 'help' in args.options {
		usage()
	}

	// -v instead of -V is unofficial, will be removed when
	// we need -v for something else
	if 'V' in args.options || 'v' in args.options || 'version' in args.options {
		version()
	}

	// ¯\_(ツ)_/¯
	if 'rage' in args.options {
		println("Grrr")
		exit(0)
	}

	// --- range ---
	if 'r' in args.options || 'range' in args.options {
		mut range     := []u32
		mut range_str := _or(args.options['r'], args.options['range'])

		if range_str == "" {
			eprintln("lcharmap: error: argument to --range missing.")
			exit(1)
		}

		// parse range into []u32
		range_data := range_str.split(",")
		range << to_i(range_data[0])
		
		if range_data.len > 1 {
			range << to_i(range_data[1])
		}

		print_rows(chardb, range, opts)
	}

	// --- characters ---
	if 'c' in args.options || 'chars' in args.options {
		// same as range, but we turn each character
		// into a u32
		mut characters := _or(args.options['c'], args.options['chars'])

		if characters == "" {
			eprintln("lcharmap: error: argument to --chars missing.")
			exit(1)
		}

		// only show each character once
		mut chars := to_u32(characters)
		sort(mut chars)
		chars = dedup(chars)

		if chars.len > 1 && !opts.format_long {
			print_line(opts.ttywidth)
			println("DEC\tHEX\tOCT\tHTML\tCHAR\tDESC")
			print_line(opts.ttywidth)
		}

		for _char in chars {
			if chars.len < 2 || opts.format_long {
				print_entry_long(_char, chardb.get_desc(_char))
				print("\n")
			} else {
				print_entry_short(_char, chardb.get_desc(_char), opts)
			}
		}
	}

	// --- search ---
	if 's' in args.options || 'search' in args.options {
		query := _or(args.options['s'], args.options['search'])

		if query == "" {
			eprintln("lcharmap: error: argument to --search missing.")
			exit(1)
		}

		mut re := regex.new_regex()
		re_err, err_pos := re.compile(query)
		
		if re_err != regex.COMPILE_OK {
			error := re.get_parse_error_string(re_err)
			println("lcharmap: regex error: $error")
			println("                query: $query")
			lc := "-".repeat(err_pos - 1)
			println("                error: $lc^")
			exit(1)
		}

		mut matches := chardb.search(mut re)
		sort(mut matches)
		matches = dedup(matches)

		if matches.len > 1 && !opts.format_long {
			print_line(opts.ttywidth)
			println("DEC\tHEX\tOCT\tHTML\tCHAR\tDESC")
			print_line(opts.ttywidth)
		}

		for _match in matches {
			if matches.len < 2 || opts.format_long {
				print_entry_long(_match, chardb.get_desc(_match))
				print("\n")
			} else {
				print_entry_short(_match, chardb.get_desc(_match), opts)
			}
		}
	}
}

fn print_rows(chardb db.Charmap, range []u32, opts Options) {
	if range.len < 2 {
		// don't panic on single item ranges (e.g. `--range 0`)
		print_entry_long(range[0], chardb.get_desc(range[0]))
		return
	}

	if range[1] < range[0] {
		eprintln("lcharmap: error: range ${range[0]} -> ${range[1]} doesn't make sense.")
		return
	}

	if range[1] == range[0] {
		print_entry_long(range[0], chardb.get_desc(range[0]))
		return
	}

	if opts.format_long {
		for i := range[0]; i <= range[1]; i++ {
			print_entry_long(i, chardb.get_desc(i))
			print("\n")
		}
	} else {
		print_line(opts.ttywidth)
		println("DEC\tHEX\tOCT\tHTML\tCHAR\tDESC")
		print_line(opts.ttywidth)

		for i := range[0]; i <= range[1]; i++ {
			print_entry_short(i, chardb.get_desc(i), opts)
		}
	}
}

fn print_entry_short(entry u32, description string, opts Options) {
	hex := to_hex(entry, false)
	oct := to_oct(entry)
	htm := "&#" + entry.str() + ";"
	cha := rune(entry)

	println("${entry}\t${hex}\t${oct}\t${htm}\t${cha}\t${description}")
	print_line(opts.ttywidth)
}

fn print_line(width u16) {
	line := "─".repeat(int(width))
	print(line)
}

fn print_entry_long(entry u32, description string) {
	hex := to_hex(entry, false)
	oct := to_oct(entry)
	htm := "&#" + entry.str() + ";"
	cha := rune(entry).str()

	print_entry_row("decimal", entry.str())
	print_entry_row("hexadecimal", hex)
	print_entry_row("octal", oct)
	print_entry_row("HTML entity", htm)
	print_entry_row("character", cha)
	print_entry_row("description", description)
}

fn print_entry_row(key string, val string) {
	print(" ".repeat(LONG_FORMAT_PADDING))
	print("$ESCAPE[1m$key$ESCAPE[0m")
	print("  $val\n")
}

fn version() {
	println("lcharmap ${LCHARMAP_VERSION}")
	exit(0)
}

fn usage() {
	print("Usage: lcharmap [OPTIONS] [ARGS]\n")
	print("Print and search information on the provided Unicode characters.\n\n")
	print("OPTIONS:\n")
	print("    -l, --long          print character entries in the long format.\n")
	print("    -h, --help          print this help message and exit.\n")
	print("    -V, --version       print version and exit.\n\n")
	print("ARGUMENTS:\n")
	print("    -r, --range RANGE   print a range of Unicode characters.\n")
	print("    -c, --chars CHARS   print a range of Unicode codepoints that match\n")
	print("                        provided character(s).\n")
	print("    -s, --search TERM   search character descriptions for TERM.\n\n")
	print("EXAMPLES:\n")
	print("    lcharmap -r 0,128   print information for characters from 0 to 128.\n")
	print("    lcharmap -c abcde   print information for characters 'a', 'b', 'c',\n")
	print("                        'd', and 'e'.\n")
	print("    lcharmap -s latin   search characters for description 'latin'.\n")
	print("                        regular expressions are supported.\n\n") // TODO: remove?
	print("Report issues to https://github.com/lptstr/lcharmap.\n")
	print("Full documentation is available locally at lcharmap(1).\n")
	exit(0)
}
