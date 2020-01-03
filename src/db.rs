use std::io;
use regex::Regex;
use std::io::Read;
use std::io::Seek;
use std::fs::File;
use std::path::PathBuf;
use std::result::Result;

pub struct DB {
    path: Option<PathBuf>,
    file: Option<File>,
}

impl DB {
    // initialize new database with dummy info
    pub fn new() -> DB {
        DB {
            path: None,
            file: None,
        }
    }
    pub fn load(&mut self, path: PathBuf) -> Result<(), std::io::Error> {
        if ! std::fs::metadata(&path).is_ok() {
            panic!("lcharmap: db.new(): chardb path not OK.");
        }
        
        // initialize file pointer
        let file = File::open(&path)?;
        self.path = Some(path);
        self.file = Some(file);
        Ok(())
    }
    // read specific line corresponding to codepoint
    // e.g. with codepoint 0 read line 0 of db file
    pub fn get_desc(&mut self, codepoint: usize) -> Result<String, std::io::Error> {
        let mut entry = 0;
        let mut cursor = 0;
        let mut file: File = self.file.take().unwrap();

        // move cursor to beginning of file
        file.seek(io::SeekFrom::Start(0))?;

        // move cursor to beginning of specified line
        while entry < codepoint {
            cursor = file.seek(io::SeekFrom::Start(cursor + 85)).unwrap();
            entry += 1;
        }
        // read 85 bytes, which is the maximum description length
        let mut buff = [0; 85]; // allocate bytes for description
        let mut char_buff = [0; 1];
        file.read(&mut char_buff)?;
        let mut ctr = 0;
        while char_buff[0] as usize != 0 {
            buff[ctr] = char_buff[0];   // copy char buffer onto main buffer
            ctr = ctr + 1;
            cursor = file.seek(io::SeekFrom::Start(cursor + 1)).unwrap();
            file.read(&mut char_buff)?;
        }
        
        // <hacky>
        self.file = Some(file);
        // convert buffer to string and return
        Ok(std::str::from_utf8(&buff).unwrap().to_string())
    }
    // return vector of codepoints which match the regex provided
    pub fn search(&mut self, regex: Regex) -> Result<Vec<usize>, std::io::Error> {
        // load entire description database into vector
        let mut matches = Vec::new();
        let mut descs = Vec::new();
        let mut file: File = self.file.take().unwrap();
        let mut readct = 1;

        // move cursor to beginning of file
        file.seek(io::SeekFrom::Start(0)).unwrap();

        // read until EOF
        while readct != 0 {
            let mut buffer = [0; 85];
            readct = file.read(&mut buffer)?;
            if readct != 0 {
                descs.push(std::str::from_utf8(&buffer).unwrap().to_string());
            }
        }
        
        // iterate over each description, looking for matches
        let mut ctr = 0;
        for desc in descs {
            if regex.is_match(&*desc) {
                matches.push(ctr);
            }
            ctr = ctr + 1;
        }

        // <hacky>
        self.file = Some(file);

        Ok(matches)
    }
}

