// factory.rs
use crate::{Array, ForwardList, DFList, Stack, Queue, BTree, Structure};

pub struct Factory;

impl Factory {
    pub fn create_from_char(c: char) -> Option<Box<dyn Structure>> {
        match c {
            'M' => Some(Box::new(Array::new(10))),
            'F' => Some(Box::new(ForwardList::new())),
            'L' => Some(Box::new(DFList::new())),
            'S' => Some(Box::new(Stack::new())),
            'Q' => Some(Box::new(Queue::new())),
            'T' => Some(Box::new(BTree::new())),
            _ => None,
        }
    }
}
