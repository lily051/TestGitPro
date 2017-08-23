package com.userService;

import com.common.TreeNodeInfo;
import com.common.TreeNodeUtil;
import com.userDao.NodeDao;
import com.userEntity.Node;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by churui on 2017/7/26.
 */
@Service("nodeService")
public class NodeServiceImpl implements NodeService{
    @Resource(name = "nodeDao")
    private NodeDao nodeDao;

    @Override
    public void addnode(Node node) {
        nodeDao.addnode(node);
    }

    public boolean nodeMax(Integer parentid){

        return true;
    }

    public List<Integer> findallparentid(Integer nodeid){
        List<Integer> result = new ArrayList<Integer>();
        Node node = nodeDao.findnodebyid(nodeid);
        while(node !=null){
            if(node.getParentid()==null || node.getParentid()==0){
                break;
            }

            result.add(node.getParentid());
            node = nodeDao.findnodebyid(node.getParentid());
        }
        return result;
    }
    @Override
    public List<Node> findall() {
        return  nodeDao.findall();
    }

    @Override
    public void deletenode(Integer nodeid) {
        nodeDao.deletenode(nodeid);
    }

    @Override
    public Node findnodebyname(String nodename) {
        return nodeDao.findnodebyname(nodename);
    }

    @Override
    public List<Node> findnodeidbyparentid(Integer parentid) {
        return nodeDao.findnodeidbyparentid(parentid);
    }

    @Override
    public Node findnodebyid(Integer nodeid) {
        return nodeDao.findnodebyid(nodeid);
    }

    @Override
    public void updatenode(Node node) {
        nodeDao.updatenode(node);
    }

    @Override
    public void getRootNode(List<TreeNodeInfo> treeNodeList, int parentid) {
        List<Node> nodeList = findnodeidbyparentid(parentid);
        Node rootnode = nodeList.get(0);
        TreeNodeInfo rootnodeinfo = TreeNodeUtil.initTreeNodeInfo(rootnode);
        rootnodeinfo.setOpen(true);
        rootnodeinfo.setIsParent(true);
        treeNodeList.add(rootnodeinfo);
    }

    @Override
    public void getNode(List<TreeNodeInfo> treeNodeList, int nodeid) {
        Node node = findnodebyid(nodeid);
        List<Node> subNodes = nodeDao.findNextLevelChildren(node);
            TreeNodeInfo treeNode = TreeNodeUtil.initTreeNodeInfo(node, nodeid);
            treeNode.setOpen(false);
            treeNodeList.add(treeNode);
        if (subNodes != null && subNodes.size() > 0) {
            for (Node unit:subNodes){
                treeNode = TreeNodeUtil.initTreeNodeInfo(unit, nodeid);
                treeNode.setOpen(false);
                List<Node> next = nodeDao.findNextLevelChildren(unit);
                if(next!=null && next.size()>0){
                    for(Node nextunit:next){
                        getNode(treeNodeList,nextunit.getNodeid());
                    }
                }
                treeNodeList.add(treeNode);
            }
        }
    }

}
