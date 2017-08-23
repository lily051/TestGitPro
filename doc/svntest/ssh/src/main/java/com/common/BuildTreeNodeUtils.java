package com.common;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by churui on 2017/8/8.
 */
public class BuildTreeNodeUtils {
     /* *   文件列表的工具类
     */
    //private FileListUtils fUtils;
    /**
     *   根结点
     */
    private TreeNode headNode;
    /**
     *   父节点
     */
    private TreeNode parentNode;
    /**
     *   文件列表
     */
    private List<File> fList;
    /**
     *   目录列表
     */
    private List<File> fDlist;
    /**
     *
     */
    private boolean flag;
    private String pid;
    /**
     *   是否有子节点
     */
    private boolean isParent;
    /**
     *  目录树的数据
     */
    private StringBuffer treeNodeData;
    /**
     *   文件路径
     */
    private String path;

    public BuildTreeNodeUtils(String path) {
        this.path = path;
    }

//    public BuildTreeNodeUtils(TreeNode tn, boolean flag) {
//        if (tn != null) {
//            fUtils = new FileListUtils(tn.getPath());
//            fList = fUtils.getfList();
//            fDlist = fUtils.getdList();
//            this.flag = flag;
//            this.pid = tn.getId();
//            this.parentNode = tn;
//        }
  //  }

    /**@description:构造一个根节点的json数据
     * @author:yehui
     * @return:StringBuffer
     * @return*/

    public StringBuffer init() {
        getHeadNode();
        treeNodeData = new StringBuffer();
        // {id:1, pId:0, name:"[core] 基本的な操作の例", open:true},
        treeNodeData.append("[{id:").append(headNode.getId()).append(",pid:")
                .append(headNode.getPid()).append(",open:")
                .append(headNode.isOpen()).append(",isParent:")
                .append(headNode.isParent()).append(",name:\"")
                .append(headNode.getPath().replaceAll("\\\\", "/"))
                .append("\"").append(",click:\"").append(headNode.getClick()).
                append("\"").append(",path:\"").append(headNode.getPath().replaceAll("\\\\", "/")).append("\"").append("}]");
        return treeNodeData;
    }

    /**@description:获得根节点
     * @author:yehui
     * @return:TreeNode
     * @return*/

    public TreeNode getHeadNode() {

        File ftemp = new File(path);
        if (ftemp.isDirectory() && ftemp.listFiles().length > 0) {
            headNode = new TreeNode("0", "0", false, true, ftemp.getPath(),
                    "getChildNodes",ftemp.getPath());
        } else {
            headNode = new TreeNode("0", "0", false, false, ftemp.getPath(),
                    "getChildNodes",ftemp.getPath());
        }

        return headNode;
    }

    /**@description:    获得某个节点的子节点
     * @author:yehui
     * @return:List<TreeNode>
     * @return*/

    public List<TreeNode> findChildNodes() {
        List<TreeNode> ltList = null;
        if (parentNode.isParent()) {
            ltList = new ArrayList<TreeNode>();
            if (flag) {
                for (int i = 0; i < fList.size(); i++) {
                    File file = new File(fList.get(i).getPath());
                    if (file.exists()) {
                        if (file.isDirectory()) {
                            isParent = true;
                        } else {
                            isParent = false;
                        }
                    }
                    TreeNode tNode;
                    if (isParent) {
                        tNode = new TreeNode(pid + i, pid, false, true, fList
                                .get(i).getName(), "getChildNodes();",fList
                                .get(i).getPath());
                    } else {
                        tNode = new TreeNode(pid + i, pid, false, false, fList
                                .get(i).getName(), "",fList
                                .get(i).getPath());
                    }
                    ltList.add(tNode);
                }
            } else {
                for (int i = 0; i < fDlist.size(); i++) {
                    TreeNode tNode;
                    tNode = new TreeNode(pid + i, pid, false, true, fDlist
                            .get(i).getName(), "",fDlist
                            .get(i).getPath());
                    ltList.add(tNode);
                }
            }
        }
        return ltList;
    }

    /**@description:    构造子节点的json数据
     * @author:yehui
     * @return:StringBuffer
     * @return*/

    public StringBuffer getTreeNodeData() {
        List<TreeNode> ltList = findChildNodes();
        int count = ltList.size();
        if (count > 0) {
            treeNodeData = new StringBuffer();
            treeNodeData.append("[");
            for (int i = 0; i < count; i++) {
                TreeNode tempNode = ltList.get(i);
                treeNodeData.append("{id:").append(tempNode.getId())
                        .append(",pid:").append(tempNode.getPid())
                        .append(",open:").append(tempNode.isOpen())
                        .append(",isParent:").append(tempNode.isParent())
                        .append(",name:").append("\"")
                        .append(tempNode.getName().replaceAll("\\\\", "/"))
                        .append("\"").append(",click:\"")
                        .append(tempNode.getClick()).append("\"").append(",path:").append("\"").append(tempNode.getPath().replaceAll("\\\\", "/")).append("\"").append("}");
                if (i == count - 1) {
                    treeNodeData.append("]");
                } else {
                    treeNodeData.append(",");
                }
            }
        }
        return treeNodeData;
    }

    public TreeNode stringToTreeNode(String str) {

        TreeNode tmpNode;
        return null;
    }

    public String getFileName() {
        return parentNode.getName();
    }

//    public FileListUtils getfUtils() {
//        return fUtils;
//    }

    public TreeNode getParentNode() {
        return parentNode;
    }

    public List<File> getfList() {
        return fList;
    }

    public List<File> getfDlist() {
        return fDlist;
    }

    public boolean isFlag() {
        return flag;
    }

    public String getPid() {
        return pid;
    }

    public boolean isParent() {
        return isParent;
    }

    public String getPath() {
        return path;
    }

    public void setPath(String path) {
        this.path = path;
    }
}
