package com.common;

/**
 * Created by churui on 2017/8/8.
 */
public class TreeNode{
    /**
     * id
     */
    private String id;
    /**
     * pid,父节点的id
     */
    private String pid;
    /**
     * 是否展开
     */
    private boolean open;
    /**
     * 是否有子节点
     */
    private boolean isParent;
    /**
     * 节点名称
     */
    private String name;
    /**
     * 点击事情
     */
    private String click;
    /**
     *   节点所指向的文件路径
     */
    private String path;

    public TreeNode(String id, String pid, boolean open, boolean isParent,
                    String name, String click, String path) {
        this.id = id;
        this.pid = pid;
        this.open = open;
        this.isParent = isParent;
        this.name = name;
        this.click = click;
        this.path = path;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getPid() {
        return pid;
    }

    public void setPid(String pid) {
        this.pid = pid;
    }

    public boolean isOpen() {
        return open;
    }

    public void setOpen(boolean open) {
        this.open = open;
    }

    public boolean isParent() {
        return isParent;
    }

    public void setParent(boolean isParent) {
        this.isParent = isParent;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getClick() {
        return click;
    }

    public void setClick(String click) {
        this.click = click;
    }

    @Override
    public String toString() {
        return super.toString();
    }

    public String getPath() {
        return path;
    }

    public void setPath(String path) {
        this.path = path;
    }
}
