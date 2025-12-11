// src/array.rs
use crate::Structure;

#[derive(Debug, Clone)]
pub struct ArNode {
    pub data: String,
}

#[derive(Debug)]
pub struct Array {
    pub head: Vec<ArNode>,
    pub len: usize,
    pub size: usize,
    pub name: String,
}

impl Array {
    pub fn new(initial_size: usize) -> Self {
        Array {
            head: vec![ArNode { data: String::new() }; initial_size],
            len: 0,
            size: initial_size,
            name: String::new(),
        }
    }
    
    pub fn push(&mut self, value: String) {
        if self.len >= self.size {
            let new_size = if self.size == 0 { 10 } else { self.size * 2 };
            self.head.resize(new_size, ArNode { data: String::new() });
            self.size = new_size;
        }
        if self.len < self.head.len() {
            self.head[self.len].data = value;
            self.len += 1;
        }
    }
    
    pub fn insert_at(&mut self, index: usize, value: String) -> Result<(), String> {
        if index > self.len {
            return Err("Index out of bounds".to_string());
        }
        
        if self.len >= self.size {
            let new_size = self.size * 2;
            self.head.resize(new_size, ArNode { data: String::new() });
            self.size = new_size;
        }
        
        // Сдвигаем элементы вправо
        for i in (index..self.len).rev() {
            if i + 1 < self.head.len() {
                self.head[i + 1].data = self.head[i].data.clone();
            }
        }
        
        if index < self.head.len() {
            self.head[index].data = value;
            self.len += 1;
            Ok(())
        } else {
            Err("Index out of bounds".to_string())
        }
    }
    
    pub fn get(&self, index: usize) -> Option<&str> {
        if index < self.len {
            Some(&self.head[index].data)
        } else {
            None
        }
    }
    
    pub fn set_at(&mut self, index: usize, value: String) -> Result<(), String> {
        if index < self.len {
            self.head[index].data = value;
            Ok(())
        } else {
            Err("Index out of bounds".to_string())
        }
    }
    
    pub fn delete_at(&mut self, index: usize) -> Result<(), String> {
        if index >= self.len {
            return Err("Index out of bounds".to_string());
        }
        
        // Сдвигаем элементы влево
        for i in index..self.len - 1 {
            if i + 1 < self.head.len() {
                self.head[i].data = self.head[i + 1].data.clone();
            }
        }
        
        self.len -= 1;
        Ok(())
    }
    
    pub fn len(&self) -> usize {
        self.len
    }
    
    pub fn is_empty(&self) -> bool {
        self.len == 0
    }
}

impl Structure for Array {
    fn serialize(&self) -> String {
        let values: Vec<String> = self.head[..self.len]
            .iter()
            .map(|node| node.data.clone())
            .collect();
        format!("M {} {} {}", self.name, self.len, values.join(" "))
    }
    
    fn deserialize(&mut self, data: &str) {
        let parts: Vec<&str> = data.split_whitespace().collect();
        if parts.len() < 3 || parts[0] != "M" {
            return;
        }
        
        self.name = parts[1].to_string();
        self.len = parts[2].parse().unwrap_or(0);
        
        let new_size = std::cmp::max(10, self.len * 2);
        self.head = vec![ArNode { data: String::new() }; new_size];
        self.size = new_size;
        
        for i in 0..self.len {
            if let Some(val) = parts.get(3 + i) {
                if i < self.head.len() {
                    self.head[i].data = val.to_string();
                }
            }
        }
    }
    
    fn name(&self) -> &str {
        &self.name
    }
    
    fn set_name(&mut self, name: String) {
        self.name = name;
    }
}
