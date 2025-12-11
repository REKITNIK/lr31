// src/file_io.rs
use std::fs::{self, File, OpenOptions};
use std::io::{self, Read, Write, BufRead, BufReader, BufWriter};
use std::path::{Path, PathBuf};
use std::collections::HashMap;

use crate::{Structure, Factory, StructureManager};
use crate::array::Array;
use crate::forward_list::ForwardList;
use crate::double_list::DFList;
use crate::stack::Stack;
use crate::queue::Queue;
use crate::binary_tree::BTree;

/// Кроссплатформенные функции для работы с файлами и директориями
pub struct FileIO;

impl FileIO {
    /// Создает директорию для файла, если она не существует
    pub fn ensure_directory_exists(filename: &str) -> io::Result<()> {
        let path = Path::new(filename);
        
        if let Some(parent) = path.parent() {
            if !parent.exists() {
                fs::create_dir_all(parent)?;
            }
        }
        
        Ok(())
    }
    
    /// Создает пустой файл
    pub fn create_file(filename: &str) -> io::Result<()> {
        Self::ensure_directory_exists(filename)?;
        
        if !Path::new(filename).exists() {
            File::create(filename)?;
        }
        
        Ok(())
    }
    
    /// Проверяет существование файла
    pub fn file_exists(filename: &str) -> bool {
        Path::new(filename).exists()
    }
    
