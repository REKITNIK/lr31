// BinaryTree.java
package structures;

import java.util.LinkedList;
import java.util.Queue;

public class BinaryTree extends Structure {
    private static class TreeNode {
        int key;
        TreeNode left;
        TreeNode right;
        TreeNode parent;
        
        TreeNode(int key) {
            this.key = key;
            this.left = null;
            this.right = null;
            this.parent = null;
        }
    }
    
    private TreeNode root;
    
    public BinaryTree() {
        super();
        root = null;
    }
    
    public BinaryTree(String name) {
        super(name);
        root = null;
    }
    
    public void add(int key) {
        TreeNode newNode = new TreeNode(key);
        
        if (root == null) {
            root = newNode;
            return;
        }
        
        TreeNode current = root;
        TreeNode parent = null;
        
        while (current != null) {
            parent = current;
            if (key < current.key) {
                current = current.left;
            } else if (key > current.key) {
                current = current.right;
            } else {
                // Key already exists
                return;
            }
        }
        
        newNode.parent = parent;
        if (key < parent.key) {
            parent.left = newNode;
        } else {
            parent.right = newNode;
        }
    }
    
    public TreeNode find(int key) {
        TreeNode current = root;
        
        while (current != null) {
            if (key == current.key) {
                return current;
            } else if (key < current.key) {
                current = current.left;
            } else {
                current = current.right;
            }
        }
        
        return null;
    }
    
    public boolean contains(int key) {
        return find(key) != null;
    }
    
    private TreeNode findMin(TreeNode node) {
        if (node == null) return null;
        while (node.left != null) {
            node = node.left;
        }
        return node;
    }
    
    private TreeNode findMax(TreeNode node) {
        if (node == null) return null;
        while (node.right != null) {
            node = node.right;
        }
        return node;
    }
    
    public TreeNode findPredecessor(int key) {
        TreeNode node = find(key);
        if (node == null) return null;
        
        if (node.left != null) {
            return findMax(node.left);
        }
        
        TreeNode current = node;
        TreeNode parent = node.parent;
        while (parent != null && current == parent.left) {
            current = parent;
            parent = parent.parent;
        }
        
        return parent;
    }
    
    public TreeNode findSuccessor(int key) {
        TreeNode node = find(key);
        if (node == null) return null;
        
        if (node.right != null) {
            return findMin(node.right);
        }
        
        TreeNode current = node;
        TreeNode parent = node.parent;
        while (parent != null && current == parent.right) {
            current = parent;
            parent = parent.parent;
        }
        
        return parent;
    }
    
    public void delete(int key) {
        TreeNode node = find(key);
        if (node == null) return;
        
        if (node.left == null && node.right == null) {
            // Case 1: Leaf node
            if (node.parent == null) {
                root = null;
            } else if (node == node.parent.left) {
                node.parent.left = null;
            } else {
                node.parent.right = null;
            }
        } else if (node.left == null) {
            // Case 2: Only right child
            if (node.parent == null) {
                root = node.right;
                root.parent = null;
            } else if (node == node.parent.left) {
                node.parent.left = node.right;
                node.right.parent = node.parent;
            } else {
                node.parent.right = node.right;
                node.right.parent = node.parent;
            }
        } else if (node.right == null) {
            // Case 3: Only left child
            if (node.parent == null) {
                root = node.left;
                root.parent = null;
            } else if (node == node.parent.left) {
                node.parent.left = node.left;
                node.left.parent = node.parent;
            } else {
                node.parent.right = node.left;
                node.left.parent = node.parent;
            }
        } else {
            // Case 4: Two children
            TreeNode successor = findMin(node.right);
            node.key = successor.key;
            
            // Delete successor
            if (successor.parent.left == successor) {
                successor.parent.left = successor.right;
            } else {
                successor.parent.right = successor.right;
            }
            
            if (successor.right != null) {
                successor.right.parent = successor.parent;
            }
        }
    }
    
    public boolean isFull() {
        return countInnerNodes(root) + 1 == countLeaves(root);
    }
    
    private int countInnerNodes(TreeNode node) {
        if (node == null || (node.left == null && node.right == null)) {
            return 0;
        }
        return 1 + countInnerNodes(node.left) + countInnerNodes(node.right);
    }
    
    private int countLeaves(TreeNode node) {
        if (node == null) return 0;
        if (node.left == null && node.right == null) return 1;
        return countLeaves(node.left) + countLeaves(node.right);
    }
    
    public String getTraversal(String mode) {
        switch (mode.toUpperCase()) {
            case "PRE":
                return preOrder();
            case "IN":
                return inOrder();
            case "POST":
                return postOrder();
            case "BFS":
                return bfs();
            default:
                return "";
        }
    }
    
    private String preOrder() {
        StringBuilder sb = new StringBuilder();
        preOrderHelper(root, sb);
        return sb.toString().trim();
    }
    
    private void preOrderHelper(TreeNode node, StringBuilder sb) {
        if (node != null) {
            sb.append(node.key).append(" ");
            preOrderHelper(node.left, sb);
            preOrderHelper(node.right, sb);
        }
    }
    
    private String inOrder() {
        StringBuilder sb = new StringBuilder();
        inOrderHelper(root, sb);
        return sb.toString().trim();
    }
    
    private void inOrderHelper(TreeNode node, StringBuilder sb) {
        if (node != null) {
            inOrderHelper(node.left, sb);
            sb.append(node.key).append(" ");
            inOrderHelper(node.right, sb);
        }
    }
    
    private String postOrder() {
        StringBuilder sb = new StringBuilder();
        postOrderHelper(root, sb);
        return sb.toString().trim();
    }
    
    private void postOrderHelper(TreeNode node, StringBuilder sb) {
        if (node != null) {
            postOrderHelper(node.left, sb);
            postOrderHelper(node.right, sb);
            sb.append(node.key).append(" ");
        }
    }
    
    private String bfs() {
        if (root == null) return "";
        
        StringBuilder sb = new StringBuilder();
        Queue<TreeNode> queue = new LinkedList<>();
        queue.offer(root);
        
        while (!queue.isEmpty()) {
            TreeNode node = queue.poll();
            sb.append(node.key).append(" ");
            
            if (node.left != null) {
                queue.offer(node.left);
            }
            if (node.right != null) {
                queue.offer(node.right);
            }
        }
        
        return sb.toString().trim();
    }
    
    @Override
    public String serialize() {
        String traversal = preOrder();
        String[] values = traversal.split(" ");
        int count = values.length > 0 && !values[0].isEmpty() ? values.length : 0;
        
        StringBuilder sb = new StringBuilder();
        sb.append("T ").append(name).append(" ").append(count);
        
        if (count > 0) {
            sb.append(" ").append(traversal);
        }
        
        return sb.toString();
    }
    
    @Override
    public void deserialize(String data) {
        String[] parts = data.split(" ");
        if (parts.length < 3) return;
        
        this.name = parts[1];
        int count = Integer.parseInt(parts[2]);
        
        // Clear tree
        root = null;
        
        // Reconstruct tree
        for (int i = 0; i < count && (i + 3) < parts.length; i++) {
            try {
                int key = Integer.parseInt(parts[i + 3]);
                add(key);
            } catch (NumberFormatException e) {
                // Skip invalid numbers
            }
        }
    }
    
    @Override
    public String toString() {
        return "BinaryTree \"" + name + "\" (size: " + countNodes(root) + "): " + inOrder();
    }
    
    private int countNodes(TreeNode node) {
        if (node == null) return 0;
        return 1 + countNodes(node.left) + countNodes(node.right);
    }
}
