extern crate getopts;
extern crate terminal_size;
extern crate regex;

use std;
use std::env;
use std::char;
use regex::Regex;
use std::process;
use std::vec::Vec;

use getopts::Options;
use terminal_size::{ Width, Height, terminal_size };
use directories::ProjectDirs;

// database for character descriptions
// database file are in the lib directory.
// database (/etc/chars.db) is generated via
// lib/upddb.c. You may build chars.db with `just run`
// in the lib dir.
mod db;
use crate::db::DB;

const LCHARMAP_VERSION: &'static str = env!("CARGO_PKG_VERSION");

//const DEFAULT_RANGE_START: usize = 0;
//const DEFAULT_RANGE_STOP:  usize = 255;

// escape char; needed for formatting
const ESCAPE:              char    = 0x1B as char;

fn main() {
    // load database
    let mut db = DB::new();
    let proj_dir = ProjectDirs::from("", "", "lcharmap").unwrap_or_else(|| {
        eprintln!("cannot determine location of chars.db");
        process::exit(1);
    });
    let db_path = proj_dir.data_dir().join("chars.db");
    let result = db.load(db_path);
    match result {
        Ok(__) => (),
        Err(_) => {
            println!("lcharmap: err!: fatal error whilst loading chardb!");
            panic!("Check previous error.");
        },
    }

    // show in long format?
    let mut show_long = false;
    
    // parse arguments via getopt
    let args: Vec<String> = env::args().collect::<Vec<String>>();

    let mut opts = Options::new();
    opts.optflag("l", "long", "Print character entries in long format");
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

    if matches.opt_present("l") {
        show_long = true;
    }

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
                                        Err(_) => {
                                            println!("lcharmap: '{}' as range doesn't make sense.", i);
                                            process::exit(1);
                                        },
                                    }
                             }).collect::<Vec<usize>>();
        print_rows(&mut db, &range, show_long);
    }

    if matches.opt_present("c") {
        // here, we basically do the exact same thing as with range, only
        // we turn each char into a u32 before processing it.

        // convert to ints
        let chars_str = match matches.opt_str("c") {
            Some(c) => c,
            None => format!("{}", to_char(0)),   // null character default
        };

        // sort and dedup
        let mut chars = chars_str.chars().map(|c| c as u32 as usize).collect::<Vec<usize>>();
        chars.sort();
        chars.dedup();

        if chars.len() > 1  && ! show_long {
            print_line(term_width());
            println!("DEC\tHEX\tOCT\tHTML\tCHAR\tDESC");
            print_line(term_width());
        }

        let _ = chars.iter().map(|c| {
            if chars.len() < 2 || show_long {
                print_entry_long(*c, db.get_desc(*c).unwrap());
                print!("\n");
            } else {
                print_entry_short(*c, db.get_desc(*c).unwrap());
            }
        }).collect::<()>();
    }

    // searching
    if matches.opt_present("s") {
        let regex;
        let term = match matches.opt_str("s") {
            Some(t) => t,
            None => {
                println!("lcharmap: err!: no search term provided!");
                process::exit(1);
            },
        };

        let regex_res = Regex::new(&*term);
        match regex_res {
            Ok(rgx) => regex = rgx,
            Err(__) => {
                println!("lcharmap: err!: invalid regex: {}", __);
                process::exit(1);
            },
        }
       
        let mut matches;
        let result = db.search(regex);
        match result {
            Ok(ms) => matches = ms,
            Err(e) => {
                println!("lcharmap: err!: error whilst searching: {}", e);
                process::exit(1);
            },
        }

        // print out matchesf
        matches.sort();
        matches.dedup();
        
        if matches.len() > 1 && ! show_long {
            print_line(term_width());
            println!("DEC\tHEX\tOCT\tHTML\tCHAR\tDESC");
            print_line(term_width());
        }

        let _ = matches.iter().map(|m| {
            if matches.len() < 2 || show_long {
                print_entry_long(*m, db.get_desc(*m).unwrap());
                print!("\n");
            } else {
                print_entry_short(*m, db.get_desc(*m).unwrap());
            }
        }).collect::<()>();
    }

    if matches.opt_present("h") {
        usage(&opts);
    }

    if matches.opt_present("V") {
        version();
    }
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
        println!("lcharmap: '{}' is not a valid character.", code);
        process::exit(1);
    }
    if let Some(ch) = char::from_u32(code as u32) {
        // only if printable.
        // if not printable, causes weird things to happen
        if code < 32 {
            return ' ';
        } else {
            return ch;
        }
    } else {
        return char::from_u32(32 as u32).unwrap();
    }
}

fn print_rows(db: &mut DB, range: &Vec<usize>, long: bool) {
    if range.len() < 2 {
        print_entry_long(range[0], db.get_desc(range[0]).unwrap()); // don't panic on single item ranges (such as `--range 0`)
        return;
    }

    if range[1] < range[0] {
        println!("lcharmap: range {} -> {} doesn't make sense.", range[0], range[1]);
        return;
    }

    if range[1] == range[0] {
        print_entry_long(range[0], db.get_desc(range[0]).unwrap());
        return;
    } else {
        if long {
            for x in range[0]..range[1] {
                print_entry_long(x, db.get_desc(x).unwrap());
                print!("\n");
            }
        } else {
            print_line(term_width());
            println!("DEC\tHEX\tOCT\tHTML\tCHAR\tDESC");
            print_line(term_width());
            for c in range[0]..range[1]+1 {
                print_entry_short(c, db.get_desc(c).unwrap());
            }
        }
    }
}

fn print_entry_short(entry: usize, description: String) {
    println!("{}\t{}\t{}\t{}\t{}\t{}",
             &format!("{}", entry),
             &format!("{:X}", entry),
             &format!("{:o}", entry),
             &format!("&#{};", entry),
             &format!("{}", to_char(entry)),
             &format!("{}", description));
    print_line(term_width());
}

fn print_line(width: usize) {
    for _ in 0..width {
        print!("─");
    }
}

fn print_entry_long(entry: usize, description: String) {
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

    // description
    print_entry_row("description", &*description);
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


