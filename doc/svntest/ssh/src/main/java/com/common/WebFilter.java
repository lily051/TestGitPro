package com.common;

import com.userAction.RoleAction;
import com.userEntity.Role;
import org.apache.log4j.Logger;

import javax.servlet.*;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;

/**
 * Created by churui on 2017/8/7.
 */
public class WebFilter extends HttpServlet implements Filter {
    private static Logger log = Logger.getLogger(WebFilter.class);
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {

    }

    @Override
    public void doFilter(ServletRequest req, ServletResponse res, FilterChain chain) throws IOException, ServletException {
        HttpServletRequest request = (HttpServletRequest) req;
        String uri = request.getRequestURI();
        log.debug("uri:"+uri);
        HttpServletResponse response = (HttpServletResponse) res;
        HttpSession session = request.getSession();
            String[] urls = new String[]{"/","/index.jsp"};
            if(session!=null){
                Role role = (Role) session.getAttribute(RoleAction.ROLE_INFO);
                log.debug("role:"+role);
                for (String path:urls){
                    if(uri.equals(path)){
                        chain.doFilter(request, response);
                        return;
                    }
                }
                if (role==null){
                    response.sendRedirect("/index.jsp");
                }else{
                    chain.doFilter(request, response);
                    return;
                }
            }else{
                response.sendRedirect("/index.jsp");
            }
    }
}
