// Main.java
package main;

public class Main {
    public static void main(String[] args) {
        StructureManager manager = new StructureManager();
        
        // Парсинг аргументов командной строки
        String filename = null;
        String query = null;
        boolean helpRequested = false;
        
        for (int i = 0; i < args.length; i++) {
            if (args[i].equals("--file") && i + 1 < args.length) {
                filename = args[++i];
                manager.setFilename(filename);
            } else if (args[i].equals("--query") && i + 1 < args.length) {
                query = args[++i];
            } else if (args[i].equals("--help")) {
                helpRequested = true;
            }
        }
        
        try {
            // Обработка --help
            if (helpRequested) {
                printHelp();
                return;
            }
            
            // Загрузка
            if (filename != null && !filename.isEmpty()) {
                if (FileManager.fileExists(filename)) {
                    manager.loadStructuresFromFile(filename);
                }
            }
            
            // Выполнение
            if (query != null && !query.isEmpty()) {
                manager.processQuery(query);
            } else {
                System.err.println("ERROR 10: Unknown command");
                System.exit(1);
            }
            
            // Сохранение
            if (filename != null && !filename.isEmpty()) {
                manager.saveCurrentStructure();
            }
        } catch (Exception e) {
            System.err.println("ERROR 10: Unknown command");
            System.exit(1);
        }
    }
    
    private static void printHelp() {
        System.out.println("Usage: java Main --file <path> --query '<COMMAND> <ARGS...>'");
    }
}
