#[allow(unused_imports)]
use std::cmp::{max, min, Eq, Ord, Ordering, PartialOrd};
use std::collections::{HashMap, HashSet};
use std::io::{stdin, stdout, BufWriter, Write};

fn main() {
    let stdin_obj = stdin();
    let mut scan = Scanner {
        stdin: stdin_obj.lock(),
    };
    let out = &mut BufWriter::new(stdout());

    let test_cases: usize = scan.read();
    'test_case_loop: for t in 1..test_cases + 1 {
        let n: usize = scan.read();
        let m: usize = scan.read();
        let mut customers: Vec<HashMap<usize, usize>> = Vec::new();

        for _ in 0..m {
            let cnt: usize = scan.read();
            let mut flavor: Option<usize> = None;
            let mut malted: Option<usize>;
            let mut requests: HashMap<usize, usize> = HashMap::new();

            for j in 0..2 * cnt {
                if j % 2 == 0 {
                    flavor = Some(scan.read());
                    flavor = Some(flavor.unwrap() - 1); // decrement 1 for indexing
                } else {
                    malted = Some(scan.read());
                    requests.insert(flavor.unwrap(), malted.unwrap());
                }
            }
            customers.push(requests);
        }

        let comparator =
            |a: &HashMap<usize, usize>, b: &HashMap<usize, usize>| match a.len().cmp(&b.len()) {
                Ordering::Less => Ordering::Less,
                Ordering::Equal => b.values().next().unwrap().cmp(a.values().next().unwrap()),
                Ordering::Greater => Ordering::Greater,
            };

        let mut flavors_to_malt: HashSet<usize> = HashSet::new();

        customers.sort_unstable_by(comparator);
        while !customers.is_empty()
            && customers[0].len() == 1
            && customers[0].values().next().unwrap() == &1
        {
            // eprintln!("{:?}", customers);
            let mut customers_to_remove: HashSet<usize> = HashSet::new();
            let flavor_to_malt = customers[0].keys().next().unwrap().clone();
            flavors_to_malt.insert(flavor_to_malt);
            customers_to_remove.insert(0);

            let current_len = customers.len();

            for (customer, index) in customers.iter_mut().zip(0..current_len) {
                if !customer.contains_key(&flavor_to_malt) {
                    continue;
                }
                if customer.get(&flavor_to_malt).unwrap() == &0 {
                    customer.remove(&flavor_to_malt);
                    if customer.len() == 0 {
                        write!(out, "Case #{}:", t).ok();
                        writeln!(out, " IMPOSSIBLE").ok();
                        continue 'test_case_loop;
                    }
                } else {
                    customers_to_remove.insert(index);
                }
            }
            let mut index = 0;
            customers.retain(|_| !customers_to_remove.contains(&(index, index += 1).0));
            customers.sort_unstable_by(comparator);
        }

        let result: Vec<i32> = (0..n)
            .map(|x| flavors_to_malt.contains(&x) as i32)
            .collect();
        write!(out, "Case #{}:", t).ok();
        for x in result.iter() {
            write!(out, " {}", x).ok();
        }
        writeln!(out).ok();
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
