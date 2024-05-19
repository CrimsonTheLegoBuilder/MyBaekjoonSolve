//use std::io;
use std::cmp::min;

fn read_line_as_number() -> i32 {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    input.trim().parse().unwrap()
}

fn main() {
    let (a, b, c) = (read_line_as_number(), read_line_as_number(), read_line_as_number());
    let A = a * 4 + b * 2;
    let B = a * 2 + c * 2;
    let C = b * 2 + c * 4;
    let mut ret = min(A, B);
    ret = min(ret, C);
    //println!("{}", ret);
}