use std::f64::consts::PI;

fn read_line_as_number() -> i64 {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    input.trim().parse().unwrap()
}

fn main() {
    let a: i64 = read_line_as_number();
    let b: f64 = a as f64 / PI;
    let c = b.sqrt();
    //println!("{}", c * 2.0 * PI);
}