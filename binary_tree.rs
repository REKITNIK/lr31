// binary_tree.rs
use crate::Structure;

#[derive(Debug, Clone)]
pub struct BNode {
    pub key: i32,
    pub left: Option<Box<BNode>>,
    pub right: Option<Box<BNode>>,
    pub parent: Option<*mut BNode>, // Используем raw pointer для родителя
}

impl BNode {
    pub fn new(key: i32) -> Self {
        BNode {
            key,
            left: None,
            right: None,
            parent: None,
        }
    }
}

#[derive(Debug)]
pub struct BTree {
    pub root: Option<Box<BNode>>,
    pub name: String,
}

impl BTree {
    pub fn new() -> Self {
        BTree {
            root: None,
            name: String::new(),
        }
    }
    
    // Вставка узла
    pub fn add_node(&mut self, key: i32) -> Result<(), String> {
        let new_node = Box::new(BNode::new(key));
        
        if self.root.is_none() {
            self.root = Some(new_node);
            return Ok(());
        }
        
        let mut current = self.root.as_mut().unwrap();
        let mut parent_ptr: *mut BNode = current.as_mut();
        
        loop {
            if key == current.key {
                return Err("ERROR: Key already exists".to_string());
            } else if key < current.key {
                if current.left.is_none() {
                    // Вставляем влево
                    current.left = Some(new_node);
                    unsafe {
                        if let Some(left) = current.left.as_mut() {
                            left.parent = Some(parent_ptr);
                        }
                    }
                    break;
                } else {
                    current = current.left.as_mut().unwrap();
                    parent_ptr = current.as_mut();
                }
            } else {
                if current.right.is_none() {
                    // Вставляем вправо
                    current.right = Some(new_node);
                    unsafe {
                        if let Some(right) = current.right.as_mut() {
                            right.parent = Some(parent_ptr);
                        }
                    }
                    break;
                } else {
                    current = current.right.as_mut().unwrap();
                    parent_ptr = current.as_mut();
                }
            }
        }
        
        Ok(())
    }
    
    // Поиск узла
    pub fn find_node(&self, key: i32) -> Option<&BNode> {
        self.find_node_rec(&self.root, key)
    }
    
    fn find_node_rec<'a>(&'a self, node: &'a Option<Box<BNode>>, key: i32) -> Option<&'a BNode> {
        match node {
            Some(n) => {
                if n.key == key {
                    Some(n)
                } else if key < n.key {
                    self.find_node_rec(&n.left, key)
                } else {
                    self.find_node_rec(&n.right, key)
                }
            }
            None => None,
        }
    }
    
    // Удаление узла
    pub fn delete_node(&mut self, key: i32) -> Result<(), String> {
        // Поиск узла для удаления
        let node_to_delete = self.find_node_mut(key)
            .ok_or_else(|| "ERROR: Key not found".to_string())?;
        
        // Определяем тип узла
        let has_left = node_to_delete.left.is_some();
        let has_right = node_to_delete.right.is_some();
        
        match (has_left, has_right) {
            // Лист
            (false, false) => {
                self.delete_leaf(node_to_delete);
            }
            // Один потомок слева
            (true, false) => {
                self.delete_with_single_child(node_to_delete, true)?;
            }
            // Один потомок справа
            (false, true) => {
                self.delete_with_single_child(node_to_delete, false)?;
            }
            // Два потомка
            (true, true) => {
                self.delete_with_two_children(node_to_delete)?;
            }
        }
        
        Ok(())
    }
    
    fn delete_leaf(&mut self, node: &mut BNode) {
        // Здесь нужно найти родителя и обнулить ссылку
        // Упрощенная версия - перестроим дерево заново
        let keys = self.collect_keys();
        let new_keys: Vec<i32> = keys.into_iter()
            .filter(|&k| k != node.key)
            .collect();
        
        self.rebuild_tree(new_keys);
    }
    
    fn delete_with_single_child(&mut self, node: &mut BNode, is_left: bool) -> Result<(), String> {
        let child = if is_left {
            node.left.take()
        } else {
            node.right.take()
        };
        
        if let Some(mut child_node) = child {
            child_node.parent = node.parent;
            // Заменяем текущий узел на потомка
            // Упрощенная реализация
            let keys = self.collect_keys();
            let new_keys: Vec<i32> = keys.into_iter()
                .filter(|&k| k != node.key)
                .collect();
            
            self.rebuild_tree(new_keys);
        }
        
        Ok(())
    }
    
    fn delete_with_two_children(&mut self, node: &mut BNode) -> Result<(), String> {
        // Находим преемника (минимальный в правом поддереве)
        let successor_key = self.find_min_key(&node.right);
        
        // Сохраняем ключи кроме удаляемого
        let keys = self.collect_keys();
        let mut new_keys: Vec<i32> = keys.into_iter()
            .filter(|&k| k != node.key)
            .collect();
        
        // Добавляем ключ преемника (если его еще нет)
        if !new_keys.contains(&successor_key) {
            new_keys.push(successor_key);
        }
        
        self.rebuild_tree(new_keys);
        Ok(())
    }
    
    fn find_min_key(&self, node: &Option<Box<BNode>>) -> i32 {
        match node {
            Some(n) => {
                if n.left.is_none() {
                    n.key
                } else {
                    self.find_min_key(&n.left)
                }
            }
            None => i32::MAX,
        }
    }
    
    fn find_node_mut(&mut self, key: i32) -> Option<&mut BNode> {
        self.find_node_mut_rec(&mut self.root, key)
    }
    
