#[macro_use] extern crate lalrpop_util;

lalrpop_mod!(pub parser);

fn main()
{
    println!("Hello, world!");
    let parser = parser::TermParser::new();
    println!("{}", parser.parse("(((((15)))))").unwrap());
}
