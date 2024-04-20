use std::io;

fn read_line_as_number() -> i32 {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    input.trim().parse().unwrap()
}

fn main() {
    let mut input = String::new();//empty string
    
    io::stdin().read_line(&mut input).expect("Error!");
    let val = input.trim().split_whitespace()
        .map(|x| x.parse::<i32>().unwrap())
        .collect::<Vec<_>>();
    if val[0] + val[1] == val[2] {println!("correct!")} else {println!("wrong!")}
}
