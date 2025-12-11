// StructureManager.java (полная версия)
package main;

import structures.*;
import factory.StructureFactory;
import io.FileManager;
import exceptions.*;
import java.util.*;
import java.util.function.Consumer;

public class StructureManager {
    private String currentFilename;
    private Map<String, Structure> database;
    
    public StructureManager() {
        this.database = new HashMap<>();
    }
    
    public void setFilename(String filename) {
        this.currentFilename = filename;
    }
    
    public String getFilename() {
        return currentFilename;
    }
    
    public void cleanup() {
        database.clear();
    }
    
    public void saveCurrentStructure() throws Exception {
        if (currentFilename == null || currentFilename.isEmpty()) {
            return;
        }
        
        FileManager.saveDatabaseToFile(currentFilename, database);
    }
    
    public boolean loadStructuresFromFile(String filename) {
        try {
            cleanup();
            database = FileManager.loadDatabaseFromFile(filename);
            currentFilename = filename;
            return true;
        } catch (Exception e) {
            System.err.println("ERROR 10: Unknown command");
            return false;
        }
    }
    
    @SuppressWarnings("unchecked")
    public <T extends Structure> T get(String name, Class<T> type) {
        Structure structure = database.get(name);
        if (structure == null) {
            return null;
        }
        return type.isInstance(structure) ? type.cast(structure) : null;
    }
    
    public void printStructure(String name) {
        Structure structure = database.get(name);
        if (structure == null) {
            System.err.println("ERROR 20: Structure not found");
            return;
        }
        
        System.out.println(structure);
    }
    
    private int safeParseInt(String str) {
        try {
            return Integer.parseInt(str);
        } catch (NumberFormatException e) {
            System.err.println("ERROR 30: Invalid index/argument");
            System.exit(1);
            return -1;
        }
    }
    
    // ===== M Commands (Array) =====
    public void handleMCommand(String[] tokens) {
        try {
            if (tokens[0].equals("MCREATE")) {
                String name = tokens.length > 1 ? tokens[1] : "default";
                if (database.containsKey(name)) {
                    System.err.println("ERROR 21: Structure already exists");
                    System.exit(1);
                }
                Array array = new Array(name);
                database.put(name, array);
                return;
            }
            
            String name = "default";
            int paramStart = 1;
            if (tokens.length > 1 && database.containsKey(tokens[1])) {
                name = tokens[1];
                paramStart = 2;
            }
            
            Array array = get(name, Array.class);
            if (array == null) {
                System.err.println("ERROR 20: Structure not found");
                System.exit(1);
            }
            
            switch (tokens[0]) {
                case "MPUSH":
                    if (tokens.length <= paramStart) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    array.push(tokens[paramStart]);
                    break;
                    
                case "MPUSHAT":
                    if (tokens.length <= paramStart + 1) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    int idx = safeParseInt(tokens[paramStart + 1]);
                    array.insert(idx, tokens[paramStart]);
                    break;
                    
                case "MGET":
                    if (tokens.length <= paramStart) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    idx = safeParseInt(tokens[paramStart]);
                    System.out.println(array.get(idx));
                    break;
                    
                case "MDEL":
                    if (tokens.length <= paramStart) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    idx = safeParseInt(tokens[paramStart]);
                    array.delete(idx);
                    break;
                    
                case "MSET":
                    if (tokens.length <= paramStart + 1) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    idx = safeParseInt(tokens[paramStart]);
                    array.set(idx, tokens[paramStart + 1]);
                    break;
                    
                case "MLEN":
                    System.out.println(array.getLength());
                    break;
                    
                default:
                    System.err.println("ERROR 10: Unknown command");
                    System.exit(1);
                    break;
            }
        } catch (Exception e) {
            System.err.println("ERROR 30: Invalid index/argument");
            System.exit(1);
        }
    }
    
