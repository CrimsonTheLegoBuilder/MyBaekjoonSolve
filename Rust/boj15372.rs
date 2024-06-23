fn read_line_as_number() -> i64 {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    input.trim().parse().unwrap()
}

fn main() {
    let mut t: i64 = read_line_as_number();
    while t > 0 {
        let a: i64 = read_line_as_number();
        //println!("{}", a * a);
        t -= 1;
    }
}