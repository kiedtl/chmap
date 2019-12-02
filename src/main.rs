extern crate getopts;
extern crate terminal_size;

use std;
use std::env;
use std::char;
use std::process;
use std::vec::Vec;

use getopts::Options;
use terminal_size::{ Width, Height, terminal_size };

const LCHARMAP_VERSION:    &str  = "0.1.0";

//const DEFAULT_RANGE_START: usize = 0;
//const DEFAULT_RANGE_STOP:  usize = 255;

// escape char; needed for formatting
const ESCAPE:              char  = 0x1B as char;

fn main() {
    // parse arguments via getopt
    let args: Vec<String> = env::args().collect::<Vec<String>>();
    let program = args[0].clone();

    let mut opts = Options::new();
    opts.optflag("h", "help", "Print this help message.");
    opts.optflag("V", "version", "Print version and exit.");
    opts.optopt("r", "range", "Print a range of Unicode codepoints. (e.g., `128,255')", "RANGE");
    opts.optopt("c", "chars", "Print a range of Unicode codepoints that match provided character(s). (e.g., `xyz')",
                "CHARS");
    opts.optopt("s", "search", "Search character descriptions for *term*.", "TERM");

    let matches = match opts.parse(&args[1..]) {
        Ok(m) => m,
        Err(e) => {
            println!("{}", e);
            process::exit(1);
        },
    };

    // range
    if matches.opt_present("r") {
        let range_str = match matches.opt_str("r") {
            Some(r) => r,
            None => "0,128".to_owned(),    // default range (ASCII characters)
        };

        // parse range into [usize, 2]
        let range = range_str.split(",")
                             .map(|i| {
                                    let tmp = i.parse::<usize>();
                                        match tmp {
                                            Ok(ni) => ni,
                                                Err(e) => {
                                                    println!("err!: '{}' as range doesn't make sense.", i);
                                                    process::exit(1);
                                                },
                                            }
                             }).collect::<Vec<usize>>();
        print_rows(&range);
    }

    if matches.opt_present("c") {
        // here, we basically do the exact same thing as with range, only
        // we turn each char into a u32 before processing it.

        // convert to ints
        let chars_str = match matches.opt_str("c") {
            Some(c) => c,
            None => format!("{}", to_char(0)),   // null character default
        };

        print_line(term_width());
        println!("DEC\tHEX\tOCT\tHTML\tCHAR");
        print_line(term_width());

        // sort and dedup
        let mut chars = chars_str.chars().collect::<Vec<char>>();
        chars.sort();
        chars.dedup();

        chars.iter().map(|c| { print_entry_short(*c as u32 as usize); *c }).collect::<Vec<char>>();
    }

    if matches.opt_present("h") {
        usage(&opts);
    }

    if matches.opt_present("V") {
        version();
    }

    // TODO: search (--search, -s)
}

fn term_width() -> usize {
    let termsize = terminal_size();
    if let Some((Width(w), Height(_g))) = termsize {
        return w as usize;
    } else {
        return 80;
    }
}

fn to_char(code: usize) -> char {
    if code > char::MAX as u32 as usize {
        println!("err!: '{}' is not a valid character.", code);
        process::exit(1);
    }
    if let Some(ch) = char::from_u32(code as u32) {
        return ch;
    } else {
        return char::from_u32(32 as u32).unwrap();
    }
}

fn print_rows(range: &Vec<usize>) {
    if range[1] < range[0] {
        println!("err!: range {} -> {} doesn't make sense.", range[0], range[1]);
        process::exit(1);
    }

    if range[1] == range[0] {
        print_entry_long(range[0]);
        process::exit(0);
    } else {
        print_line(term_width());
        println!("DEC\tHEX\tOCT\tHTML\tCHAR");
        print_line(term_width());
        for c in range[0]..range[1]+1 {
            print_entry_short(c);
        }
    }
}

fn print_entry_short(entry: usize) {
    println!("{}\t{}\t{}\t{}\t{}",
             &format!("{}", entry),
             &format!("{:X}", entry),
             &format!("{:o}", entry),
             &format!("&#{};", entry),
             &format!("{}", to_char(entry)));
    print_line(term_width());
}

fn print_line(width: usize) {
    for _ in 0..width {
        print!("─");
    }
}

fn print_entry_long(entry: usize) {
    // print decimal
    print_entry_row("decimal", &format!("{}", entry));

    // print hexadecimal
    print_entry_row("hexadecimal", &format!("{:X}", entry));

    // print octal
    print_entry_row("octal", &format!("{:o}", entry));

    // print HTML code
    print_entry_row("HTML entity", &format!("&#{};", entry));

    // print char
    print_entry_row("character", &format!("{}", to_char(entry)));
}

fn print_entry_row(key: &str, value: &str) {
    let spc = 20;
    for _ in 0..spc-key.len() {
        print!(" ");
    }
    print!("{}[1m{}{}[0m", ESCAPE, key, ESCAPE);
    println!("  {}", value);
}

fn version() {
    println!("lcharmap {}", LCHARMAP_VERSION);
}

fn usage(opts: &Options) {
    let usage = format!("lcharmap {}\n\nUsage:\n    lcharmap [OPTIONS] [ARGS]", LCHARMAP_VERSION);
    print!("{}", opts.usage(&*usage));
}


