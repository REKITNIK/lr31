// src/double_list.rs
use crate::{Structure, AsAny};
use std::fmt;

/// Узел двусвязного списка
#[derive(Debug)]
pub struct DFNode {
    pub key: String,
    pub next: Option<Box<DFNode>>,
    pub prev: Option<*mut DFNode>, // Сырой указатель для обратной ссылки
}

impl DFNode {
    pub fn new(key: String) -> Self {
        DFNode {
            key,
            next: None,
            prev: None,
        }
    }
}

/// Двусвязный список
#[derive(Debug)]
pub struct DFList {
    pub head: Option<Box<DFNode>>,
    pub tail: Option<*mut DFNode>, // Сырой указатель на хвост
    pub length: usize,
    pub name: String,
}

impl DFList {
    /// Создает новый пустой двусвязный список
    pub fn new() -> Self {
        DFList {
            head: None,
            tail: None,
            length: 0,
            name: String::new(),
        }
    }
    
    /// Добавляет элемент в начало списка
    pub fn push_front(&mut self, key: String) {
        let mut new_node = Box::new(DFNode::new(key));
        
        match self.head.take() {
            Some(mut old_head) => {
                // Старая голова становится второй
                old_head.prev = Some(new_node.as_mut() as *mut DFNode);
                new_node.next = Some(old_head);
                self.head = Some(new_node);
            }
            None => {
                // Список был пуст
                self.tail = Some(new_node.as_mut() as *mut DFNode);
                self.head = Some(new_node);
            }
        }
        
        self.length += 1;
    }
    
    /// Добавляет элемент в конец списка
    pub fn push_back(&mut self, key: String) {
        let mut new_node = Box::new(DFNode::new(key));
        
        unsafe {
            let new_node_ptr = new_node.as_mut() as *mut DFNode;
            
            match self.tail {
                Some(mut tail_ptr) => {
                    // Присоединяем новый узел к хвосту
                    if let Some(tail) = tail_ptr.as_mut() {
                        tail.next = Some(new_node);
                        new_node.prev = Some(tail_ptr);
                        self.tail = Some(new_node_ptr);
                    }
                }
                None => {
                    // Список был пуст
                    self.head = Some(new_node);
                    self.tail = Some(new_node_ptr);
                }
            }
        }
        
        self.length += 1;
    }
    
    /// Удаляет первый элемент и возвращает его значение
    pub fn pop_front(&mut self) -> Option<String> {
        self.head.take().map(|mut old_head| {
            match old_head.next.take() {
                Some(mut new_head) => {
                    new_head.prev = None;
                    self.head = Some(new_head);
                }
                None => {
                    // Список стал пустым
                    self.tail = None;
                }
            }
            
            self.length -= 1;
            old_head.key
        })
    }
    
    /// Удаляет последний элемент и возвращает его значение
    pub fn pop_back(&mut self) -> Option<String> {
        unsafe {
            self.tail.take().map(|tail_ptr| {
                if let Some(tail) = tail_ptr.as_mut() {
                    // Находим предыдущий узел
                    match tail.prev {
                        Some(mut prev_ptr) => {
                            if let Some(prev) = prev_ptr.as_mut() {
                                prev.next = None;
                                self.tail = Some(prev_ptr);
                            }
                        }
                        None => {
                            // Это был последний/единственный элемент
                            self.head = None;
                            self.tail = None;
                        }
                    }
                }
                
                self.length -= 1;
                
                // Извлекаем значение из Box (опасно, но нужно)
                let tail_box = Box::from_raw(tail_ptr);
                tail_box.key
            })
        }
    }
    
    /// Получает элемент по индексу
    pub fn get(&self, index: usize) -> Option<&str> {
        if index >= self.length {
            return None;
        }
        
        let mut current = self.head.as_ref();
        for _ in 0..index {
            current = current.and_then(|node| node.next.as_ref());
        }
        
        current.map(|node| node.key.as_str())
    }
    
    /// Устанавливает элемент по индексу
    pub fn set(&mut self, index: usize, value: String) -> Result<(), String> {
        if index >= self.length {
            return Err("Index out of bounds".to_string());
        }
        
        let mut current = self.head.as_mut();
        for _ in 0..index {
            current = current.and_then(|node| node.next.as_mut());
        }
        
        if let Some(node) = current {
            node.key = value;
            Ok(())
        } else {
            Err("Node not found".to_string())
        }
    }
    
