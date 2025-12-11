// src/forward_list.rs
use crate::Structure;

#[derive(Debug)]
pub struct FNode {
    pub key: String,
    pub next: Option<Box<FNode>>,
}

#[derive(Debug)]
pub struct ForwardList {
    pub head: Option<Box<FNode>>,
    pub size: usize,
    pub name: String,
}

impl ForwardList {
    pub fn new() -> Self {
        ForwardList {
            head: None,
            size: 0,
            name: String::new(),
        }
    }
    
    pub fn push_front(&mut self, key: String) {
        let new_node = Box::new(FNode {
            key,
            next: self.head.take(),
        });
        self.head = Some(new_node);
        self.size += 1;
    }
    
    pub fn push_back(&mut self, key: String) {
        let new_node = Box::new(FNode { key, next: None });
        
        if let Some(ref mut head) = self.head {
            let mut current = head;
            while let Some(ref mut next) = current.next {
                current = next;
            }
            current.next = Some(new_node);
        } else {
            self.head = Some(new_node);
        }
        self.size += 1;
    }
    
    pub fn pop_front(&mut self) -> Option<String> {
        self.head.take().map(|node| {
            self.head = node.next;
            self.size -= 1;
            node.key
        })
    }
    
    pub fn front(&self) -> Option<&str> {
        self.head.as_ref().map(|node| node.key.as_str())
    }
    
    pub fn clear(&mut self) {
        self.head = None;
        self.size = 0;
    }
    
    pub fn to_vec(&self) -> Vec<String> {
        let mut result = Vec::new();
        let mut current = self.head.as_ref();
        
        while let Some(node) = current {
            result.push(node.key.clone());
            current = node.next.as_ref();
        }
        
        result
    }
    
    pub fn contains(&self, value: &str) -> bool {
        let mut current = self.head.as_ref();
        
        while let Some(node) = current {
            if node.key == value {
                return true;
            }
            current = node.next.as_ref();
        }
        
        false
    }
    
    pub fn len(&self) -> usize {
        self.size
    }
    
    pub fn is_empty(&self) -> bool {
        self.size == 0
    }
    
    pub fn get(&self, index: usize) -> Option<&str> {
        if index >= self.size {
            return None;
        }
        
        let mut current = self.head.as_ref();
        for _ in 0..index {
            current = current.and_then(|node| node.next.as_ref());
        }
        
        current.map(|node| node.key.as_str())
    }
}

impl Structure for ForwardList {
    fn serialize(&self) -> String {
        let values: Vec<String> = self.to_vec();
        format!("F {} {} {}", self.name, self.size, values.join(" "))
    }
    
    fn deserialize(&mut self, data: &str) {
        let parts: Vec<&str> = data.split_whitespace().collect();
        if parts.len() < 3 || parts[0] != "F" {
            return;
        }
        
        self.name = parts[1].to_string();
        let count: usize = parts[2].parse().unwrap_or(0);
        
        self.clear();
        
        for i in 0..count {
            if let Some(value) = parts.get(3 + i) {
                self.push_back(value.to_string());
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
