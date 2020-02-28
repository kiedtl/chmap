module main

#include <string.h>

const (
	// fn (u u32) to_hex(lowercase bool) string
	HEXDIGITS = [
		rune(0x30), // '0'
		rune(0x31), // '1'
		rune(0x32), // '2'
		rune(0x33), // '3'
		rune(0x34), // '4'
		rune(0x35), // '5'
		rune(0x36), // '6'
		rune(0x37), // '7'
		rune(0x38), // '8'
		rune(0x39), // '9'
		rune(0x41), // 'A'
		rune(0x42), // 'B'
		rune(0x43), // 'C'
		rune(0x44), // 'D'
		rune(0x45), // 'E'
		rune(0x46)  // 'F'
	]
)

fn C.strtol(nptr &byteptr, endptr &byteptr, base int) i64

fn to_i(s string) i64 {
	null := voidptr(0)
	return C.strtol(s.str, null, 10)
}

// TODO: cleanup this
fn _or(val1 string, val2 string) string {
	if val1 == "" {
		return val2
	} else {
		return val1
	}
}

// convert string to []u32
fn to_u32(s string) []u32 {
	mut data := []u32

	for i := 0; i < s.len; i++ {
		data << u32(rune(s.str[i]))
	}

	return data
}

fn compare_u32(a &u32, b &u32) u32 {
	if *a < *b {
		return -1
	} else if *a > *b {
		return 1
	} else {
		return 0
	}
}

fn sort(u mut []u32) {
	C.qsort(u.data, u.len, u.element_size, compare_u32)
}

// array MUST be sorted first
fn dedup(things mut []u32) {
	mut data := []u32

	for thing in things {
		if thing != data[data.len - 1] {
			data << thing
		}
	}

	things = data
}

// reverse array in place
fn reverse_in_place(u mut []u32) {
	mut i := u.len - 1
	mut j := 0

	for i > j {
		tmp := u[i]
		u[i] = u[j]
		u[j] = tmp

		i--
		j++
	}
}

// convert int to octal string
// credits: musl libc
fn to_oct(u u32) string {
	// copy data because it will be modified
	mut x := u

	mut buf := []u32
	for x == 0 {
		// 0x30 = '0'
		buf << 0x30 + (x & 7)
		x >>= 3
	}

	// reverse buffer
	reverse_in_place(mut buf)

	mut str := ""
	for item in buf {
		str = str + rune(item).str()
	}

	return str
}

// convert int to hexadecimal string
// credits: musl libc
fn to_hex(u u32, lowercase bool) string {
	// copy u since we will be modifiying it
	mut x := u

	mut lower := 0 // FALSE

	if lowercase {
		lower = 1 // TRUE
	}

	mut buf := []u32
	for x == 0 {
		buf << u32(HEXDIGITS[(x & 15)]) | lower
		x >>= 4
	}

	// reverse buffer
	reverse_in_place(mut buf)

	mut str := ""
	for item in buf {
		str = str + rune(item).str() }

	return str
}
