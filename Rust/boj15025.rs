use std::io;
use std::cmp::max;

fn main() {
    let mut s = String::new();

    io::stdin().read_line(&mut s).unwrap();

    let values:Vec<i32> = s
        .as_mut_str()
        .split_whitespace()
        .map(|s| s.parse().unwrap())
        .collect();

    let a: i32 = values[0];
    let b: i32 = values[1];
    let c: bool = a == b;
    let d: i32 = max(a, b) * 2;

    // if a == 0 && b == 0 {
    //     println!("Not a moose");
    // }
    // else if c {
    //     println!("Even {}", d);
    // }
    // else {println!("Odd {}", d);}
}