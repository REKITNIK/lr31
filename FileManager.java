// FileManager.java
package io;

import structures.*;
import java.io.*;
import java.util.*;

public class FileManager {
    
    public static void ensureDirectoryExists(String filename) {
        File file = new File(filename);
        File parent = file.getParentFile();
        
        if (parent != null && !parent.exists()) {
            parent.mkdirs();
        }
    }
    
    public static boolean fileExists(String filename) {
        File file = new File(filename);
        return file.exists();
    }
    
    public static String getStructureType(String filename) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line = reader.readLine();
            if (line == null || line.isEmpty()) {
                throw new IOException("Empty file");
            }
            
            String[] parts = line.split(" ", 2);
            if (parts.length == 0) {
                throw new IOException("Invalid file format");
            }
            
            String typeChar = parts[0];
            switch (typeChar) {
                case "M": return "Array";
                case "F": return "ForwardList";
                case "L": return "DoubleList";
                case "S": return "Stack";
                case "Q": return "Queue";
                case "T": return "BinaryTree";
                default: throw new IOException("Unknown type: " + typeChar);
            }
        }
    }
    
    public static void saveDatabaseToFile(String filename, Map<String, Structure> database) throws IOException {
        ensureDirectoryExists(filename);
        
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename))) {
            for (Structure structure : database.values()) {
                writer.write(structure.serialize());
                writer.newLine();
            }
        }
    }
    
    public static Map<String, Structure> loadDatabaseFromFile(String filename) throws IOException {
        Map<String, Structure> database = new HashMap<>();
        
        if (!fileExists(filename)) {
            return database;
        }
        
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line;
            while ((line = reader.readLine()) != null) {
                if (line.trim().isEmpty()) {
                    continue;
                }
                
                Structure structure = StructureFactory.createStructureFromSerialized(line);
                if (structure != null) {
                    database.put(structure.getName(), structure);
                }
            }
        }
        
        return database;
    }
    
    public static void saveStructureToFile(String filename, Structure structure) throws IOException {
        ensureDirectoryExists(filename);
        
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename))) {
            writer.write(structure.serialize());
        }
    }
    
    public static Structure loadStructureFromFile(String filename) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line = reader.readLine();
            if (line == null || line.isEmpty()) {
                throw new IOException("Empty file");
            }
            
            return StructureFactory.createStructureFromSerialized(line);
        }
    }
}
