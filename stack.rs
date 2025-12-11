// src/stack.rs
use crate::{Structure, AsAny};
use crate::forward_list::ForwardList;
use std::fmt;

/// Стек (LIFO - Last In, First Out)
/// Реализован как адаптер над ForwardList
#[derive(Debug)]
pub struct Stack {
    pub list: ForwardList,
    pub size: usize,
    pub max_size: usize,
    pub name: String,
}

impl Stack {
    /// Создает новый пустой стек
    pub fn new() -> Self {
        Stack {
            list: ForwardList::new(),
            size: 0,
            max_size: 1000,
            name: String::new(),
        }
    }
    
    /// Создает стек с заданным максимальным размером
    pub fn with_max_size(max_size: usize) -> Self {
        Stack {
            list: ForwardList::new(),
            size: 0,
            max_size,
            name: String::new(),
        }
    }
    
    /// Добавляет элемент на вершину стека
    pub fn push(&mut self, value: String) -> Result<(), String> {
        if self.size >= self.max_size {
            return Err("ERROR: Stack overflow".to_string());
        }
        
        // Используем push_front для эмуляции стека
        self.list.push_front(value);
        self.size += 1;
        Ok(())
    }
    
    /// Удаляет и возвращает элемент с вершины стека
    pub fn pop(&mut self) -> Option<String> {
        if self.size == 0 {
            return None;
        }
        
        // Используем pop_front для эмуляции стека
        let result = self.list.pop_front();
        if result.is_some() {
            self.size -= 1;
        }
        result
    }
    
    /// Возвращает элемент с вершины стека без удаления
    pub fn peek(&self) -> Option<&str> {
        self.list.front()
    }
    
    /// Проверяет, пуст ли стек
    pub fn is_empty(&self) -> bool {
        self.size == 0
    }
    
    /// Проверяет, переполнен ли стек
    pub fn is_full(&self) -> bool {
        self.size >= self.max_size
    }
    
    /// Возвращает количество элементов в стеке
    pub fn len(&self) -> usize {
        self.size
    }
    
    /// Очищает стек
    pub fn clear(&mut self) {
        self.list.clear();
        self.size = 0;
    }
    
    /// Преобразует стек в вектор (от вершины к основанию)
    pub fn to_vec(&self) -> Vec<String> {
        self.list.to_vec()
    }
    
    /// Инициализирует стек
    pub fn initialize(&mut self) {
        self.list = ForwardList::new();
        self.size = 0;
    }
}

impl Structure for Stack {
    fn serialize(&self) -> String {
        // Формат: S name size top ... bottom
        let elements: Vec<String> = self.to_vec();
        format!("S {} {} {}", self.name, self.size, elements.join(" "))
    }
    
