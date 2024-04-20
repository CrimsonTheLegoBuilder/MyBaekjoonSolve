use std::io;

fn read_line_as_number() -> i32 {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    input.trim().parse().unwrap()
}

fn main() {
    let (h, m) = (read_line_as_number(), read_line_as_number());
    println!("{}", h * 60 + m);
}