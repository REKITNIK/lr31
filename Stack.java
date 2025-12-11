// Stack.java
package structures;

import exceptions.StackException;

public class Stack extends Structure {
    private ForwardList list;
    private int size;
    private static final int MAX_SIZE = 1000;
    
    public Stack() {
        super();
        list = new ForwardList();
        size = 0;
    }
    
    public Stack(String name) {
        super(name);
        list = new ForwardList();
        size = 0;
    }
    
    public void push(String value) throws StackException {
        if (size >= MAX_SIZE) {
            throw new StackException("Stack overflow");
        }
        
        list.pushFront(value);
        size++;
    }
    
    public String pop() throws StackException {
        if (size == 0) {
            throw new StackException("Stack is empty");
        }
        
        try {
            String value = list.popFront();
            size--;
            return value;
        } catch (IndexOutOfRangeException e) {
            throw new StackException("Stack error: " + e.getMessage());
        }
    }
    
    public String peek() throws StackException {
        if (size == 0) {
            throw new StackException("Stack is empty");
        }
        
        try {
            return list.get(0);
        } catch (IndexOutOfRangeException e) {
            throw new StackException("Stack error: " + e.getMessage());
        }
    }
    
    public boolean isEmpty() {
        return size == 0;
    }
    
    public boolean isFull() {
        return size >= MAX_SIZE;
    }
    
    public int getSize() {
        return size;
    }
    
    public void clear() {
        list = new ForwardList();
        size = 0;
    }
    
    @Override
    public String serialize() {
        StringBuilder sb = new StringBuilder();
        sb.append("S ").append(name).append(" ").append(size);
        
        // Stack stores data in ForwardList, need to serialize it
        String listSerialized = list.serialize();
        String[] parts = listSerialized.split(" ", 3);
        if (parts.length >= 3) {
            sb.append(" ").append(parts[2]);
        }
        
        return sb.toString();
    }
    
    @Override
    public void deserialize(String data) {
        String[] parts = data.split(" ");
        if (parts.length < 3) return;
        
        this.name = parts[1];
        int count = Integer.parseInt(parts[2]);
        
        // Clear stack
        list = new ForwardList();
        size = 0;
        
        // Reconstruct stack
        for (int i = 0; i < count && (i + 3) < parts.length; i++) {
            try {
                push(parts[i + 3]);
            } catch (StackException e) {
                // Should not happen during deserialization
            }
        }
    }
    
    @Override
    public String toString() {
        return "Stack \"" + name + "\" (size: " + size + "): " + list.toString();
    }
}