    /// Вставляет элемент перед заданным индексом
    pub fn insert_before(&mut self, index: usize, key: String) -> Result<(), String> {
        if index > self.length {
            return Err("Index out of bounds".to_string());
        }
        
        if index == 0 {
            self.push_front(key);
            return Ok(());
        }
        
        if index == self.length {
            self.push_back(key);
            return Ok(());
        }
        
        // Находим узел, перед которым вставляем
        let mut current = self.head.as_mut();
        for _ in 0..index {
            current = current.and_then(|node| node.next.as_mut());
        }
        
        if let Some(current_node) = current {
            let mut new_node = Box::new(DFNode::new(key));
            
            unsafe {
                let new_node_ptr = new_node.as_mut() as *mut DFNode;
                
                // Связываем с предыдущим узлом
                if let Some(prev_ptr) = current_node.prev {
                    if let Some(prev) = prev_ptr.as_mut() {
                        prev.next = Some(new_node);
                        new_node.prev = Some(prev_ptr);
                    }
                }
                
                // Связываем с текущим узлом
                new_node.next = Some(Box::from_raw(current_node as *mut DFNode));
                current_node.prev = Some(new_node_ptr);
            }
            
            self.length += 1;
            Ok(())
        } else {
            Err("Node not found".to_string())
        }
    }
    
    /// Вставляет элемент после заданного индекса
    pub fn insert_after(&mut self, index: usize, key: String) -> Result<(), String> {
        if index >= self.length {
            return Err("Index out of bounds".to_string());
        }
        
        if index == self.length - 1 {
            self.push_back(key);
            return Ok(());
        }
        
        // Находим узел, после которого вставляем
        let mut current = self.head.as_mut();
        for _ in 0..index {
            current = current.and_then(|node| node.next.as_mut());
        }
        
        if let Some(current_node) = current {
            let mut new_node = Box::new(DFNode::new(key));
            
            unsafe {
                let new_node_ptr = new_node.as_mut() as *mut DFNode;
                
                // Связываем со следующим узлом
                if let Some(mut next_node) = current_node.next.take() {
                    next_node.prev = Some(new_node_ptr);
                    new_node.next = Some(next_node);
                }
                
                // Связываем с текущим узлом
                current_node.next = Some(new_node);
                new_node.prev = Some(current_node as *mut DFNode);
            }
            
            self.length += 1;
            Ok(())
        } else {
            Err("Node not found".to_string())
        }
    }
    
    /// Удаляет элемент по индексу
    pub fn remove(&mut self, index: usize) -> Result<String, String> {
        if index >= self.length {
            return Err("Index out of bounds".to_string());
        }
        
        if index == 0 {
            return self.pop_front().ok_or_else(|| "List is empty".to_string());
        }
        
        if index == self.length - 1 {
            return self.pop_back().ok_or_else(|| "List is empty".to_string());
        }
        
        // Находим удаляемый узел
        let mut current = self.head.as_mut();
        for _ in 0..index {
            current = current.and_then(|node| node.next.as_mut());
        }
        
        if let Some(node_to_remove) = current {
            let value = node_to_remove.key.clone();
            
            unsafe {
                // Связываем предыдущий и следующий узлы
                if let Some(prev_ptr) = node_to_remove.prev {
                    if let Some(prev) = prev_ptr.as_mut() {
                        prev.next = node_to_remove.next.take();
                    }
                }
                
                if let Some(mut next_node) = node_to_remove.next.take() {
                    next_node.prev = node_to_remove.prev;
                }
            }
            
            self.length -= 1;
            Ok(value)
        } else {
            Err("Node not found".to_string())
        }
    }
    
    /// Удаляет первый узел с заданным значением
    pub fn remove_by_value(&mut self, value: &str) -> bool {
        let mut current = self.head.as_mut();
        let mut index = 0;
        
        while let Some(node) = current {
            if node.key == value {
                if self.remove(index).is_ok() {
                    return true;
                }
            }
            current = node.next.as_mut();
            index += 1;
        }
        
        false
    }
    
    /// Находит узел с заданным значением
    pub fn find(&self, value: &str) -> Option<usize> {
        let mut current = self.head.as_ref();
        let mut index = 0;
        
        while let Some(node) = current {
            if node.key == value {
                return Some(index);
            }
            current = node.next.as_ref();
            index += 1;
        }
        
        None
    }
    