    fn deserialize(&mut self, data: &str) {
        let parts: Vec<&str> = data.split_whitespace().collect();
        
        if parts.len() < 3 || parts[0] != "S" {
            return;
        }
        
        self.name = parts[1].to_string();
        self.size = parts[2].parse().unwrap_or(0);
        
        // Очищаем текущий стек
        self.clear();
        
        // Добавляем элементы в обратном порядке, потому что в файле они хранятся top->bottom
        // а push_front будет добавлять их в правильном порядке
        for i in 0..self.size {
            if let Some(value) = parts.get(3 + i) {
                if let Err(e) = self.push(value.to_string()) {
                    eprintln!("Warning during deserialize: {}", e);
                    break;
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


impl Default for Stack {
    fn default() -> Self {
        Self::new()
    }
}

impl fmt::Display for Stack {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "Stack[size={}, max={}, name={}] {{ ", 
               self.size, self.max_size, self.name)?;
        
        let elements = self.to_vec();
        write!(f, "[")?;
        
        for (i, elem) in elements.iter().enumerate() {
            if i > 0 {
                write!(f, ", ")?;
            }
            write!(f, "{}", elem)?;
        }
        
        write!(f, "] }}")
    }
}

// Реализация функций из Stack.h (C++ интерфейс)
pub fn initialize_stack(stack: &mut Stack) {
    stack.initialize();
}

pub fn push_stack(stack: &mut Stack, data: &str) -> Result<(), String> {
    stack.push(data.to_string())
}

pub fn pop_stack(stack: &mut Stack) -> Result<String, String> {
    stack.pop().ok_or_else(|| "ERROR: Stack is empty".to_string())
}

pub fn peek_stack(stack: &Stack) -> Result<&str, String> {
    stack.peek().ok_or_else(|| "ERROR: Stack is empty".to_string())
}

pub fn is_stack_empty(stack: &Stack) -> bool {
    stack.is_empty()
}

pub fn is_stack_full(stack: &Stack) -> bool {
    stack.is_full()
}

pub fn get_stack_size(stack: &Stack) -> usize {
    stack.len()
}

pub fn clear_stack(stack: &mut Stack) {
    stack.clear();
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_stack_push_pop() {
        let mut stack = Stack::new();
        
        assert!(stack.push("first".to_string()).is_ok());
        assert!(stack.push("second".to_string()).is_ok());
        assert!(stack.push("third".to_string()).is_ok());
        
        assert_eq!(stack.len(), 3);
        assert!(!stack.is_empty());
        
        assert_eq!(stack.peek(), Some("third"));
        assert_eq!(stack.pop(), Some("third".to_string()));
        assert_eq!(stack.pop(), Some("second".to_string()));
        assert_eq!(stack.pop(), Some("first".to_string()));
        
        assert!(stack.is_empty());
        assert_eq!(stack.pop(), None);
    }
    
    #[test]
    fn test_stack_overflow() {
        let mut stack = Stack::with_max_size(2);
        
        assert!(stack.push("a".to_string()).is_ok());
        assert!(stack.push("b".to_string()).is_ok());
        
        let result = stack.push("c".to_string());
        assert!(result.is_err());
        assert_eq!(result.unwrap_err(), "ERROR: Stack overflow");
    }
    
    #[test]
    fn test_stack_serialize() {
        let mut stack = Stack::new();
        stack.set_name("test_stack".to_string());
        
        stack.push("bottom".to_string()).unwrap();
        stack.push("middle".to_string()).unwrap();
        stack.push("top".to_string()).unwrap();
        
        let serialized = stack.serialize();
        // Ожидаем: S test_stack 3 top middle bottom
        assert_eq!(serialized, "S test_stack 3 top middle bottom");
    }
    
    #[test]
    fn test_stack_deserialize() {
        let mut stack = Stack::new();
        stack.deserialize("S my_stack 3 x y z");
        
        assert_eq!(stack.name(), "my_stack");
        assert_eq!(stack.len(), 3);
        
        // Проверяем порядок LIFO
        assert_eq!(stack.pop(), Some("x".to_string()));  // top
        assert_eq!(stack.pop(), Some("y".to_string()));  // middle
        assert_eq!(stack.pop(), Some("z".to_string()));  // bottom
    }
    
    #[test]
    fn test_stack_clear() {
        let mut stack = Stack::new();
        
        stack.push("a".to_string()).unwrap();
        stack.push("b".to_string()).unwrap();
        stack.push("c".to_string()).unwrap();
        
        assert_eq!(stack.len(), 3);
        
        stack.clear();
        assert_eq!(stack.len(), 0);
        assert!(stack.is_empty());
        assert_eq!(stack.pop(), None);
    }
    
    #[test]
    fn test_stack_interface_functions() {
        let mut stack = Stack::new();
        
        assert!(push_stack(&mut stack, "test").is_ok());
        assert_eq!(get_stack_size(&stack), 1);
        assert!(!is_stack_empty(&stack));
        
        let peek_result = peek_stack(&stack);
        assert!(peek_result.is_ok());
        assert_eq!(peek_result.unwrap(), "test");
        
        let pop_result = pop_stack(&mut stack);
        assert!(pop_result.is_ok());
        assert_eq!(pop_result.unwrap(), "test");
        
        assert!(is_stack_empty(&stack));
    }
}
