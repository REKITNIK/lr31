// StructureFactory.java
package factory;

import structures.*;

public class StructureFactory {
    public static Structure createStructure(char type) {
        return createStructure(type, "default");
    }
    
    public static Structure createStructure(char type, String name) {
        switch (type) {
            case 'M':
                return new Array(name);
            case 'F':
                return new ForwardList(name);
            case 'L':
                return new DoubleList(name);
            case 'S':
                return new Stack(name);
            case 'Q':
                return new Queue(name);
            case 'T':
                return new BinaryTree(name);
            default:
                return null;
        }
    }
    
    public static Structure createStructureFromSerialized(String data) {
        if (data == null || data.length() < 3) {
            return null;
        }
        
        char type = data.charAt(0);
        Structure structure = createStructure(type);
        
        if (structure != null) {
            structure.deserialize(data);
        }
        
        return structure;
    }
}
