package structures;

import exceptions.IndexOutOfRangeException;

public class DoubleList extends Structure {

    private class Node {
        String data;
        Node prev;
        Node next;

        Node(String data) {
            this.data = data;
            this.prev = null;
            this.next = null;
        }
    }

    private Node head;
    private Node tail;
    private int size;

    public DoubleList() {
        super();
        head = null;
        tail = null;
        size = 0;
    }

    public DoubleList(String name) {
        super(name);
        head = null;
        tail = null;
        size = 0;
    }

    public void addHead(String value) {
        Node newNode = new Node(value);

        if (head == null) {
            head = newNode;
            tail = newNode;
        } else {
            newNode.next = head;
            head.prev = newNode;
            head = newNode;
        }
        size++;
    }

    public void addTail(String value) {
        Node newNode = new Node(value);

        if (tail == null) {
            head = newNode;
            tail = newNode;
        } else {
            newNode.prev = tail;
            tail.next = newNode;
            tail = newNode;
        }
        size++;
    }

    public void addAfter(int index, String value) throws IndexOutOfRangeException {
        if (index < 0 || index >= size) {
            throw new IndexOutOfRangeException("Index out of range: " + index);
        }

        if (index == size - 1) {
            addTail(value);
            return;
        }

        Node current = getNodeAt(index);
        Node newNode = new Node(value);

        newNode.next = current.next;
        newNode.prev = current;

        if (current.next != null) {
            current.next.prev = newNode;
        }
        current.next = newNode;

        size++;
    }

    public void addBefore(int index, String value) throws IndexOutOfRangeException {
        if (index < 0 || index >= size) {
            throw new IndexOutOfRangeException("Index out of range: " + index);
        }

        if (index == 0) {
            addHead(value);
            return;
        }

        Node current = getNodeAt(index);
        Node newNode = new Node(value);

        newNode.prev = current.prev;
        newNode.next = current;

        if (current.prev != null) {
            current.prev.next = newNode;
        } else {
            head = newNode;
        }
        current.prev = newNode;

        size++;
    }

    public String deleteHead() throws IndexOutOfRangeException {
        if (head == null) {
            throw new IndexOutOfRangeException("List is empty");
        }

        String value = head.data;

        if (head == tail) {
            head = null;
            tail = null;
        } else {
            head = head.next;
            head.prev = null;
        }

        size--;
        return value;
    }

    public String deleteTail() throws IndexOutOfRangeException {
        if (tail == null) {
            throw new IndexOutOfRangeException("List is empty");
        }

        String value = tail.data;

        if (head == tail) {
            head = null;
            tail = null;
        } else {
            tail = tail.prev;
            tail.next = null;
        }

        size--;
        return value;
    }

    public void deleteAt(int index) throws IndexOutOfRangeException {
        if (index < 0 || index >= size) {
            throw new IndexOutOfRangeException("Index out of range: " + index);
        }

        if (index == 0) {
            deleteHead();
        } else if (index == size - 1) {
            deleteTail();
        } else {
            Node toDelete = getNodeAt(index);
            toDelete.prev.next = toDelete.next;
            toDelete.next.prev = toDelete.prev;
            size--;
        }
    }

    public boolean deleteByValue(String value) {
        Node current = head;
        int index = 0;

        while (current != null) {
            if (current.data.equals(value)) {
                try {
                    deleteAt(index);
                    return true;
                } catch (IndexOutOfRangeException e) {
                    return false;
                }
            }
            current = current.next;
            index++;
        }
        return false;
    }

    public String get(int index) throws IndexOutOfRangeException {
        return getNodeAt(index).data;
    }

