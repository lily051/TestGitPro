package com.userAction;

import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;
import com.userEntity.Role;
import com.userEntity.Sysconfigtbl;
import org.apache.struts2.interceptor.ServletRequestAware;
import org.apache.struts2.interceptor.ServletResponseAware;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.Map;

/**
 * Created by churui on 2017/7/25.
 */
public class BaseAction extends ActionSupport implements ServletResponseAware, ServletRequestAware {
    private HttpServletResponse response;
    private HttpServletRequest request;
    private Role role;

    @Override
    public void setServletRequest(HttpServletRequest request) {
        this.request = request;
    }

    @Override
    public void setServletResponse(HttpServletResponse response) {
        this.response = response;
    }

    public Role getRoleInfo() {
        try {
            if (role == null && request != null && request.getSession() != null) {
                role = (Role) request.getSession().getAttribute(RoleAction.ROLE_INFO);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return role;
    }
    public Integer getSysTreeOrder(){
        ActionContext actionContext = ActionContext.getContext();
        Map<String, Object> application = actionContext.getApplication();
        Sysconfigtbl sysConfig = (Sysconfigtbl)application.get("sysConfig");
        return sysConfig.getTreeOrder();
    }


    public HttpServletResponse getResponse() {
        return response;
    }

    public void setResponse(HttpServletResponse response) {
        this.response = response;
    }

    public HttpServletRequest getRequest() {
        return request;
    }

    public void setRequest(HttpServletRequest request) {
        this.request = request;
    }

    public Role getRole() {
        return role;
    }

    public void setRole(Role role) {
        this.role = role;
    }
}