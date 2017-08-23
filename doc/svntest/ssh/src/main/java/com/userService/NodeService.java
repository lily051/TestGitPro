package com.userService;

import com.common.TreeNodeInfo;
import com.userEntity.Node;

import java.util.List;

/**
 * Created by churui on 2017/7/26.
 */
public interface NodeService {
    public void addnode(Node node);
    public List<Node> findall();
    public void deletenode(Integer nodeid);
    public Node findnodebyname(String nodename);
    public List<Node> findnodeidbyparentid(Integer parentid);
    public Node findnodebyid(Integer nodeid);
    public void updatenode(Node node);
    public void getRootNode(List<TreeNodeInfo> treeNodeList, int parentid);
    public void getNode(List<TreeNodeInfo> treeNodeList, int nodeid);
    public List<Integer> findallparentid(Integer parentid);
    //public void getNode(List<TreeNodeInfo> treeNodeList, int parentid);

}
