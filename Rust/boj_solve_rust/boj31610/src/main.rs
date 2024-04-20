use std::io;

fn read_line_as_number() -> i32 {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    input.trim().parse().unwrap()
}

fn main() {
    let (a, b, c) = (read_line_as_number(), read_line_as_number(), read_line_as_number());
    let ans = a * b + c;
    println!("{}", ans);
}