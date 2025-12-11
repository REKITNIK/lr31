// src/lib.rs
pub mod structure;
pub mod array;
pub mod forward_list;
pub mod double_list;
pub mod stack;
pub mod queue;
pub mod binary_tree;
pub mod factory;
pub mod manager;
pub mod file_io;  // Добавлено

// Публичный API
pub use structure::{Structure, AsAny};
pub use array::Array;
pub use forward_list::ForwardList;
pub use double_list::DFList;
pub use stack::Stack;
pub use queue::Queue;
pub use binary_tree::{BTree, BNode};
pub use factory::Factory;
pub use manager::StructureManager;
pub use file_io::FileIO;  // Добавлено
