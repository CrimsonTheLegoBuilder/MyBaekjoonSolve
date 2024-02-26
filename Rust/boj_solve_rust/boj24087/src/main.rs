fn read_line_as_number() -> i32 {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    input.trim().parse().unwrap()
}

fn main() {
    let (s, a, b) = (read_line_as_number(), read_line_as_number(), read_line_as_number());
    let ans: i32 = 250;
    let shr = (s - a) / b;
    let rem = (s - a) % b;
    let f = if rem == 0 {0} else {1};
    if shr <= 0 {println!("{}", ans)} else {println!("{}", ans + (shr + f) * 100)}
}
