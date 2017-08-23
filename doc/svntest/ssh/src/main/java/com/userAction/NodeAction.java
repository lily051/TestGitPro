package com.userAction;

import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.common.TreeNodeInfo;
import com.common.TreeNodeUtil;
import com.userEntity.Node;
import com.userEntity.Role;
import com.userEntity.Roles;
import com.userEntity.User;
import com.userService.NodeService;
import com.userService.RoleService;
import com.userService.UserService;
import org.springframework.stereotype.Controller;

import javax.annotation.Resource;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

/**
 * Created by churui on 2017/7/26.
 */
@Controller
public class NodeAction extends BaseAction{
    private static final String ROOT_NOE = "root_0";
    private static boolean FLAG = true;

    private Integer nodeid;
    private String nodename;
    private Integer parentid;
    private Integer nodeId;
    protected String checked;
    //是否展开
    private boolean open;
    //是否有子节点
    private boolean isParent;
    private String click;
    //节点所指向的文件路径
    private String path;

    @Resource(name = "nodeService")
    private NodeService nodeService;
    private List<Node> nodeList =new ArrayList<Node>();
    private String msg;
    private boolean flag = true;
    private Node node = new Node();
    private List<User> userList1 = new ArrayList<User>();
    private List<User> userList2 = new ArrayList<User>();
    @Resource(name = "userService")
    private UserService userService;
    @Resource (name = "roleService")
    private RoleService roleService;
    private Integer level;
    private List<Node> levelist0=new ArrayList<Node>();
    private List<Node> levelist1=new ArrayList<Node>();
    private List<Node> levelist2=new ArrayList<Node>();
    private List<String> levelist3 = new ArrayList<String>();
    private List<Role> roleList = new ArrayList<Role>();
    private List<Roles> rolesList = new ArrayList<Roles>();
    private JSONArray jsonArray = new JSONArray();
    private JSONArray jsonArray1 = new JSONArray();

    public String addnode(){
        node = nodeService.findnodebyname(nodename);
        if(nodename.equals("")||nodename==null){
            msg="名称不能为空";
            flag = false;
            return "addnode";
        }
        if (node!=null){
            flag = false;
            msg="名称已存在";
            return "addnode";
        }
        node = new Node();
        node.setNodename(nodename);
        node.setParentid(parentid);
        if(parentid==0){
            level = 0;
        }else if(parentid==1){
            level=1;
        }else{
            level=parentid;
        }
        node.setLevel(level);
        if(nodeMax(parentid)){
            flag = false;
            msg="节点树不能超过6级";
            return "addnode";
        }
        nodeService.addnode(node);
        return "addnode";

    }
    public boolean nodeMax(Integer parentid){
        List<Integer> parentNodes = nodeService.findallparentid(parentid);
        if (parentNodes.size() >=6) {
            return true;
        }
        return false;
    }
    public String findsubnode(){
        node= nodeService.findnodebyname(nodename);
        return "findsubnode";
    }
    public String shownodetree() throws Exception{
        try{
            List<TreeNodeInfo> treeNodeList = new LinkedList<TreeNodeInfo>();//发送给userindex.jsp
            List<TreeNodeInfo> treeNodeList1 = new LinkedList<TreeNodeInfo>();//发送给roleindex.jsp,只需根节点和1级节点
            List<Node> nodeList = nodeService.findall();
            for(Node node:nodeList){
                if(node.getParentid()==0){
                    nodeService.getRootNode(treeNodeList,node.getParentid());
                    nodeService.getRootNode(treeNodeList1,node.getParentid());
                }else if(node.getParentid()==1){
                    nodeService.getNode(treeNodeList,node.getNodeid());
                    TreeNodeInfo treeNode = TreeNodeUtil.initTreeNodeInfo(node, node.getNodeid());
                    treeNode.setOpen(false);
                    treeNodeList1.add(treeNode);
                }
            }
            for(TreeNodeInfo nodeInfo : treeNodeList){
                JSONObject jsonObject = new JSONObject();
                jsonObject.put("id",nodeInfo.getId());
                jsonObject.put("pId",nodeInfo.getpId());
                jsonObject.put("name", nodeInfo.getName());
                jsonObject.put("open",nodeInfo.isOpen());
                jsonArray.add(jsonObject);
            }
            for(TreeNodeInfo nodeInfo : treeNodeList1){
                JSONObject jsonObject = new JSONObject();
                jsonObject.put("id",nodeInfo.getId());
                jsonObject.put("pId",nodeInfo.getpId());
                jsonObject.put("name", nodeInfo.getName());
                jsonObject.put("open",nodeInfo.isOpen());
                jsonArray1.add(jsonObject);
            }
        }catch (Exception e){
            e.printStackTrace();
        }
        return "shownodetree";
    }
    public String deletesubnode(){
        deletesubnodes(nodeid);
        return "deletesubnode";
    }
    public void deletesubnodes(Integer nodeid){
        if(nodeid==1){
            flag = false;
            msg = "根节点不能删除";
            return ;
        }
        node = nodeService.findnodebyid(nodeid);
        if (node.getParentid()==1){
            roleList=roleService.findbynode(nodeid);
            rolesList = roleService.findrolesbynode(nodeid);
            if (roleList.size()>0 || rolesList.size()>0){
                flag=false;
                msg="节点中包含用户或角色信息";
                return;
            }else{
                deletenode(nodeid);
            }
        }else{
            deletenode(nodeid);
        }
    }
    public void deletenode(Integer nodeid){
        FLAG = true;
        userList1 = userService.findallbyid(nodeid);
        if(userList1.size()>0) {
            flag = false;
            msg = "节点中有用户信息,不能删除";
            FLAG=false;
            //return ;
        }else{
            nodeList=nodeService.findnodeidbyparentid(nodeid);
            if (nodeList.size()==0){
                nodeService.deletenode(nodeid);
            }else{
                for(Node node:nodeList){
                    if (FLAG){
                        deletenode(node.getNodeid());
                    }
                }
               if (FLAG){
                   nodeService.deletenode(nodeid);
               }
            }
        }

    }
    public String updatenode(){
        node = nodeService.findnodebyid(nodeid);
        node.setNodename(nodename);
        if(nodename.equals("")||nodename==null){
            msg="名称不能为空";
            flag = false;
            return "updatenode";
        }
        Node nodes = nodeService.findnodebyname(nodename);
        if (nodes!=null && nodename.equals(nodes.getNodename())){
            flag = false;
            msg="名称已存在";
            return "updatenode";
        }
//        node.setNodeid(node.getNodeid());
//        node.setLevel(node.getLevel());
//        node.setParentid(node.getParentid());
        nodeService.updatenode(node);
        return "updatenode";
    }
    public List<Node> findnodeidbyparentid(){
        nodeList=nodeService.findnodeidbyparentid(nodeid);
        return nodeList;
    }
    public Integer getNodeid() {
        return nodeid;
    }