    /// Проверяет, пуст ли список
    pub fn is_empty(&self) -> bool {
        self.length == 0
    }
    
    /// Возвращает длину списка
    pub fn len(&self) -> usize {
        self.length
    }
    
    /// Очищает список
    pub fn clear(&mut self) {
        self.head = None;
        self.tail = None;
        self.length = 0;
    }
    
    /// Преобразует список в вектор
    pub fn to_vec(&self) -> Vec<String> {
        let mut result = Vec::with_capacity(self.length);
        let mut current = self.head.as_ref();
        
        while let Some(node) = current {
            result.push(node.key.clone());
            current = node.next.as_ref();
        }
        
        result
    }
}

impl Structure for DFList {
    fn serialize(&self) -> String {
        let elements: Vec<String> = self.to_vec();
        format!("L {} {} {}", self.name, self.length, elements.join(" "))
    }
    
    fn deserialize(&mut self, data: &str) {
        let parts: Vec<&str> = data.split_whitespace().collect();
        
        if parts.len() < 3 || parts[0] != "L" {
            return;
        }
        
        self.name = parts[1].to_string();
        self.length = parts[2].parse().unwrap_or(0);
        
        // Очищаем текущий список
        self.clear();
        
        // Добавляем элементы
        for i in 0..self.length {
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

//impl AsAny for DFList {
//    fn as_any(&self) -> &dyn std::any::Any {
//        self
//    }
//    
//    fn as_any_mut(&mut self) -> &mut dyn std::any::Any {
//        self
//    }
//}

impl Default for DFList {
    fn default() -> Self {
        Self::new()
    }
}

impl fmt::Display for DFList {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "DoubleList[")?;
        
        let mut current = self.head.as_ref();
        let mut first = true;
        
        while let Some(node) = current {
            if !first {
                write!(f, ", ")?;
            }
            write!(f, "{}", node.key)?;
            first = false;
            current = node.next.as_ref();
        }
        
        write!(f, "]")
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_push_front() {
        let mut list = DFList::new();
        list.push_front("world".to_string());
        list.push_front("hello".to_string());
        
        assert_eq!(list.len(), 2);
        assert_eq!(list.get(0), Some("hello"));
        assert_eq!(list.get(1), Some("world"));
    }
    
    #[test]
    fn test_push_back() {
        let mut list = DFList::new();
        list.push_back("hello".to_string());
        list.push_back("world".to_string());
        
        assert_eq!(list.len(), 2);
        assert_eq!(list.get(0), Some("hello"));
        assert_eq!(list.get(1), Some("world"));
    }
    
    #[test]
    fn test_pop_front() {
        let mut list = DFList::new();
        list.push_back("hello".to_string());
        list.push_back("world".to_string());
        
        assert_eq!(list.pop_front(), Some("hello".to_string()));
        assert_eq!(list.len(), 1);
        assert_eq!(list.get(0), Some("world"));
    }
    
    #[test]
    fn test_pop_back() {
        let mut list = DFList::new();
        list.push_back("hello".to_string());
        list.push_back("world".to_string());
        
        assert_eq!(list.pop_back(), Some("world".to_string()));
        assert_eq!(list.len(), 1);
        assert_eq!(list.get(0), Some("hello"));
    }
    
    #[test]
    fn test_insert_and_remove() {
        let mut list = DFList::new();
        list.push_back("a".to_string());
        list.push_back("c".to_string());
        
        list.insert_before(1, "b".to_string()).unwrap();
        assert_eq!(list.len(), 3);
        assert_eq!(list.get(1), Some("b"));
        
        let removed = list.remove(1).unwrap();
        assert_eq!(removed, "b");
        assert_eq!(list.len(), 2);
    }
    
    #[test]
    fn test_serialize() {
        let mut list = DFList::new();
        list.set_name("test_list".to_string());
        list.push_back("hello".to_string());
        list.push_back("world".to_string());
        
        let serialized = list.serialize();
        assert_eq!(serialized, "L test_list 2 hello world");
    }
    
    #[test]
    fn test_deserialize() {
        let mut list = DFList::new();
        list.deserialize("L my_list 3 a b c");
        
        assert_eq!(list.name(), "my_list");
        assert_eq!(list.len(), 3);
        assert_eq!(list.get(0), Some("a"));
        assert_eq!(list.get(1), Some("b"));
        assert_eq!(list.get(2), Some("c"));
    }
}
