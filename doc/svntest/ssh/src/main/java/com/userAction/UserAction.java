package com.userAction;

import com.opensymphony.xwork2.ActionSupport;
import com.userEntity.Node;
import com.userEntity.User;
import com.userService.NodeService;
import com.userService.UserService;
import org.apache.log4j.Logger;
import org.springframework.stereotype.Controller;

import javax.annotation.Resource;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by churui on 2017/7/11.
 */
@Controller
public class UserAction extends ActionSupport {
    private Integer id;
    private String name;
    private Integer age;
    private String phonenum;
    private String address;
    private String msg;
    private Integer nodeid;
    private String nodename;
    @Resource(name = "userService")
    private UserService userService;
    @Resource(name = "nodeService")
    private NodeService nodeService;
    private List<User> userList = new ArrayList<User>();
    private List<User> usersList= new ArrayList<User>();
    private List<User> subuserList0= new ArrayList<User>();
    private List<User> subuserList1= new ArrayList<User>();
    private boolean flag=true;
    private List<Node> nodeList = new ArrayList<Node>();
    private List<Node> nodeList1 = new ArrayList<Node>();
    private static Logger log = Logger.getLogger(UserAction.class);

    public String adduser() {
        User user = userService.finduserbyname(name);
        if(name.equals("")|| name==null||phonenum.equals("")||phonenum==null){
            msg="姓名/手机号码不能为空";
            flag=false;
            log.debug("姓名/手机号码不能为空");
            return "adduser";
        }
        if (user!=null){
            msg="姓名已存在";
            flag=false;
            return "adduser";
        }else{
            Node node = new Node();
            node.setNodeid(nodeid);
             user = new User();
            user.setName(name);
            user.setAge(age);
            user.setPhonenum(phonenum);
            user.setAddress(address);
            user.setNodetbl(node);
            msg = userService.addUser(user);
            return "adduser";
        }

    }
    public String findsubuser(){
        if(nodeid==1){
            subuserList0=userService.findall();
        }else{
            nodeList= nodeService.findnodeidbyparentid(nodeid);
            subuserList0=userService.findallbyid(nodeid);
            for (Node node:nodeList){
                usersList=userService.findallbyid(node.getNodeid());
                if(usersList!=null){
                    for(User user:usersList){
                        subuserList1.add(user);
                    }
                }

            }
        }

        return "findsubuser";
    }
    public List<Node> findnodelist(){
        while(nodeList.size()>0){
           for (Node node:nodeList){
            nodeList= nodeService.findnodeidbyparentid(node.getNodeid());
            nodeList1.add(node);
            findnodelist();
           }
        }
        return nodeList1;
    }
    public String findall() {
        userList = userService.findallbyid(nodeid);
        return "findall";
    }

    public String finduser() {
        usersList = userService.findall();
        for (User user : usersList) {
        }
        return "finduser";
    }

    public String deleteuser() {
        userService.deleteUser(id);
        return "deleteuser";
    }


    public String updateuser() {
        User user = new User();
        Node node = new Node();
        node.setNodeid(nodeid);
        user.setId(id);
        user.setName(name);
        user.setAge(age);
        user.setPhonenum(phonenum);
        user.setAddress(address);
        user.setNodetbl(node);
        userService.updateUser(user);
        return "updateuser";
    }

    public void setUserService(UserService userService) {
        this.userService = userService;
    }


    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }

    public String getPhonenum() {
        return phonenum;
    }

    public void setPhonenum(String phonenum) {
        this.phonenum = phonenum;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    public List<User> getUserList() {
        return userList;
    }

    public void setUserList(List<User> userList) {
        this.userList = userList;
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

    public List<User> getUsersList() {
        return usersList;
    }

    public void setUsersList(List<User> usersList) {
        this.usersList = usersList;
    }

    public boolean isFlag() {
        return flag;
    }

    public void setFlag(boolean flag) {
        this.flag = flag;
    }

    public List<User> getSubuserList0() {
        return subuserList0;
    }

    public void setSubuserList0(List<User> subuserList0) {
        this.subuserList0 = subuserList0;
    }

    public List<User> getSubuserList1() {
        return subuserList1;
    }

    public void setSubuserList1(List<User> subuserList1) {
        this.subuserList1 = subuserList1;
    }

    public List<Node> getNodeList() {
        return nodeList;
    }

    public void setNodeList(List<Node> nodeList) {
        this.nodeList = nodeList;
    }

    public List<Node> getNodeList1() {
        return nodeList1;
    }

    public void setNodeList1(List<Node> nodeList1) {
        this.nodeList1 = nodeList1;
    }
}