    /// Возвращает тип структуры из первой строки файла
    pub fn get_structure_type(filename: &str) -> io::Result<String> {
        let file = File::open(filename)?;
        let reader = BufReader::new(file);
        
        if let Some(Ok(first_line)) = reader.lines().next() {
            let parts: Vec<&str> = first_line.split_whitespace().collect();
            if parts.is_empty() {
                return Err(io::Error::new(
                    io::ErrorKind::InvalidData,
                    "Empty file"
                ));
            }
            
            let type_char = parts[0];
            match type_char {
                "M" => Ok("Array".to_string()),
                "F" => Ok("ForwardList".to_string()),
                "L" => Ok("DoubleList".to_string()),
                "S" => Ok("Stack".to_string()),
                "Q" => Ok("Queue".to_string()),
                "T" => Ok("FBTree".to_string()),
                _ => Err(io::Error::new(
                    io::ErrorKind::InvalidData,
                    format!("Unknown type: {}", type_char)
                ))
            }
        } else {
            Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "Cannot read file"
            ))
        }
    }
    
    /// Загружает одну структуру из файла (устаревший метод, для совместимости)
    pub fn load_structure_from_file(filename: &str) -> io::Result<(String, Box<dyn Structure>)> {
        let structure_type = Self::get_structure_type(filename)?;
        
        match structure_type.as_str() {
            "Array" => {
                let array = Self::load_array_from_file(filename)?;
                Ok(("Array".to_string(), Box::new(array)))
            }
            "ForwardList" => {
                let list = Self::load_forward_list_from_file(filename)?;
                Ok(("ForwardList".to_string(), Box::new(list)))
            }
            "DoubleList" => {
                let list = Self::load_double_list_from_file(filename)?;
                Ok(("DoubleList".to_string(), Box::new(list)))
            }
            "Stack" => {
                let stack = Self::load_stack_from_file(filename)?;
                Ok(("Stack".to_string(), Box::new(stack)))
            }
            "Queue" => {
                let queue = Self::load_queue_from_file(filename)?;
                Ok(("Queue".to_string(), Box::new(queue)))
            }
            "FBTree" => {
                let tree = Self::load_tree_from_file(filename)?;
                Ok(("FBTree".to_string(), Box::new(tree)))
            }
            _ => Err(io::Error::new(
                io::ErrorKind::InvalidData,
                format!("Unknown structure type: {}", structure_type)
            ))
        }
    }
    
    /// Сохраняет одну структуру в файл (устаревший метод, для совместимости)
    pub fn save_structure_to_file(
        filename: &str,
        structure_type: &str,
        structure: &dyn Structure
    ) -> io::Result<()> {
        Self::ensure_directory_exists(filename)?;
        
        let serialized = structure.serialize();
        let mut file = OpenOptions::new()
            .write(true)
            .create(true)
            .truncate(true)
            .open(filename)?;
        
        writeln!(file, "{}", serialized)?;
        Ok(())
    }
    
    // === ARRAY ===
    
    /// Сохраняет массив в файл
    pub fn save_array_to_file(filename: &str, array: &Array) -> io::Result<()> {
        Self::ensure_directory_exists(filename)?;
        
        let mut file = OpenOptions::new()
            .write(true)
            .create(true)
            .truncate(true)
            .open(filename)?;
        
        writeln!(file, "{}", array.serialize())?;
        Ok(())
    }
    
    /// Загружает массив из файла
    pub fn load_array_from_file(filename: &str) -> io::Result<Array> {
        let file = File::open(filename)?;
        let mut reader = BufReader::new(file);
        let mut line = String::new();
        reader.read_line(&mut line)?;
        
        let mut array = Array::new(10);
        array.deserialize(line.trim());
        Ok(array)
    }
    
    // === FORWARD LIST ===
    
    /// Сохраняет односвязный список в файл
    pub fn save_forward_list_to_file(filename: &str, list: &ForwardList) -> io::Result<()> {
        Self::ensure_directory_exists(filename)?;
        
        let mut file = OpenOptions::new()
            .write(true)
            .create(true)
            .truncate(true)
            .open(filename)?;
        
        writeln!(file, "{}", list.serialize())?;
        Ok(())
    }
    
    /// Загружает односвязный список из файла
    pub fn load_forward_list_from_file(filename: &str) -> io::Result<ForwardList> {
        let file = File::open(filename)?;
        let mut reader = BufReader::new(file);
        let mut line = String::new();
        reader.read_line(&mut line)?;
        
        let mut list = ForwardList::new();
        list.deserialize(line.trim());
        Ok(list)
    }
    
    // === DOUBLE LIST ===
    
    /// Сохраняет двусвязный список в файл
    pub fn save_double_list_to_file(filename: &str, list: &DFList) -> io::Result<()> {
        Self::ensure_directory_exists(filename)?;
        
        let mut file = OpenOptions::new()
            .write(true)
            .create(true)
            .truncate(true)
            .open(filename)?;
        
        writeln!(file, "{}", list.serialize())?;
        Ok(())
    }
    
    /// Загружает двусвязный список из файла
    pub fn load_double_list_from_file(filename: &str) -> io::Result<DFList> {
        let file = File::open(filename)?;
        let mut reader = BufReader::new(file);
        let mut line = String::new();
        reader.read_line(&mut line)?;
        
        let mut list = DFList::new();
        list.deserialize(line.trim());
        Ok(list)
    }
    
    // === STACK ===
    
    /// Сохраняет стек в файл
    pub fn save_stack_to_file(filename: &str, stack: &Stack) -> io::Result<()> {
        Self::ensure_directory_exists(filename)?;
        
        let mut file = OpenOptions::new()
            .write(true)
            .create(true)
            .truncate(true)
            .open(filename)?;
        
        writeln!(file, "{}", stack.serialize())?;
        Ok(())
    }
    
    /// Загружает стек из файла
    pub fn load_stack_from_file(filename: &str) -> io::Result<Stack> {
        let file = File::open(filename)?;
        let mut reader = BufReader::new(file);
        let mut line = String::new();
        reader.read_line(&mut line)?;
        
        let mut stack = Stack::new();
        stack.deserialize(line.trim());
        Ok(stack)
    }
    
    // === QUEUE ===
    
    /// Сохраняет очередь в файл
    pub fn save_queue_to_file(filename: &str, queue: &Queue) -> io::Result<()> {
        Self::ensure_directory_exists(filename)?;
        
        let mut file = OpenOptions::new()
            .write(true)
            .create(true)
            .truncate(true)
            .open(filename)?;
        
        writeln!(file, "{}", queue.serialize())?;
        Ok(())
    }
    
    /// Загружает очередь из файла
    pub fn load_queue_from_file(filename: &str) -> io::Result<Queue> {
        let file = File::open(filename)?;
        let mut reader = BufReader::new(file);
        let mut line = String::new();
        reader.read_line(&mut line)?;
        
        let mut queue = Queue::new();
        queue.deserialize(line.trim());
        Ok(queue)
    }
    
    // === BINARY TREE ===
    
    /// Сохраняет бинарное дерево в файл
    pub fn save_tree_to_file(filename: &str, tree: &BTree) -> io::Result<()> {
        Self::ensure_directory_exists(filename)?;
        
        let mut file = OpenOptions::new()
            .write(true)
            .create(true)
            .truncate(true)
            .open(filename)?;
        
        writeln!(file, "{}", tree.serialize())?;
        Ok(())
    }
    
    /// Загружает бинарное дерево из файла
    pub fn load_tree_from_file(filename: &str) -> io::Result<BTree> {
        let file = File::open(filename)?;
        let mut reader = BufReader::new(file);
        let mut line = String::new();
        reader.read_line(&mut line)?;
        
        let mut tree = BTree::new();
        tree.deserialize(line.trim());
        Ok(tree)
    }
    
    // === DATABASE OPERATIONS ===
    
    /// Загружает всю базу данных структур из файла
    pub fn load_database_from_file(
        filename: &str,
        database: &mut HashMap<String, Box<dyn Structure>>
    ) -> io::Result<()> {
        if !Path::new(filename).exists() {
            return Ok(()); // Файл не существует - ничего не загружаем
        }
        
        let file = File::open(filename)?;
        let reader = BufReader::new(file);
        
        for line in reader.lines() {
            let line = line?;
            if line.trim().is_empty() {
                continue;
            }
            
            let parts: Vec<&str> = line.split_whitespace().collect();
            if parts.len() < 3 {
                continue;
            }
            
            let type_char = parts[0];
            let name = parts[1].to_string();
            
            // Создаем структуру с помощью фабрики
            if let Some(mut structure) = Factory::create_from_char(type_char.chars().next().unwrap()) {
                structure.deserialize(&line);
                structure.set_name(name.clone());
                database.insert(name, structure);
            }
        }
        
        Ok(())
    }
    
    /// Сохраняет всю базу данных структур в файл
    pub fn save_database_to_file(
        filename: &str,
        database: &HashMap<String, Box<dyn Structure>>
    ) -> io::Result<()> {
        Self::ensure_directory_exists(filename)?;
        
        let file = OpenOptions::new()
            .write(true)
            .create(true)
            .truncate(true)
            .open(filename)?;
        
        let mut writer = BufWriter::new(file);
        
        for (_, structure) in database {
            writeln!(writer, "{}", structure.serialize())?;
        }
        
        writer.flush()?;
        Ok(())
    }
    
    /// Экспорт базы данных в удобочитаемый формат
    pub fn export_database_human_readable(
        filename: &str,
        database: &HashMap<String, Box<dyn Structure>>
    ) -> io::Result<()> {
        Self::ensure_directory_exists(filename)?;
        
        let mut file = OpenOptions::new()
            .write(true)
            .create(true)
            .truncate(true)
            .open(filename)?;
        
        writeln!(file, "=== DATA STRUCTURES DATABASE ===")?;
        writeln!(file, "Total structures: {}", database.len())?;
        writeln!(file)?;
        
        for (name, structure) in database {
            let type_name = match structure.serialize().chars().next() {
                Some('M') => "Array",
                Some('F') => "Forward List",
                Some('L') => "Double List",
                Some('S') => "Stack",
                Some('Q') => "Queue",
                Some('T') => "Binary Tree",
                _ => "Unknown",
            };
            
            writeln!(file, "Structure: {} ({})", name, type_name)?;
            
            // Парсим serialized данные для красивого вывода
            let serialized = structure.serialize();
            let parts: Vec<&str> = serialized.split_whitespace().collect();
            
            if parts.len() >= 3 {
                let count: usize = parts[2].parse().unwrap_or(0);
                writeln!(file, "  Elements: {}", count)?;
                
                if count > 0 && parts.len() > 3 {
                    write!(file, "  Values: ")?;
                    for i in 3..parts.len().min(10) {
                        write!(file, "{} ", parts[i])?;
                    }
                    if parts.len() > 13 {
                        write!(file, "...")?;
                    }
                    writeln!(file)?;
                }
            }
            
            writeln!(file)?;
        }
        
        Ok(())
    }
    
    /// Импорт из старого формата (для совместимости)
    pub fn import_from_legacy_format(
        legacy_filename: &str,
        new_filename: &str
    ) -> io::Result<()> {
        let mut database = HashMap::new();
        
        // Пытаемся определить тип файла по расширению или содержимому
        if legacy_filename.ends_with(".array") {
            let array = Self::load_array_from_file(legacy_filename)?;
            database.insert("imported_array".to_string(), Box::new(array));
        } else if legacy_filename.ends_with(".list") {
            let list = Self::load_forward_list_from_file(legacy_filename)?;
            database.insert("imported_list".to_string(), Box::new(list));
        } else {
            // Пробуем автоматически определить тип
            if let Ok((_, structure)) = Self::load_structure_from_file(legacy_filename) {
                database.insert("imported".to_string(), structure);
            }
        }
        
        Self::save_database_to_file(new_filename, &database)
    }
    
    /// Создает резервную копию базы данных
    pub fn create_backup(original_filename: &str) -> io::Result<String> {
        if !Path::new(original_filename).exists() {
            return Err(io::Error::new(
                io::ErrorKind::NotFound,
                "Original file not found"
            ));
        }
        
        // Создаем имя файла для бэкапа с timestamp
        let timestamp = chrono::Local::now().format("%Y%m%d_%H%M%S");
        let backup_filename = format!("{}.backup_{}", original_filename, timestamp);
        
        // Копируем файл
        fs::copy(original_filename, &backup_filename)?;
        
        Ok(backup_filename)
    }
    
    /// Восстанавливает из резервной копии
    pub fn restore_from_backup(backup_filename: &str, target_filename: &str) -> io::Result<()> {
        if !Path::new(backup_filename).exists() {
            return Err(io::Error::new(
                io::ErrorKind::NotFound,
                "Backup file not found"
            ));
        }
        
        fs::copy(backup_filename, target_filename)?;
        Ok(())
    }
    
    /// Получает список всех структур в файле
    pub fn list_structures_in_file(filename: &str) -> io::Result<Vec<(String, String)>> {
        let mut structures = Vec::new();
        
        if !Path::new(filename).exists() {
            return Ok(structures);
        }
        
        let file = File::open(filename)?;
        let reader = BufReader::new(file);
        
        for line in reader.lines() {
            let line = line?;
            if line.trim().is_empty() {
                continue;
            }
            
            let parts: Vec<&str> = line.split_whitespace().collect();
            if parts.len() < 2 {
                continue;
            }
            
            let type_char = parts[0];
            let name = parts[1].to_string();
            let type_name = match type_char {
                "M" => "Array",
                "F" => "Forward List",
                "L" => "Double List",
                "S" => "Stack",
                "Q" => "Queue",
                "T" => "Binary Tree",
                _ => "Unknown",
            };
            
            structures.push((name, type_name.to_string()));
        }
        
        Ok(structures)
    }
    
    /// Валидация файла базы данных
    pub fn validate_database_file(filename: &str) -> io::Result<Vec<String>> {
        let mut errors = Vec::new();
        
        if !Path::new(filename).exists() {
            return Ok(vec!["File does not exist".to_string()]);
        }
        
        let file = File::open(filename)?;
        let reader = BufReader::new(file);
        
        for (line_num, line) in reader.lines().enumerate() {
            let line = line?;
            if line.trim().is_empty() {
                continue;
            }
            
            let parts: Vec<&str> = line.split_whitespace().collect();
            
            // Проверка минимального количества частей
            if parts.len() < 3 {
                errors.push(format!("Line {}: Too few fields", line_num + 1));
                continue;
            }
            
            // Проверка типа структуры
            let type_char = parts[0];
            if !["M", "F", "L", "S", "Q", "T"].contains(&type_char) {
                errors.push(format!("Line {}: Unknown structure type '{}'", line_num + 1, type_char));
                continue;
            }
            
            // Проверка количества элементов
            if let Ok(count) = parts[2].parse::<usize>() {
                let expected_fields = 3 + count;
                if parts.len() < expected_fields {
                    errors.push(format!(
                        "Line {}: Expected {} fields, got {}",
                        line_num + 1,
                        expected_fields,
                        parts.len()
                    ));
                }
            } else {
                errors.push(format!("Line {}: Invalid count '{}'", line_num + 1, parts[2]));
            }
        }
        
        Ok(errors)
    }
}

/// Вспомогательные функции для менеджера
impl StructureManager {
    /// Загружает базу данных из файла
    /// Создает резервную копию текущего файла
    pub fn create_backup(&self) -> Result<String, String> {
        if let Some(filename) = &self.current_filename {
            FileIO::create_backup(filename)
                .map_err(|e| format!("ERROR: Cannot create backup: {}", e))
        } else {
            Ok(Vec::new())
        }
    }
    
    /// Валидирует текущий файл
    pub fn validate_current_file(&self) -> Result<Vec<String>, String> {
        if let Some(filename) = &self.current_filename {
            FileIO::validate_database_file(filename)
                .map_err(|e| format!("ERROR: Cannot validate file: {}", e))
        } else {
            Ok(vec!["No file selected".to_string()])
        }
    }
}
