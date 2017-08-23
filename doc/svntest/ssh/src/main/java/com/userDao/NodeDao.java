package com.userDao;

import com.userEntity.Node;

import java.util.List;

/**
 * Created by churui on 2017/7/26.
 */
public interface NodeDao {
    public void addnode(Node node);
    public List<Node> findall();
    public void deletenode(Integer nodeid);
    public Node findnodebyname(String nodename);
    public List<Node> findnodeidbyparentid(Integer parentid);
    public Node findnodebyid(Integer nodeid);
    public void updatenode(Node node);
    public List<Node> findNextLevelChildren(final Node nodetbl);
}
