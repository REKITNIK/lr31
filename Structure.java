// Structure.java
package structures;

public abstract class Structure {
    protected String name;
    
    public Structure() {
        this.name = "default";
    }
    
    public Structure(String name) {
        this.name = name;
    }
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public abstract String serialize();
    public abstract void deserialize(String data);
    
    @Override
    public abstract String toString();
}
