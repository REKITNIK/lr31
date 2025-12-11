// queue.rs
use crate::Structure;
use crate::forward_list::ForwardList;

#[derive(Debug)]
pub struct Queue {
    list: ForwardList,
    size: usize,
    max_size: usize,
    name: String,
}

impl Queue {
    pub fn new() -> Self {
        Queue {
            list: ForwardList::new(),
            size: 0,
            max_size: 1000,
            name: String::new(),
        }
    }
    
    pub fn with_max_size(max_size: usize) -> Self {
        Queue {
            list: ForwardList::new(),
            size: 0,
            max_size,
            name: String::new(),
        }
    }
    
    pub fn enqueue(&mut self, value: String) -> Result<(), String> {
        if self.size >= self.max_size {
            return Err("ERROR: Queue overflow".to_string());
        }
        
        self.list.push_back(value);
        self.size += 1;
        Ok(())
    }
    
    pub fn dequeue(&mut self) -> Option<String> {
        if self.size == 0 {
            return None;
        }
        
        let value = self.list.pop_front();
        if value.is_some() {
            self.size -= 1;
        }
        value
    }
    
    pub fn front(&self) -> Option<&str> {
        self.list.front()
    }
    
    pub fn is_empty(&self) -> bool {
        self.size == 0
    }
    
    pub fn is_full(&self) -> bool {
        self.size >= self.max_size
    }
    
    pub fn len(&self) -> usize {
        self.size
    }
    
    pub fn clear(&mut self) {
        self.list.clear();
        self.size = 0;
    }
}

impl Structure for Queue {
    fn serialize(&self) -> String {
        // Формат: Q name size front ... back
        let values: Vec<String> = self.list.to_vec();
        format!("Q {} {} {}", self.name, self.size, values.join(" "))
    }
    
    fn deserialize(&mut self, data: &str) {
        let parts: Vec<&str> = data.split_whitespace().collect();
        if parts.len() < 3 || parts[0] != "Q" {
            return;
        }
        
        self.name = parts[1].to_string();
        self.size = parts[2].parse().unwrap_or(0);
        
        // Очищаем текущую очередь
        self.clear();
        
        // Добавляем элементы в порядке FIFO
        for i in 3..parts.len() {
            if let Err(e) = self.enqueue(parts[i].to_string()) {
                eprintln!("Warning during deserialize: {}", e);
                break;
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

// Реализация AsAny для downcast
use crate::AsAny;
