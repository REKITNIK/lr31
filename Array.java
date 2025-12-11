// Array.java
package structures;

import exceptions.IndexOutOfRangeException;

public class Array extends Structure {
    private String[] data;
    private int length;
    private int capacity;
    private static final int INITIAL_CAPACITY = 10;
    
    public Array() {
        super();
        this.capacity = INITIAL_CAPACITY;
        this.data = new String[capacity];
        this.length = 0;
    }
    
    public Array(String name) {
        super(name);
        this.capacity = INITIAL_CAPACITY;
        this.data = new String[capacity];
        this.length = 0;
    }
    
    public Array(int initialSize) {
        super();
        this.capacity = Math.max(initialSize, 1);
        this.data = new String[capacity];
        this.length = 0;
    }
    
    private void extend() {
        int newCapacity = capacity * 2;
        String[] newData = new String[newCapacity];
        System.arraycopy(data, 0, newData, 0, length);
        data = newData;
        capacity = newCapacity;
    }
    
    public String get(int index) throws IndexOutOfRangeException {
        if (index < 0 || index >= length) {
            throw new IndexOutOfRangeException("Index out of range: " + index);
        }
        return data[index];
    }
    
    public void set(int index, String value) throws IndexOutOfRangeException {
        if (index < 0 || index >= length) {
            throw new IndexOutOfRangeException("Index out of range: " + index);
        }
        data[index] = value;
    }
    
    public void delete(int index) throws IndexOutOfRangeException {
        if (index < 0 || index >= length) {
            throw new IndexOutOfRangeException("Index out of range: " + index);
        }
        
        for (int i = index; i < length - 1; i++) {
            data[i] = data[i + 1];
        }
        data[length - 1] = null;
        length--;
    }
    
    public void insert(int index, String value) throws IndexOutOfRangeException {
        if (index < 0 || index > length) {
            throw new IndexOutOfRangeException("Index out of range: " + index);
        }
        
        if (length >= capacity) {
            extend();
        }
        
        for (int i = length; i > index; i--) {
            data[i] = data[i - 1];
        }
        
        data[index] = value;
        length++;
    }
    
    public void push(String value) {
        insert(length, value);
    }
    
    public int getLength() {
        return length;
    }
    
    @Override
    public String serialize() {
        StringBuilder sb = new StringBuilder();
        sb.append("M ").append(name).append(" ").append(length);
        
        for (int i = 0; i < length; i++) {
            sb.append(" ").append(data[i]);
        }
        
        return sb.toString();
    }
    
    @Override
    public void deserialize(String data) {
        String[] parts = data.split(" ");
        if (parts.length < 3) return;
        
        this.name = parts[1];
        int count = Integer.parseInt(parts[2]);
        
        this.length = 0;
        this.capacity = Math.max(count, INITIAL_CAPACITY);
        this.data = new String[capacity];
        
        for (int i = 0; i < count && (i + 3) < parts.length; i++) {
            this.data[i] = parts[i + 3];
            this.length++;
        }
    }
    
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Array \"").append(name).append("\" (length: ").append(length).append("): [");
        
        for (int i = 0; i < length; i++) {
            if (i > 0) sb.append(", ");
            sb.append(data[i]);
        }
        
        sb.append("]");
        return sb.toString();
    }
}
