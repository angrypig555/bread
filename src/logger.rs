use std::{fs::create_dir_all, path::PathBuf};
use chrono::{Datetime, Local, Utc};

fn log_init() {
    let path = PathBuf::from("/tmp/bread/")

    let log_file = path.join("rusttp.log");
    let curr_time = Local::now();
    let log_header = format!("rusttp v1.0 - logs - {}\n", curr_time);
    if !path.exists() {
        create_dir_all(path);
    }
    if !log_file.exists() {
        fs::write(&log_file, &log_header);
    }
    if log_file.exists() {
        fs::remove_file(&log_file);
        fs::write(&log_file, &log_header);
    }
    LOG_PATH.set(log_file);
}

fn log(msg: &str) {
    if !LOGGING_ENABLED.load(Ordering::Relaxed) {
        return;
    }
    let curr_time = Local::now();
    let message = format!("{:?} - {}: {}\n", thread::current().id(), curr_time, msg);
    if let Some(path) = LOG_PATH.get() {
        let file = OpenOptions::new().create(true).append(true).open(path);
        if let Ok(mut file) = file {
            let _ = file.write_all(message.as_bytes());
        }
    }
    

}