#[allow(unused_imports)]
use std::cmp::{max, min};
use std::io::{stdin, stdout, BufWriter, Write};

fn main() {
    let stdin_obj = stdin();
    let mut scan = Scanner {
        stdin: stdin_obj.lock(),
    };
    let out = &mut BufWriter::new(stdout());

    let test_cases: usize = scan.read();
    for t in 1..test_cases + 1 {
        let n: usize = scan.read();
        let mut x: Vec<i64> = scan.read_vec(n);
        let mut y: Vec<i64> = scan.read_vec(n);
        x.sort_unstable();
        y.sort_unstable_by(|a, b| b.cmp(a));
        let mut ans: i64 = 0;
        for (x, y) in x.iter().zip(y.iter()) {
            ans += x * y;
        }
        writeln!(out, "Case #{}: {}", t, ans).ok();
    }
}

#[derive(Default)]
pub struct Scanner<R> {
    stdin: R,
}
impl<R: std::io::Read> Scanner<R> {
    pub fn read<T: std::str::FromStr>(&mut self) -> T {
        use std::io::Read;
        let buf = self
            .stdin
            .by_ref()
            .bytes()
            .map(|b| b.unwrap())
            .skip_while(|&b| b == b' ' || b == b'\n' || b == b'\r')
            .take_while(|&b| b != b' ' && b != b'\n' && b != b'\r')
            .collect::<Vec<_>>();
        unsafe { std::str::from_utf8_unchecked(&buf) }
            .parse()
            .ok()
            .expect("Parse error.")
    }
    pub fn read_line(&mut self) -> String {
        use std::io::Read;
        let buf = self
            .stdin
            .by_ref()
            .bytes()
            .map(|b| b.unwrap())
            .skip_while(|&b| b == b'\n' || b == b'\r')
            .take_while(|&b| b != b'\n' && b != b'\r')
            .collect::<Vec<_>>();
        unsafe { std::str::from_utf8_unchecked(&buf) }
            .parse()
            .ok()
            .expect("Parse error.")
    }
    pub fn read_vec<T: std::str::FromStr>(&mut self, n: usize) -> Vec<T> {
        (0..n).map(|_| self.read()).collect()
    }
    pub fn read_chars(&mut self) -> Vec<char> {
        self.read::<String>().chars().collect()
    }
}
