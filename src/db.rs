use std::io;
use std::io::Read;
use std::io::Seek;
use std::fs::File;
use std::result::Result;

pub struct DB {
    path: Option<String>,
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
    pub fn load(&mut self, path: String) -> Result<(), std::io::Error> {
        if ! std::fs::metadata(&*path).is_ok() {
            panic!("lcharmap: db.new(): chardb path not OK.");
        }
        
        // initialize file pointer
        let file = File::open(&*path)?;
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
    // TODO: implement
    //pub fn search_desc() {}
}

