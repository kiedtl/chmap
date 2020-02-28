module terminfo

// TODO:
//	- move terminfo.c into submodule
//	- use submodule instead of terminfo.c,
//		which can then be maintained
//		separately.
#include "terminfo.c"

fn C.ttywidth()  u16
fn C.ttyheight() u16

pub struct Termdata {
pub:
	width  u16
	height u16
}

pub fn retrieve() Termdata {
	return Termdata {
		width:  C.ttywidth()
		height: C.ttyheight()
	}
}