    public void setNodeid(Integer nodeid) {
        this.nodeid = nodeid;
    }

    public String getNodename() {
        return nodename;
    }

    public void setNodename(String nodename) {
        this.nodename = nodename;
    }

    public List<Node> getNodeList() {
        return nodeList;
    }

    public void setNodeList(List<Node> nodeList) {
        this.nodeList = nodeList;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    public boolean isFlag() {
        return flag;
    }

    public void setFlag(boolean flag) {
        this.flag = flag;
    }

    public Node getNode() {
        return node;
    }

    public void setNode(Node node) {
        this.node = node;
    }

    public Integer getParentid() {
        return parentid;
    }

    public void setParentid(Integer parentid) {
        this.parentid = parentid;
    }

    public List<User> getUserList1() {
        return userList1;
    }

    public void setUserList1(List<User> userList1) {
        this.userList1 = userList1;
    }

    public List<User> getUserList2() {
        return userList2;
    }

    public void setUserList2(List<User> userList2) {
        this.userList2 = userList2;
    }


    public Integer getLevel() {
        return level;
    }

    public void setLevel(Integer level) {
        this.level = level;
    }

    public List<Node> getLevelist0() {
        return levelist0;
    }

    public void setLevelist0(List<Node> levelist0) {
        this.levelist0 = levelist0;
    }

    public List<Node> getLevelist1() {
        return levelist1;
    }

    public void setLevelist1(List<Node> levelist1) {
        this.levelist1 = levelist1;
    }

    public List<Node> getLevelist2() {
        return levelist2;
    }

    public void setLevelist2(List<Node> levelist2) {
        this.levelist2 = levelist2;
    }

    public List<String> getLevelist3() {
        return levelist3;
    }

    public void setLevelist3(List<String> levelist3) {
        this.levelist3 = levelist3;
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

    public void setParent(boolean parent) {
        isParent = parent;
    }

    public String getClick() {
        return click;
    }

    public void setClick(String click) {
        this.click = click;
    }

    public String getPath() {
        return path;
    }

    public void setPath(String path) {
        this.path = path;
    }

    public Integer getNodeId() {
        return nodeId;
    }

    public void setNodeId(Integer nodeId) {
        this.nodeId = nodeId;
    }

    public String getChecked() {
        return checked;
    }

    public void setChecked(String checked) {
        this.checked = checked;
    }

    public JSONArray getJsonArray() {
        return jsonArray;
    }

    public void setJsonArray(JSONArray jsonArray) {
        this.jsonArray = jsonArray;
    }

    public JSONArray getJsonArray1() {
        return jsonArray1;
    }

    public void setJsonArray1(JSONArray jsonArray1) {
        this.jsonArray1 = jsonArray1;
    }
}
