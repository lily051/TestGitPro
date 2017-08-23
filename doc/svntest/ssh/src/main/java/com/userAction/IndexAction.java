package com.userAction;

import com.userEntity.Role;
import com.userService.RoleService;

import javax.annotation.Resource;

/**
 * Created by churui on 2017/8/18.
 */
public class IndexAction extends BaseAction{
    private Integer loginRid;
    private Integer modelnum;
    private Integer orgcode;
    private Integer rolecode;
    private Integer rolescode;
    @Resource(name = "roleService")
    private RoleService roleService;
    private  String currentModelCode ="";

    public String gotoIndexpage(){
        try{
            Role role = getRoleInfo();
            loginRid=role.getRoles().getRid();
            modelnum = roleService.findroleprebyrid(loginRid).getModulenum();

//            if ( !currentModelCode.equals("") ){
//                boolean isReturn = true;
//                if (modelnum.toString().equals(currentModelCode)){
//                    isReturn=false;
//                }else if(modelnum==0 && currentModelCode.equals("1")){
//                    isReturn=false;
//                }else if(modelnum==0 && currentModelCode.equals("2")){
//                    isReturn=false;
//                }else if(modelnum==0 && currentModelCode.equals("3")){
//                    isReturn=false;
//                }else if(modelnum==2 && currentModelCode.equals("3")){
//                    isReturn=false;
//                }
//                if ( isReturn ){
//                    return "error";
//                }
//            }
            if (currentModelCode.equals("1")){
                orgcode=1;
                rolecode=2;
                rolescode=3;
                return "gotoOrgIndex";
            }else if (currentModelCode.equals("2")){
                orgcode=1;
                rolecode=2;
                rolescode=3;
                return "gotoRoleIndex";
            }else if(currentModelCode.equals("0")){
                orgcode=1;
                rolecode=2;
                rolescode=3;
                return "gotoUserIndex";
            }else if(currentModelCode.equals("3")){
                orgcode=1;
                rolecode=2;
                rolescode=3;
                return "gotoRolesIndex";
            }
        }catch(Exception e){
            e.printStackTrace();
        }
        return "gotoUserindex";
    }


    public Integer getLoginRid() {
        return loginRid;
    }

    public void setLoginRid(Integer loginRid) {
        this.loginRid = loginRid;
    }

    public Integer getModelnum() {
        return modelnum;
    }

    public void setModelnum(Integer modelnum) {
        this.modelnum = modelnum;
    }

    public String getCurrentModelCode() {
        return currentModelCode;
    }

    public void setCurrentModelCode(String currentModelCode) {
        this.currentModelCode = currentModelCode;
    }

    public Integer getOrgcode() {
        return orgcode;
    }

    public void setOrgcode(Integer orgcode) {
        this.orgcode = orgcode;
    }

    public Integer getRolecode() {
        return rolecode;
    }

    public void setRolecode(Integer rolecode) {
        this.rolecode = rolecode;
    }

    public Integer getRolescode() {
        return rolescode;
    }

    public void setRolescode(Integer rolescode) {
        this.rolescode = rolescode;
    }
}
