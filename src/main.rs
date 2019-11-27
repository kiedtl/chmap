use std;
use std::env;
use std::char;
use std::process;
use std::vec::Vec;

use terminal_size::{ Width, Height, terminal_size };

const LCHARMAP_VERSION:    &str  = "0.1.0";

const DEFAULT_RANGE_START: usize = 0;
const DEFAULT_RANGE_STOP:  usize = 255;

// escape char; needed for formatting
const ESCAPE:              char  = 0x1B as char;

fn main() {
    // parse arguments
    let range: [usize; 2] = args();
    print_rows(&range);
    process::exit(0);
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
        println!("err!: unable to convert '{:X}' to type char", code);
        return char::from_u32(32 as u32).unwrap();
    }
}

fn print_rows(range: &[usize; 2]) {
    if range[1] < range[0] {
        println!("err: range {} -> {} doesn't make sense.", range[0], range[1]);
        process::exit(1);
    }
    
    if range[1] == range[0] {
        print_entry_long(range[0]);
        process::exit(0);
    } else {
        print_line(term_width());
        println!("DEC\tHEX\tOCT\tHTML\tCHAR");
        print_line(term_width());
        for c in range[0]..range[1] {
            print_entry_short(c);
        }
    }
}

fn print_entry_short(entry: usize) {
    println!("{}\t{}\t{}\t{}\t{}",
             &format!("{}", entry),
             &format!("{:X}", entry),
             &format!("{:o}", entry),
             &format!("&#{:x};", entry),
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
    print_entry_row("HTML entity", &format!("&#{:x};", entry));

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

fn args() -> [usize; 2] {
    let args = env::args().collect::<Vec<String>>();
    let mut range: [usize; 2] = [0, 0];
    
    // print usage
    // TODO: use better lib (structopt/getopt)
    if args.len() == 1 || (args.len() > 1 && (args[1] == "--help" || args[1] == "-h")) {
        usage();
        process::exit(1);
    }
   
    // print version
    // TODO: use better lib (structopt/getopt)
    if args.len() > 1 && (args[1] == "--version" || args[1] == "-V") {
        println!("lcharmap {}", LCHARMAP_VERSION);
        process::exit(0);
    }

    // get range beginning
    if args.len() < 2 {
        return [0, 255];
    }

    let arg1_res = args[1].parse::<usize>();
    match arg1_res {
        Ok(val) => range[0] = val,
        Err(_er) => {
            let val = args[1].chars().collect::<Vec<char>>()[0] as u32 as usize;
            return [val, val];
        },
    }

    // get range end
    if args.len() < 3 {
        range[1] = range[0];
        return range;
    }
    let arg2_res = args[2].parse::<usize>();
    match arg2_res {
        Ok(val) => range[1] = val,
        Err(_er) => {
            println!("warn: '{}' is not a valid integer. defaulting to '{}'.", args[2], range[0]);
            range[1] = range[0];
        },
    }
    
    // return range
    range
}   

fn usage() {
    println!("lcharmap {}
MIT (c) Kied Llaentenn
USAGE:
\tlcharmap [RANGE BEGIN] [RANGE END]
\tlcharmap [CHAR]

EXAMPLES:
\tlcharmap 0 25
\tlcharmap 255
\tlcharmap 128 255
\tlcharmap A
\tlcharmap
", LCHARMAP_VERSION);
}
