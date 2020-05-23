use std::thread;

fn main() {
    let threads = 8;
    let mut handles = Vec::new();

    for _ in 0..threads {
        handles.push(thread::spawn(move || {
            let mut easy = curl::easy::Easy::new();
            easy.url("http://localhost:8080").unwrap();
            easy.perform().unwrap();
        }));
    }

    for handle in handles  {
        handle.join().unwrap();
    }
}
