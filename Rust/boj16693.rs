use std::io;
use std::f32::consts::PI;

fn main() {
    let mut s = String::new();

    io::stdin().read_line(&mut s).unwrap();

    let values:Vec<f32> = s
        .as_mut_str()
        .split_whitespace()
        .map(|s| s.parse().unwrap())
        .collect();

    let a1: f32 = values[0];
    let p1: f32 = values[1];

    s.clear();

    io::stdin().read_line(&mut s).unwrap();

    let values:Vec<f32> = s
        .as_mut_str()
        .split_whitespace()
        .map(|s| s.parse().unwrap())
        .collect();

    let r1: f32 = values[0];
    let p2: f32 = values[1];
    let f1: f32 = a1 / p1;
    let f2: f32 = r1 * r1 * PI / p2;
    //if f1 > f2 { println!("Slice of pizza"); }
    //else { println!("Whole pizza"); }
}