// src/main.rs
use std::env;
use std::process;
use lab1::StructureManager;


fn main() {
    let args: Vec<String> = env::args().collect();
    
    if args.len() < 2 {
        print_usage();
        process::exit(1);
    }
    
    let mut manager = StructureManager::new();
    
    // Парсинг аргументов
    let mut filename = None;
    let mut query = None;
    
    let mut i = 1;
    while i < args.len() {
        match args[i].as_str() {
            "--file" if i + 1 < args.len() => {
                filename = Some(args[i + 1].clone());
                i += 2;
            }
            "--query" if i + 1 < args.len() => {
                query = Some(args[i + 1].clone());
                i += 2;
            }
            "--help" => {
                print_help();
                return;
            }
            _ => {
                eprintln!("Unknown argument: {}", args[i]);
                print_usage();
                process::exit(1);
            }
        }
    }
    
    // Выполнение команды
    match execute_command(&mut manager, filename, query) {
        Ok(output) => {
            if !output.is_empty() {
                println!("{}", output);
            }
        }
        Err(e) => {
            eprintln!("{}", e);
            process::exit(1);
        }
    }
}

fn execute_command(
    manager: &mut StructureManager,
    filename: Option<String>,
    query: Option<String>
) -> Result<String, String> {
    // Загрузка
    if let Some(ref filename) = filename {
        if std::path::Path::new(filename).exists() {
            manager.load_structures_from_file(filename)?;
        }
        manager.set_filename(filename.clone());
    }
    
    // Выполнение
    if let Some(ref query) = query {
        let result = manager.execute_command(query)?;
        Ok(result)
    } else {
        Err("ERROR 10: Unknown command".to_string())
    }
    
    // Сохранение
    if let Some(ref filename) = filename {
        manager.save_current_structure()?;
    }
    
    Ok("".to_string())
}

fn print_usage() {
    println!("Usage: lab1 --file <filename> --query '<command>'");
    println!("       lab1 --help");
}

fn print_help() {
    println!("Data Structures Manager");
    println!("======================");
    println!();
    println!("Commands:");
    println!("  MCREATE [name]              Create array");
    println!("  MPUSH <value>               Add element to array");
    println!("  MPUSHAT <value> <index>     Insert element at index");
    println!("  MGET <index>                Get element from array");
    println!("  MDEL <index>                Delete element from array");
    println!("  MSET <index> <value>        Set element in array");
    println!("  MLEN                        Get array length");
    println!();
    println!("  FCREATE [name]              Create forward list");
    println!("  FPUSH <value> <mode>        Push to list (0-front, 1-back, 2-after head, 3-before tail)");
    println!("  FDEL <mode>                 Delete from list (0-front, 1-back, 2-after head, 3-before tail)");
    println!("  FSEARCH <value>             Search in list");
    println!("  FGET <index>                Get element at index");
    println!("  FLEN                        Get list length");
    println!();
    println!("  SCREATE [name]              Create stack");
    println!("  SPUSH <value>               Push to stack");
    println!("  SPOP                        Pop from stack");
    println!("  SLEN                        Get stack size");
    println!();
    println!("  QCREATE [name]              Create queue");
    println!("  QPUSH <value>               Enqueue");
    println!("  QPOP                        Dequeue");
    println!("  QLEN                        Get queue size");
    println!();
    println!("  TCREATE [name]              Create binary tree");
    println!("  TINSERT <key>               Insert key into tree");
    println!("  TSEARCH <key>               Search key in tree");
    println!("  TCHECK                      Check if tree is full");
    println!("  TDEL <key>                  Delete key from tree");
    println!("  TGET <mode>                 Traverse tree (PRE, IN, POST, BFS)");
    println!("  TGETNODES <key> <mode>      Get predecessor/successor (PREV, NEXT)");
    println!();
    println!("  PRINT <name>                Print structure");
}