    // ===== F Commands (ForwardList) =====
    public void handleFCommand(String[] tokens) {
        try {
            if (tokens[0].equals("FCREATE")) {
                String name = tokens.length > 1 ? tokens[1] : "default";
                if (database.containsKey(name)) {
                    System.err.println("ERROR 21: Structure already exists");
                    System.exit(1);
                }
                ForwardList fl = new ForwardList(name);
                database.put(name, fl);
                return;
            }
            
            String name = "default";
            int paramStart = 1;
            if (tokens.length > 1 && database.containsKey(tokens[1])) {
                name = tokens[1];
                paramStart = 2;
            }
            
            ForwardList fl = get(name, ForwardList.class);
            if (fl == null && !tokens[0].equals("FCREATE")) {
                fl = new ForwardList(name);
                database.put(name, fl);
            }
            
            if (fl == null) {
                System.err.println("ERROR 20: Structure not found");
                System.exit(1);
            }
            
            switch (tokens[0]) {
                case "FPUSH":
                    if (tokens.length < paramStart + 2) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    String value = tokens[paramStart];
                    int mode = safeParseInt(tokens[paramStart + 1]);
                    
                    if (mode == 0) {
                        fl.pushFront(value);
                    } else if (mode == 1) {
                        fl.pushBack(value);
                    } else if (mode == 2) {
                        if (fl.getSize() > 0) {
                            fl.insertAfter(0, value);
                        } else {
                            fl.pushFront(value);
                        }
                    } else if (mode == 3) {
                        if (fl.getSize() > 0) {
                            fl.insertBefore(fl.getSize() - 1, value);
                        } else {
                            fl.pushFront(value);
                        }
                    } else {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    break;
                    
                case "FDEL":
                    if (tokens.length < paramStart + 1) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    mode = safeParseInt(tokens[paramStart]);
                    
                    if (mode == 0) {
                        fl.popFront();
                    } else if (mode == 1) {
                        fl.popBack();
                    } else if (mode == 2) {
                        // Удалить после головы (если есть)
                        if (fl.getSize() > 1) {
                            try {
                                String first = fl.get(0);
                                fl.insertAfter(0, "temp");
                                fl.removeByValue("temp");
                            } catch (Exception e) {
                                // ignore
                            }
                        }
                    } else if (mode == 3) {
                        // Удалить перед хвостом
                        if (fl.getSize() > 1) {
                            try {
                                fl.popBack();
                                String last = fl.get(fl.getSize() - 1);
                                fl.popBack();
                                fl.pushBack(last);
                            } catch (Exception e) {
                                // ignore
                            }
                        }
                    } else {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    break;
                    
                case "FDELVAL":
                    if (tokens.length < paramStart + 1) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    value = tokens[paramStart];
                    if (!fl.removeByValue(value)) {
                        System.err.println("ERROR 20: Structure not found");
                        System.exit(1);
                    }
                    break;
                    
                case "FSEARCH":
                    if (tokens.length < paramStart + 1) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    value = tokens[paramStart];
                    System.out.println(fl.findByValue(value) != null ? "TRUE" : "FALSE");
                    break;
                    
                case "FGET":
                    if (tokens.length < paramStart + 1) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    int idx = safeParseInt(tokens[paramStart]);
                    System.out.println(fl.get(idx));
                    break;
                    
                case "FLEN":
                    System.out.println(fl.getSize());
                    break;
                    
                default:
                    System.err.println("ERROR 10: Unknown command");
                    System.exit(1);
                    break;
            }
        } catch (Exception e) {
            System.err.println("ERROR 30: Invalid index/argument");
            System.exit(1);
        }
    }
    
    // ===== L Commands (DoubleList) =====
    public void handleLCommand(String[] tokens) {
        try {
            if (tokens[0].equals("LCREATE")) {
                String name = tokens.length > 1 ? tokens[1] : "default";
                if (database.containsKey(name)) {
                    System.err.println("ERROR 21: Structure already exists");
                    System.exit(1);
                }
                DoubleList dl = new DoubleList(name);
                database.put(name, dl);
                return;
            }
            
            String name = "default";
            int paramStart = 1;
            if (tokens.length > 1 && database.containsKey(tokens[1])) {
                name = tokens[1];
                paramStart = 2;
            }
            
            DoubleList dl = get(name, DoubleList.class);
            if (dl == null && !tokens[0].equals("LCREATE")) {
                dl = new DoubleList(name);
                database.put(name, dl);
            }
            
            if (dl == null) {
                System.err.println("ERROR 20: Structure not found");
                System.exit(1);
            }
            
            switch (tokens[0]) {
                case "LPUSH":
                    if (tokens.length < paramStart + 2) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    String value = tokens[paramStart];
                    int mode = safeParseInt(tokens[paramStart + 1]);
                    
                    if (mode == 0) {
                        dl.addHead(value);
                    } else if (mode == 1) {
                        dl.addTail(value);
                    } else if (mode == 2) {
                        if (dl.getSize() > 0) {
                            dl.addAfter(0, value);
                        } else {
                            dl.addHead(value);
                        }
                    } else if (mode == 3) {
                        if (dl.getSize() > 0) {
                            dl.addBefore(dl.getSize() - 1, value);
                        } else {
                            dl.addHead(value);
                        }
                    } else {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    break;
                    
                case "LDEL":
                    if (tokens.length < paramStart + 1) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    mode = safeParseInt(tokens[paramStart]);
                    
                    if (mode == 0) {
                        dl.deleteHead();
                    } else if (mode == 1) {
                        dl.deleteTail();
                    } else if (mode == 2) {
                        // Удалить после головы
                        if (dl.getSize() > 1) {
                            dl.deleteAt(1);
                        }
                    } else if (mode == 3) {
                        // Удалить перед хвостом
                        if (dl.getSize() > 1) {
                            dl.deleteAt(dl.getSize() - 2);
                        }
                    } else {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    break;
                    
                case "LGET":
                    if (tokens.length < paramStart + 1) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    int idx = safeParseInt(tokens[paramStart]);
                    System.out.println(dl.get(idx));
                    break;
                    
                case "LSEARCH":
                    if (tokens.length < paramStart + 1) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    value = tokens[paramStart];
                    System.out.println(dl.findByValue(value) != null ? "TRUE" : "FALSE");
                    break;
                    
                case "LDELVAL":
                    if (tokens.length < paramStart + 1) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    value = tokens[paramStart];
                    if (!dl.deleteByValue(value)) {
                        System.err.println("ERROR 20: Structure not found");
                        System.exit(1);
                    }
                    break;
                    
                case "LLEN":
                    System.out.println(dl.getSize());
                    break;
                    
                default:
                    System.err.println("ERROR 10: Unknown command");
                    System.exit(1);
                    break;
            }
        } catch (Exception e) {
            System.err.println("ERROR 30: Invalid index/argument");
            System.exit(1);
        }
    }
    
    // ===== S Commands (Stack) =====
    public void handleSCommand(String[] tokens) {
        try {
            if (tokens[0].equals("SCREATE")) {
                String name = tokens.length > 1 ? tokens[1] : "default";
                if (database.containsKey(name)) {
                    System.err.println("ERROR 21: Structure already exists");
                    System.exit(1);
                }
                Stack stack = new Stack(name);
                database.put(name, stack);
                return;
            }
            
            String name = "default";
            int paramStart = 1;
            if (tokens.length > 1 && database.containsKey(tokens[1])) {
                name = tokens[1];
                paramStart = 2;
            }
            
            Stack stack = get(name, Stack.class);
            if (stack == null && !tokens[0].equals("SCREATE")) {
                stack = new Stack(name);
                database.put(name, stack);
            }
            
            if (stack == null) {
                System.err.println("ERROR 20: Structure not found");
                System.exit(1);
            }
            
            switch (tokens[0]) {
                case "SPUSH":
                    if (tokens.length <= paramStart) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    stack.push(tokens[paramStart]);
                    break;
                    
                case "SPOP":
                    try {
                        System.out.println(stack.pop());
                    } catch (StackException e) {
                        System.err.println("ERROR 40: Empty structure");
                        System.exit(1);
                    }
                    break;
                    
                case "SLEN":
                    System.out.println(stack.getSize());
                    break;
                    
                default:
                    System.err.println("ERROR 10: Unknown command");
                    System.exit(1);
                    break;
            }
        } catch (Exception e) {
            System.err.println("ERROR 40: Empty structure");
            System.exit(1);
        }
    }
    
    // ===== Q Commands (Queue) =====
    public void handleQCommand(String[] tokens) {
        try {
            if (tokens[0].equals("QCREATE")) {
                String name = tokens.length > 1 ? tokens[1] : "default";
                if (database.containsKey(name)) {
                    System.err.println("ERROR 21: Structure already exists");
                    System.exit(1);
                }
                Queue queue = new Queue(name);
                database.put(name, queue);
                return;
            }
            
            String name = "default";
            int paramStart = 1;
            if (tokens.length > 1 && database.containsKey(tokens[1])) {
                name = tokens[1];
                paramStart = 2;
            }
            
            structures.Queue queue = get(name, structures.Queue.class);
            if (queue == null && !tokens[0].equals("QCREATE")) {
                queue = new structures.Queue(name);
                database.put(name, queue);
            }
            
            if (queue == null) {
                System.err.println("ERROR 20: Structure not found");
                System.exit(1);
            }
            
            switch (tokens[0]) {
                case "QPUSH":
                    if (tokens.length <= paramStart) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    queue.enqueue(tokens[paramStart]);
                    break;
                    
                case "QPOP":
                    try {
                        System.out.println(queue.dequeue());
                    } catch (QueueException e) {
                        System.err.println("ERROR 40: Empty structure");
                        System.exit(1);
                    }
                    break;
                    
                case "QLEN":
                    System.out.println(queue.getSize());
                    break;
                    
                default:
                    System.err.println("ERROR 10: Unknown command");
                    System.exit(1);
                    break;
            }
        } catch (Exception e) {
            System.err.println("ERROR 40: Empty structure");
            System.exit(1);
        }
    }
    
    // ===== T Commands (BinaryTree) =====
    public void handleTCommand(String[] tokens) {
        try {
            if (tokens[0].equals("TCREATE")) {
                String name = tokens.length > 1 ? tokens[1] : "default";
                if (database.containsKey(name)) {
                    System.err.println("ERROR 21: Structure already exists");
                    System.exit(1);
                }
                BinaryTree tree = new BinaryTree(name);
                database.put(name, tree);
                return;
            }
            
            String name = "default";
            int paramStart = 1;
            if (tokens.length > 1 && database.containsKey(tokens[1])) {
                name = tokens[1];
                paramStart = 2;
            }
            
            BinaryTree tree = get(name, BinaryTree.class);
            if (tree == null && !tokens[0].equals("TCREATE")) {
                tree = new BinaryTree(name);
                database.put(name, tree);
            }
            
            if (tree == null) {
                System.err.println("ERROR 20: Structure not found");
                System.exit(1);
            }
            
            switch (tokens[0]) {
                case "TINSERT":
                    if (tokens.length <= paramStart) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    int key = safeParseInt(tokens[paramStart]);
                    tree.add(key);
                    break;
                    
                case "TSEARCH":
                    if (tokens.length <= paramStart) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    key = safeParseInt(tokens[paramStart]);
                    System.out.println(tree.contains(key) ? "TRUE" : "FALSE");
                    break;
                    
                case "TCHECK":
                    System.out.println(tree.isFull() ? "TRUE" : "FALSE");
                    break;
                    
                case "TDEL":
                    if (tokens.length <= paramStart) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    key = safeParseInt(tokens[paramStart]);
                    tree.delete(key);
                    break;
                    
                case "TGET":
                    if (tokens.length <= paramStart) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    String mode = tokens[paramStart];
                    
                    if (tree.contains(-1)) { // Проверка на пустое дерево
                        System.err.println("ERROR 40: Empty structure");
                        System.exit(1);
                    }
                    
                    switch (mode.toUpperCase()) {
                        case "PRE":
                            System.out.println(tree.getTraversal("PRE"));
                            break;
                        case "IN":
                            System.out.println(tree.getTraversal("IN"));
                            break;
                        case "POST":
                            System.out.println(tree.getTraversal("POST"));
                            break;
                        case "BFS":
                            System.out.println(tree.getTraversal("BFS"));
                            break;
                        default:
                            System.err.println("ERROR 10: Unknown command");
                            System.exit(1);
                    }
                    break;
                    
                case "TGETNODES":
                    if (tokens.length <= paramStart + 1) {
                        System.err.println("ERROR 30: Invalid index/argument");
                        System.exit(1);
                    }
                    key = safeParseInt(tokens[paramStart]);
                    mode = tokens[paramStart + 1];
                    
                    BinaryTree.TreeNode result = null;
                    if (mode.equals("PREV")) {
                        result = tree.findPredecessor(key);
                    } else if (mode.equals("NEXT")) {
                        result = tree.findSuccessor(key);
                    } else {
                        System.err.println("ERROR 10: Unknown command");
                        System.exit(1);
                    }
                    
                    if (result == null) {
                        System.out.println();
                    } else {
                        System.out.println(result.key);
                    }
                    break;
                    
                default:
                    System.err.println("ERROR 10: Unknown command");
                    System.exit(1);
                    break;
            }
        } catch (Exception e) {
            System.err.println("ERROR 10: Unknown command");
            System.exit(1);
        }
    }
    
    public void processQuery(String query) {
        if (query == null || query.trim().isEmpty()) {
            System.err.println("ERROR 10: Unknown command");
            System.exit(1);
        }
        
        String[] tokens = query.split("\\s+");
        if (tokens.length == 0) {
            System.err.println("ERROR 10: Unknown command");
            System.exit(1);
        }
        
        String cmd = tokens[0];
        
        if (cmd.equals("PRINT")) {
            if (tokens.length < 2) {
                System.err.println("ERROR 30: Invalid index/argument");
                System.exit(1);
            }
            printStructure(tokens[1]);
            return;
        }
        
        char c = cmd.charAt(0);
        
        switch (c) {
            case 'M':
                handleMCommand(tokens);
                break;
            case 'F':
                handleFCommand(tokens);
                break;
            case 'L':
                handleLCommand(tokens);
                break;
            case 'S':
                handleSCommand(tokens);
                break;
            case 'Q':
                handleQCommand(tokens);
                break;
            case 'T':
                handleTCommand(tokens);
                break;
            default:
                System.err.println("ERROR 10: Unknown command");
                System.exit(1);
                break;
        }
    }
}
