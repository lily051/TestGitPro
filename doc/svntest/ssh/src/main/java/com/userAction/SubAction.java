package com.userAction;

import com.opensymphony.xwork2.ActionSupport;
import com.userEntity.Node;
import com.userEntity.Subnode;
import com.userService.SubService;
import org.springframework.stereotype.Controller;

import javax.annotation.Resource;
import java.util.List;

/**
 * Created by churui on 2017/7/29.
 */
@Controller
public class SubAction extends ActionSupport {
    private Integer subid;
    private Integer nodeid;
    private String  subname;
    private String msg;
    @Resource(name = "subService")
    private SubService subService;
    private List<Subnode> subnodeList;

    public String execute(){
        Subnode nodes = new Subnode();
        nodes.setSubid(subid);
        nodes.setSubname(subname);
        System.out.println(subname);
        Node node = new Node();
        node.setNodeid(nodeid);
        nodes.setNodetbl(node);
        subService.addSub(nodes);
        System.out.println("ok");
        return "success";
    }
    public String findbynodeid(){
        subnodeList=subService.findbynodeid(1);
        for (Subnode node:subnodeList){
            System.out.println(node.getSubname());
        }
        return "success";
    }

    public Integer getSubid() {
        return subid;
    }

    public void setSubid(Integer subid) {
        this.subid = subid;
    }

    public Integer getNodeid() {
        return nodeid;
    }

    public void setNodeid(Integer nodeid) {
        this.nodeid = nodeid;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    public SubService getSubService() {
        return subService;
    }

    public void setSubService(SubService subService) {
        this.subService = subService;
    }

    public String getSubname() {
        return subname;
    }

    public void setSubname(String subname) {
        this.subname = subname;
    }

    public List<Subnode> getSubnodeList() {
        return subnodeList;
    }

    public void setSubnodeList(List<Subnode> subnodeList) {
        this.subnodeList = subnodeList;
    }
}