    private Node getNodeAt(int index) throws IndexOutOfRangeException {
        if (index < 0 || index >= size) {
            throw new IndexOutOfRangeException("Index out of range: " + index);
        }

        // Optimize: traverse from head or tail based on index
        if (index <= size / 2) {
            Node current = head;
            for (int i = 0; i < index; i++) {
                current = current.next;
            }
            return current;
        } else {
            Node current = tail;
            for (int i = size - 1; i > index; i--) {
                current = current.prev;
            }
            return current;
        }
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

    public void deleteNodesBeforeIndex(int index) throws IndexOutOfRangeException {
        if (index <= 0) {
            return;
        }
        if (index >= size) {
            // Удалить все узлы
            head = null;
            tail = null;
            size = 0;
            return;
        }

        // Получаем новый начальный узел
        Node newHead = getNodeAt(index);
        if (newHead != null) {
            // Разрываем связь с предыдущими узлами
            newHead.prev = null;
            
            // Освобождаем память предыдущих узлов (опционально, GC сделает это)
            Node current = head;
            while (current != newHead) {
                Node next = current.next;
                current.prev = null;
                current.next = null;
                current = next;
            }
            
            // Устанавливаем новую голову
            head = newHead;
            size -= index;  // Корректируем размер
        }
    }

    public void deleteNodesAfterIndex(int index) throws IndexOutOfRangeException {
        if (index < 0 || index >= size - 1) {
            return;
        }

        // Найти узел по индексу
        Node current = getNodeAt(index);
        if (current == null) {
            return;
        }

        // Удалить узлы после 'current'
        Node toDelete = current.next;
        while (toDelete != null) {
            Node next = toDelete.next;  // Сохраняем следующий узел
            // Очищаем связи узла
            toDelete.prev = null;
            toDelete.next = null;
            size--;
            toDelete = next;  // Переходим к следующему узлу
        }

        current.next = null;
        tail = current;
    }

    public void deleteNodesFromTo(int start, int end) throws IndexOutOfRangeException {
        if (start < 0 || end >= size || start > end) {
            throw new IndexOutOfRangeException("Invalid index range");
        }

        int deleteCount = end - start + 1;
        
        // Если удаляем с начала
        if (start == 0) {
            if (end == size - 1) {
                // Удаляем все узлы
                head = null;
                tail = null;
                size = 0;
            } else {
                // Устанавливаем новую голову
                Node newHead = getNodeAt(end + 1);
                newHead.prev = null;
                
                // Освобождаем удаляемые узлы
                Node current = head;
                while (current != newHead) {
                    Node next = current.next;
                    current.prev = null;
                    current.next = null;
                    current = next;
                }
                
                head = newHead;
                size -= deleteCount;
            }
            return;
        }

        // Если удаляем до конца
        if (end == size - 1) {
            Node newTail = getNodeAt(start - 1);
            newTail.next = null;
            
            // Освобождаем удаляемые узлы
            Node current = newTail.next;
            while (current != null) {
                Node next = current.next;
                current.prev = null;
                current.next = null;
                current = next;
            }
            
            tail = newTail;
            size = start;  // Новый размер = start (индексация с 0)
            return;
        }

        // Общий случай: удаление в середине
        Node beforeStart = getNodeAt(start - 1);
        Node afterEnd = getNodeAt(end + 1);
        
        // Устанавливаем связи
        beforeStart.next = afterEnd;
        afterEnd.prev = beforeStart;
        
        // Освобождаем удаляемые узлы
        Node current = getNodeAt(start);
        for (int i = 0; i < deleteCount && current != afterEnd; i++) {
            Node next = current.next;
            current.prev = null;
            current.next = null;
            current = next;
        }
        
        // Обновляем размер
        size -= deleteCount;
    }

    @Override
    public String serialize() {
        StringBuilder sb = new StringBuilder();
        sb.append("L ").append(name).append(" ").append(size);

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
            addTail(parts[i + 3]);
        }
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("DoubleList \"").append(name).append("\" (size: ").append(size).append("): [");

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
    
    // Дополнительные методы для удобства работы
    
    public boolean isEmpty() {
        return size == 0;
    }
    
    public void clear() {
        // Помогаем сборщику мусора
        Node current = head;
        while (current != null) {
            Node next = current.next;
            current.prev = null;
            current.next = null;
            current = next;
        }
        head = null;
        tail = null;
        size = 0;
    }
    
    public String getFirst() throws IndexOutOfRangeException {
        if (head == null) {
            throw new IndexOutOfRangeException("List is empty");
        }
        return head.data;
    }
    
    public String getLast() throws IndexOutOfRangeException {
        if (tail == null) {
            throw new IndexOutOfRangeException("List is empty");
        }
        return tail.data;
    }
}