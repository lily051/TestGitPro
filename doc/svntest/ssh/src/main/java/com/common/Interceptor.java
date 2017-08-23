package com.common;

import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionInvocation;
import com.opensymphony.xwork2.interceptor.AbstractInterceptor;
import com.userAction.RoleAction;
import com.userDao.RoleDao;
import com.userEntity.Role;
import org.apache.log4j.Logger;
import org.apache.struts2.ServletActionContext;
import org.springframework.dao.DataAccessResourceFailureException;
import org.springframework.stereotype.Component;

import javax.annotation.Resource;
import java.util.Map;

/**
 * Created by churui on 2017/8/4.
 */
@Component
public class Interceptor extends AbstractInterceptor {

    @Resource(name = "roleDao")
    private RoleDao roleDao;
    private static Logger log = Logger.getLogger(Interceptor.class);
    @Override
    public String intercept(ActionInvocation invocation) {
        try {
            String url = ServletActionContext.getRequest().getServletPath();
            log.debug("url:"+url);
            if (url.equals("/roleAction!login.action")){
                return invocation.invoke();
            }
            ActionContext actionContext = invocation.getInvocationContext();
            Map<String, Object> session = actionContext.getSession();
            Role role = (Role) session.get(RoleAction.ROLE_INFO);

            if (role != null) {
                roleDao.findall();
                return  invocation.invoke();
            }
            log.debug("还没有登录");
            return "unlogin";

//            if(session.size()>0 && session.get(RoleAction.ROLE_INFO) != null){
//
//                if (role != null) {
//                    roleDao.findall();
//                    return  invocation.invoke();
//                }else{
//                    return "unlogin";
//                }
//            }else {
//                log.debug("session:"+session);
//                return "unlogin";
//            }
        } catch (DataAccessResourceFailureException e){
            e.printStackTrace();
            return "error";
        } catch (Exception e){
            e.printStackTrace();
            return "error";
        }
   }
}