    fn find_node_mut_rec<'a>(&'a mut self, node: &'a mut Option<Box<BNode>>, key: i32) -> Option<&'a mut BNode> {
        match node {
            Some(n) => {
                if n.key == key {
                    Some(n)
                } else if key < n.key {
                    self.find_node_mut_rec(&mut n.left, key)
                } else {
                    self.find_node_mut_rec(&mut n.right, key)
                }
            }
            None => None,
        }
    }
    
    // Сбор всех ключей в векторе (pre-order)
    fn collect_keys(&self) -> Vec<i32> {
        let mut keys = Vec::new();
        self.collect_keys_rec(&self.root, &mut keys);
        keys
    }
    
    fn collect_keys_rec(&self, node: &Option<Box<BNode>>, keys: &mut Vec<i32>) {
        if let Some(n) = node {
            keys.push(n.key);
            self.collect_keys_rec(&n.left, keys);
            self.collect_keys_rec(&n.right, keys);
        }
    }
    
    // Перестроение дерева из вектора ключей
    fn rebuild_tree(&mut self, keys: Vec<i32>) {
        self.root = None;
        for key in keys {
            let _ = self.add_node(key); // Игнорируем ошибки дубликатов
        }
    }
    
    // Проверка на полноту дерева
    pub fn is_full(&self) -> bool {
        self.is_full_rec(&self.root)
    }
    
    fn is_full_rec(&self, node: &Option<Box<BNode>>) -> bool {
        match node {
            Some(n) => {
                let left_full = self.is_full_rec(&n.left);
                let right_full = self.is_full_rec(&n.right);
                
                // Узел полный если:
                // 1. Оба потомка есть или оба отсутствуют
                // 2. И левое и правое поддеревья полные
                let children_match = n.left.is_some() == n.right.is_some();
                children_match && left_full && right_full
            }
            None => true, // Пустое дерево считается полным
        }
    }
    
    // Обходы дерева
    pub fn pre_order(&self) -> Vec<i32> {
        let mut result = Vec::new();
        self.pre_order_rec(&self.root, &mut result);
        result
    }
    
    fn pre_order_rec(&self, node: &Option<Box<BNode>>, result: &mut Vec<i32>) {
        if let Some(n) = node {
            result.push(n.key);
            self.pre_order_rec(&n.left, result);
            self.pre_order_rec(&n.right, result);
        }
    }
    
    pub fn in_order(&self) -> Vec<i32> {
        let mut result = Vec::new();
        self.in_order_rec(&self.root, &mut result);
        result
    }
    
    fn in_order_rec(&self, node: &Option<Box<BNode>>, result: &mut Vec<i32>) {
        if let Some(n) = node {
            self.in_order_rec(&n.left, result);
            result.push(n.key);
            self.in_order_rec(&n.right, result);
        }
    }
    
    pub fn post_order(&self) -> Vec<i32> {
        let mut result = Vec::new();
        self.post_order_rec(&self.root, &mut result);
        result
    }
    
    fn post_order_rec(&self, node: &Option<Box<BNode>>, result: &mut Vec<i32>) {
        if let Some(n) = node {
            self.post_order_rec(&n.left, result);
            self.post_order_rec(&n.right, result);
            result.push(n.key);
        }
    }
    
    // BFS обход
    pub fn bfs(&self) -> Vec<i32> {
        let mut result = Vec::new();
        let mut queue = Vec::new();
        
        if let Some(root) = &self.root {
            queue.push(root);
        }
        
        while let Some(node) = queue.pop() {
            result.push(node.key);
            
            if let Some(left) = &node.left {
                queue.insert(0, left); // Добавляем в начало для правильного порядка
            }
            
            if let Some(right) = &node.right {
                queue.insert(0, right);
            }
        }
        
        result
    }
    
    // Поиск преемника (in-order successor)
    pub fn find_successor(&self, key: i32) -> Option<i32> {
        let node = self.find_node(key)?;
        
        // Если есть правое поддерево
        if let Some(right) = &node.right {
            return Some(self.find_min_key(&Some(right.clone())));
        }
        
        // Иначе идем вверх по родителям
        // Упрощенная реализация - ищем следующий больший ключ в дереве
        let all_keys = self.in_order();
        for k in all_keys {
            if k > key {
                return Some(k);
            }
        }
        
        None
    }
    
    // Поиск предшественника (in-order predecessor)
    pub fn find_predecessor(&self, key: i32) -> Option<i32> {
        let all_keys = self.in_order();
        let mut prev = None;
        
        for k in all_keys {
            if k >= key {
                break;
            }
            prev = Some(k);
        }
        
        prev
    }
}

impl Structure for BTree {
    fn serialize(&self) -> String {
        // Формат: T name count key1 key2 ... (pre-order)
        let keys = self.pre_order();
        format!("T {} {} {}", self.name, keys.len(), 
                keys.iter().map(|k| k.to_string()).collect::<Vec<_>>().join(" "))
    }
    
    fn deserialize(&mut self, data: &str) {
        let parts: Vec<&str> = data.split_whitespace().collect();
        if parts.len() < 3 || parts[0] != "T" {
            return;
        }
        
        self.name = parts[1].to_string();
        let count: usize = parts[2].parse().unwrap_or(0);
        
        // Очищаем дерево
        self.root = None;
        
        // Добавляем ключи в порядке pre-order
        for i in 0..count {
            if let Some(key_str) = parts.get(3 + i) {
                if let Ok(key) = key_str.parse::<i32>() {
                    let _ = self.add_node(key); // Игнорируем ошибки дубликатов
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

// Реализация AsAny для downcast
use crate::AsAny;
