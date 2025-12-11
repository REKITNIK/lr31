package structures;

import exceptions.IndexOutOfRangeException;

public class ForwardList extends Structure {

    // Убрали static, если нужно использовать в методах removeBefore/removeAfter
    private class Node {
        String data;
        Node next;

        Node(String data) {
            this.data = data;
            this.next = null;
        }
    }
    
    private Node head;
    private Node tail;
    private int size;

    public ForwardList() {
        super();
        head = null;
        tail = null;
        size = 0;
    }

    public ForwardList(String name) {
        super(name);
        head = null;
        tail = null;
        size = 0;
    }

    public void pushFront(String value) {
        Node newNode = new Node(value);

        if (head == null) {
            head = newNode;
            tail = newNode;
        } else {
            newNode.next = head;
            head = newNode;
        }
        size++;
    }

    public void pushBack(String value) {
        Node newNode = new Node(value);

        if (tail == null) {
            head = newNode;
            tail = newNode;
        } else {
            tail.next = newNode;
            tail = newNode;
        }
        size++;
    }

    public void insertBefore(int position, String value) throws IndexOutOfRangeException {
        if (position < 0 || position > size) {
            throw new IndexOutOfRangeException("Position out of range: " + position);
        }

        if (position == 0) {
            pushFront(value);
        } else if (position == size) {
            pushBack(value);
        } else {
            Node prev = getNodeAt(position - 1);
            Node newNode = new Node(value);
            newNode.next = prev.next;
            prev.next = newNode;
            size++;
        }
    }

    public void insertAfter(int position, String value) throws IndexOutOfRangeException {
        if (position < 0 || position >= size) {
            throw new IndexOutOfRangeException("Position out of range: " + position);
        }

        if (position == size - 1) {
            pushBack(value);
        } else {
            Node current = getNodeAt(position);
            Node newNode = new Node(value);
            newNode.next = current.next;
            current.next = newNode;
            size++;
        }
    }

    public String popFront() throws IndexOutOfRangeException {
        if (head == null) {
            throw new IndexOutOfRangeException("List is empty");
        }

        String value = head.data;
        head = head.next;

        if (head == null) {
            tail = null;
        }

        size--;
        return value;
    }

    public String popBack() throws IndexOutOfRangeException {
        if (tail == null) {
            throw new IndexOutOfRangeException("List is empty");
        }

        if (head == tail) {
            String value = head.data;
            head = null;
            tail = null;
            size--;
            return value;
        }

        Node current = head;
        while (current.next != tail) {
            current = current.next;
        }

        String value = tail.data;
        current.next = null;
        tail = current;
        size--;

        return value;
    }

    public String get(int position) throws IndexOutOfRangeException {
        return getNodeAt(position).data;
    }

    private Node getNodeAt(int position) throws IndexOutOfRangeException {
        if (position < 0 || position >= size) {
            throw new IndexOutOfRangeException("Position out of range: " + position);
        }

        Node current = head;
        for (int i = 0; i < position; i++) {
            current = current.next;
        }

        return current;
    }

    public void removeBefore(Node targetNode) throws IndexOutOfRangeException {
        if (targetNode == null) {
            throw new IndexOutOfRangeException("Target node is null");
        }

        if (head == null || targetNode == head) {
            throw new IndexOutOfRangeException("Cannot remove before head");
        }

        if (head.next == targetNode) {
            popFront();
            return;
        }

        Node current = head;
        while (current != null && current.next != null && current.next.next != targetNode) {
            current = current.next;
        }

        if (current == null || current.next == null) {
            throw new IndexOutOfRangeException("Cannot find element to remove");
        }

        Node toDelete = current.next;
        current.next = targetNode;
        size--;
    }

    public void removeAfter(Node prevNode) throws IndexOutOfRangeException {
        if (prevNode == null || prevNode.next == null) {
            throw new IndexOutOfRangeException("Node has no next element");
        }

        Node toDelete = prevNode.next;
        prevNode.next = toDelete.next;

        if (toDelete == tail) {
            tail = prevNode;
        }

        size--;
    }

    public Node findByValue(String value) {
        Node current = head;
        while (current != null) {
            if (current.data.equals(value)) {
                return current;
            }
            current = current.next;
        }
        return null;
    }

    public int getSize() {
        return size;
    }

    @Override
    public String serialize() {
        StringBuilder sb = new StringBuilder();
        sb.append("F ").append(name).append(" ").append(size);

        Node current = head;
        while (current != null) {
            sb.append(" ").append(current.data);
            current = current.next;
        }

        return sb.toString();
    }

    @Override
    public void deserialize(String data) {
        String[] parts = data.split(" ");
        if (parts.length < 3) {
            return;
        }

        this.name = parts[1];
        int count = Integer.parseInt(parts[2]);

        // Clear existing list
        head = null;
        tail = null;
        size = 0;

        for (int i = 0; i < count && (i + 3) < parts.length; i++) {
            pushBack(parts[i + 3]);
        }
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("ForwardList \"").append(name).append("\" (size: ").append(size).append("): [");

        Node current = head;
        while (current != null) {
            if (current != head) {
                sb.append(", ");
            }
            sb.append(current.data);
            current = current.next;
        }

        sb.append("]");
        return sb.toString();
    }
}