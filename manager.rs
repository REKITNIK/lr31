// src/manager.rs
use std::collections::HashMap;
use crate::Structure;
use crate::array::Array;
use crate::forward_list::ForwardList;
use crate::stack::Stack;
use crate::queue::Queue;

pub struct StructureManager {
    pub database: HashMap<String, Box<dyn Structure>>,
    pub current_filename: Option<String>,
}

impl StructureManager {
    pub fn new() -> Self {
        StructureManager {
            database: HashMap::new(),
            current_filename: None,
        }
    }
    
    pub fn set_filename(&mut self, filename: String) {
        self.current_filename = Some(filename);
    }
    
    pub fn cleanup(&mut self) {
        self.database.clear();
    }
    
    pub fn get_structure_mut<T: 'static>(&mut self, name: &str) -> Option<&mut T> {
        self.database.get_mut(name)
            .and_then(|s| (s as &mut dyn std::any::Any).downcast_mut::<T>())
    }
    
    pub fn execute_command(&mut self, command: &str) -> Result<String, String> {
        let tokens: Vec<String> = command.split_whitespace()
            .map(|s| s.to_string())
            .collect();
        
        if tokens.is_empty() {
            return Err("ERROR 10: Unknown command".to_string());
        }
        
        let cmd = &tokens[0];
        
        match cmd.chars().next() {
            Some('M') => self.handle_m_command(&tokens),
            Some('F') => self.handle_f_command(&tokens),
            Some('S') => self.handle_s_command(&tokens),
            Some('Q') => self.handle_q_command(&tokens),
            _ => Err("ERROR 10: Unknown command".to_string())
        }
    }
    
    fn handle_m_command(&mut self, tokens: &[String]) -> Result<String, String> {
        if tokens.is_empty() {
            return Err("ERROR 10: Unknown command".to_string());
        }
        
        let cmd = &tokens[0];
        
        if cmd == "MCREATE" {
            let name = if tokens.len() > 1 {
                tokens[1].clone()
            } else {
                "default".to_string()
            };
            
            if self.database.contains_key(&name) {
                return Err("ERROR 21: Structure already exists".to_string());
            }
            
            let mut arr = Array::new(10);
            arr.set_name(name.clone());
            self.database.insert(name, Box::new(arr));
            return Ok("Array created".to_string());
        }
        
        let (name, param_start) = if tokens.len() > 1 && self.database.contains_key(&tokens[1]) {
            (tokens[1].clone(), 2)
        } else {
            ("default".to_string(), 1)
        };
        
        let arr = self.get_structure_mut::<Array>(&name)
            .ok_or_else(|| format!("ERROR 20: Structure not found: {}", name))?;
        
        match cmd.as_str() {
            "MPUSH" => {
                if tokens.len() <= param_start {
                    return Err("ERROR 30: Invalid index/argument".to_string());
                }
                arr.push(tokens[param_start].clone());
                Ok("Element pushed".to_string())
            }
            "MGET" => {
                if tokens.len() <= param_start {
                    return Err("ERROR 30: Invalid index/argument".to_string());
                }
                let idx = tokens[param_start].parse::<usize>()
                    .map_err(|_| "ERROR 30: Invalid index/argument".to_string())?;
                arr.get(idx)
                    .map(|s| s.to_string())
                    .ok_or_else(|| "ERROR 30: Invalid index/argument".to_string())
            }
            "MLEN" => {
                Ok(arr.len().to_string())
            }
            _ => Err("ERROR 10: Unknown command".to_string())
        }
    }
    
    fn handle_f_command(&mut self, tokens: &[String]) -> Result<String, String> {
        if tokens.is_empty() {
            return Err("ERROR 10: Unknown command".to_string());
        }
        
        let cmd = &tokens[0];
        
        if cmd == "FCREATE" {
            let name = if tokens.len() > 1 {
                tokens[1].clone()
            } else {
                "default".to_string()
            };
            
            if self.database.contains_key(&name) {
                return Err("ERROR 21: Structure already exists".to_string());
            }
            
            let mut list = ForwardList::new();
            list.set_name(name.clone());
            self.database.insert(name, Box::new(list));
            return Ok("Forward list created".to_string());
        }
        
        Ok("Forward list command".to_string())
    }
    
    fn handle_s_command(&mut self, tokens: &[String]) -> Result<String, String> {
        if tokens.is_empty() {
            return Err("ERROR 10: Unknown command".to_string());
        }
        
        let cmd = &tokens[0];
        
        if cmd == "SCREATE" {
            let name = if tokens.len() > 1 {
                tokens[1].clone()
            } else {
                "default".to_string()
            };
            
            if self.database.contains_key(&name) {
                return Err("ERROR 21: Structure already exists".to_string());
            }
            
            let mut stack = Stack::new();
            stack.set_name(name.clone());
            self.database.insert(name, Box::new(stack));
            return Ok("Stack created".to_string());
        }
        
        Ok("Stack command".to_string())
    }
    
    fn handle_q_command(&mut self, tokens: &[String]) -> Result<String, String> {
        if tokens.is_empty() {
            return Err("ERROR 10: Unknown command".to_string());
        }
        
        let cmd = &tokens[0];
        
        if cmd == "QCREATE" {
            let name = if tokens.len() > 1 {
                tokens[1].clone()
            } else {
                "default".to_string()
            };
            
            if self.database.contains_key(&name) {
                return Err("ERROR 21: Structure already exists".to_string());
            }
            
            let mut queue = Queue::new();
            queue.set_name(name.clone());
            self.database.insert(name, Box::new(queue));
            return Ok("Queue created".to_string());
        }
        
        Ok("Queue command".to_string())
    }
    
    pub fn print_current_structure(&self, name: &str) -> Result<(), String> {
        if let Some(structure) = self.database.get(name) {
            println!("{}", structure.serialize());
            Ok(())
        } else {
            Err(format!("ERROR 20: Structure not found: {}", name))
        }
    }
}
