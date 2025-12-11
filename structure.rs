// src/structure.rs

/// Базовый трейт для всех структур данных
pub trait Structure: std::fmt::Debug {
    /// Сериализация структуры в строку
    fn serialize(&self) -> String;
    
    /// Десериализация структуры из строки
    fn deserialize(&mut self, data: &str);
    
    /// Получение имени структуры
    fn name(&self) -> &str;
    
    /// Установка имени структуры
    fn set_name(&mut self, name: String);
}

/// Трейт для downcast (приведение к конкретному типу)
pub trait AsAny {
    /// Приведение к &dyn Any
    fn as_any(&self) -> &dyn std::any::Any;
    
    /// Приведение к &mut dyn Any
    fn as_any_mut(&mut self) -> &mut dyn std::any::Any;
}

// Реализация AsAny для всех типов, реализующих Structure
impl<T: 'static + Structure> AsAny for T {
    fn as_any(&self) -> &dyn std::any::Any {
        self
    }
    
    fn as_any_mut(&mut self) -> &mut dyn std::any::Any {
        self
    }
}

// Реализация для Box<dyn Structure>
impl Structure for Box<dyn Structure> {
    fn serialize(&self) -> String {
        self.as_ref().serialize()
    }
    
    fn deserialize(&mut self, data: &str) {
        self.as_mut().deserialize(data)
    }
    
    fn name(&self) -> &str {
        self.as_ref().name()
    }
    
    fn set_name(&mut self, name: String) {
        self.as_mut().set_name(name)
    }
}
