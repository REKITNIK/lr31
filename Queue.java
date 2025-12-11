// Queue.java
package structures;

import exceptions.QueueException;

public class Queue extends Structure {
    private ForwardList list;
    private int size;
    private static final int MAX_SIZE = 1000;
    
    public Queue() {
        super();
        list = new ForwardList();
        size = 0;
    }
    
    public Queue(String name) {
        super(name);
        list = new ForwardList();
        size = 0;
    }
    
    public void enqueue(String value) throws QueueException {
        if (size >= MAX_SIZE) {
            throw new QueueException("Queue overflow");
        }
        
        list.pushBack(value);
        size++;
    }
    
    public String dequeue() throws QueueException {
        if (size == 0) {
            throw new QueueException("Queue is empty");
        }
        
        try {
            String value = list.popFront();
            size--;
            return value;
        } catch (IndexOutOfRangeException e) {
            throw new QueueException("Queue error: " + e.getMessage());
        }
    }
    
    public String front() throws QueueException {
        if (size == 0) {
            throw new QueueException("Queue is empty");
        }
        
        try {
            return list.get(0);
        } catch (IndexOutOfRangeException e) {
            throw new QueueException("Queue error: " + e.getMessage());
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
        sb.append("Q ").append(name).append(" ").append(size);
        
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
        
        // Clear queue
        list = new ForwardList();
        size = 0;
        
        // Reconstruct queue
        for (int i = 0; i < count && (i + 3) < parts.length; i++) {
            try {
                enqueue(parts[i + 3]);
            } catch (QueueException e) {
                // Should not happen during deserialization
            }
        }
    }
    
    @Override
    public String toString() {
        return "Queue \"" + name + "\" (size: " + size + "): " + list.toString();
    }
}
