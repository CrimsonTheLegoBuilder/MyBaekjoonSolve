use std::f64::consts::PI;

fn read_line_as_number() -> i64 {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    input.trim().parse().unwrap()
}

fn main() {
    let d1: i64 = read_line_as_number();
    let d2: i64 = read_line_as_number();
    let d3: f64 = d1 as f64 * 2.0;
    let d4: f64 = d2 as f64 * 2.0 * 3.141592;
    //println!("{:.6}", d3 + d4);
}