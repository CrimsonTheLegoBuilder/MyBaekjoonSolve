use std::io;

fn main() {
    let mut s = String::new();

    io::stdin().read_line(&mut s).unwrap();

    let values:Vec<i32> = s
        .as_mut_str()
        .split_whitespace()
        .map(|s| s.parse().unwrap())
        .collect();

    let a1: i32 = values[0];
    let b1: i32 = values[1];
    let c1: i32 = values[2];

    s.clear();

    io::stdin().read_line(&mut s).unwrap();

    let values:Vec<i32> = s
        .as_mut_str()
        .split_whitespace()
        .map(|s| s.parse().unwrap())
        .collect();

    let a2: i32 = values[0];
    let b2: i32 = values[1];
    let c2: i32 = values[2];

    let mut t:i32 = 0;
    if a1 < a2 { t += a2 - a1 }
    if b1 < b2 { t += b2 - b1 }
    if c1 < c2 { t += c2 - c1 }
    //println!("{}", t);
}