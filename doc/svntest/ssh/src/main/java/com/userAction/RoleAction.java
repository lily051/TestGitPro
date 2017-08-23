package com.userAction;

import com.common.ModuleInfo;
import com.opensymphony.xwork2.ActionSupport;
import com.redis.RedisUtil;
import com.userEntity.Node;
import com.userEntity.Role;
import com.userEntity.Rolepre;
import com.userEntity.Roles;
import com.userService.NodeService;
import com.userService.RoleService;
import org.apache.log4j.Logger;
import org.apache.struts2.interceptor.ServletRequestAware;
import org.apache.struts2.interceptor.ServletResponseAware;
import org.springframework.stereotype.Controller;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by churui on 2017/7/11.
 */
@Controller
public class RoleAction extends ActionSupport implements ServletResponseAware, ServletRequestAware {

    public static String ROLE_INFO = "role_info";
    public static String NAMESPACE_LOGIN = "login$";

    //用户信息
    private Integer roleid;
    private String rolename;
    private String rolepwd;
    private String roledesc;
    private String remark;
    //角色信息
    private Integer rid;
    private String rname;
    private String rdesc;
    private Integer modelnum;
    @Resource(name = "roleService")
    private RoleService roleService;
    private String message;
    private Integer nodeid;
    private List<Role> roleList = new ArrayList<Role>();
    private boolean flag = true;
    @Resource(name = "nodeService")
    private NodeService nodeService;
    private List<Node> nodeList;
    private HttpServletResponse response;
    private HttpServletRequest request;
    private HttpSession session;
    private List<Roles> rolesList = new ArrayList<Roles>();

    private Integer invalidateTimes;
   // private RedisClientTemplate redisClientTemplate;
    //private Sysconfigtbl sysconfig = new Sysconfigtbl();
    private  RedisUtil redisUtil;
    private static Logger log = Logger.getLogger(RoleAction.class);

    //添加用户
    public String addrole() {
        Node node = new Node();
        node.setNodeid(nodeid);
        Roles roles = new Roles();
        roles = roleService.findrolesbyname(rname);
        Role role = new Role();
        role.setRolename(rolename);
        role.setRolepwd(rolepwd);
        role.setRoledesc(roledesc);
        role.setRemark(remark);
        role.setNodetbl(node);
        role.setRoles(roles);
        roleService.addRole(role);
        return "addrole";

    }
    //添加角色
    public String addroles(){
        Roles roles = roleService.findrolesbyname(rname);
        if (roles !=null){
            flag=false;
            message="角色名已经存在";
            return "addroles";
        }else{
            Node node = new Node();
            node.setNodeid(nodeid);
            roles = new Roles();
            roles.setNodetbl(node);
            roles.setRdesc(rdesc);
            roles.setRname(rname);
            roleService.addRoles(roles);
            addrolepre();
            return "addroles";
        }
    }
    public String updateroles(){
        Node node = new Node();
        node.setNodeid(nodeid);
        Roles roles = roleService.findrolesbyrid(rid);
            roles.setRname(rname);
            roles.setRdesc(rdesc);
            roles.setNodetbl(node);
            roleService.updateroles(roles);
            System.out.println();
            updaterolepre();
            return "updateroles";
    }
    public String deleteroles(){
        roleList = roleService.findrolebyrid(rid);
        if(roleList.size()>0){
            flag=false;
            message="需先删除相关用户信息";
        }else{
            roleService.deleterolepre(rid);
            roleService.deleteroles(rid);
        }

        return "deleteroles";
    }

    //添加权限
    public void addrolepre(){
        Rolepre rolepre = new Rolepre();
        Roles roles = roleService.findrolesbyname(rname);
        rolepre.setModulenum(modelnum);
        rolepre.setRoles(roles);
        roleService.addrolepre(rolepre);
    }
    public void getModuleInfo(){
        ModuleInfo moduleInfo = new ModuleInfo();
    }
    public void updaterolepre(){
        Rolepre rolepre=roleService.findroleprebyrid(rid);
        Roles roles = roleService.findrolesbyrid(rid);
        rolepre.setModulenum(modelnum);
        rolepre.setRoles(roles);
        roleService.updaterolepre(rolepre);
    }

    public String findrolebynode(){
        roleList = roleService.findbynode(nodeid);
        return "findrolebynode";
    }
    public String findrolesbynode(){
        rolesList = roleService.findrolesbynode(nodeid);
        return "findrolesbynode";
    }
    public String findallrole(){
        roleList = roleService.findall();
        return "findallrole";
    }
    public String deleterole(){
        if (nodeid==1){
            flag = false;
            message = "根节点下的用户信息不能删除";
            return "deleterole";
        }
        roleService.deleterole(roleid);
        return "deleterole";
    }
    public String deletesrole(){
        Role role = roleService.findbyroleid(roleid);
        if(role.getNodetbl().getNodeid()==1){
            flag = false;
            message = "根节点下的用户信息不能删除";
            return "deletesrole";
        }
        roleService.deleterole(roleid);
        return "deletesrole";
    }
    public String updaterole(){
        Node node = new Node();
        node.setNodeid(nodeid);
        Roles roles = roleService.findrolesbyname(rname);
        Role role = new Role();
        role.setRoleid(roleid);
        role.setRolename(rolename);
        role.setRolepwd(rolepwd);
        role.setRoledesc(roledesc);
        role.setRemark(remark);
        role.setNodetbl(node);
        role.setRoles(roles);
        roleService.updaterole(role);
        return "updaterole";
    }

    public  String login() throws Exception{
            if (rolename.equals("")||rolename==null||rolepwd.equals("")||rolepwd==null) {
                flag = false;
                message="用户名或密码不能为空";
                return "login";
            }
            Role role = roleService.findRole(rolename);
            if (role==null){
                flag = false;
                message="账号不存在";
                return "login";
            }
            Boolean validate = loginvalidate(role);
            if (!validate){
                return "login";
            } else{
                saveSession(role);
                modelnum=roleService.findroleprebyrid(role.getRoles().getRid()).getModulenum();
                return "login";
            }
    }

    public boolean loginvalidate(Role role){
        String ip = getRemoteAddr();
        invalidateTimes=redisUtil.get(NAMESPACE_LOGIN+ip)==null?0:Integer.parseInt(redisUtil.get(NAMESPACE_LOGIN+ip));
        if(!role.getRolepwd().equals(rolepwd)){
            flag = false;
            invalidateTimes++;
            redisUtil.set(NAMESPACE_LOGIN+ip,String.valueOf(invalidateTimes));
            if (invalidateTimes>3){
                int ipLockedTime = 1 * 60;
                redisUtil.setex(NAMESPACE_LOGIN+ip,ipLockedTime,String.valueOf(invalidateTimes));
                message="ip已锁定,请1分钟后再试";
                return false;
            }
            if (invalidateTimes<=3){
                message="密码错误";
                return false;
            }
        }else{
            if (invalidateTimes<=3){
                redisUtil.del(NAMESPACE_LOGIN+ip);
                return true;
            }else{
                flag=false;
                message="ip已锁定,请1分钟后再试";
                log.debug("密码正确,验证次数大于3");
                return false;
            }
        }
        return true;

    }


    public HttpSession getSession() {
        try {
            return request.getSession();
        } catch (Exception e) {
            return null;
        }
    }
    private void saveSession(Role role) {
        HttpSession session = getSession();
        if (session!=null){
            session.setAttribute(ROLE_INFO,role);
        }
    }

    //获取客户端ip地址

    public String getRemoteAddr() {
        if (request.getRemoteAddr() == "127.0.0.1" || request.getRemoteAddr().equals("127.0.0.1")){
            return fetchLocIp();
        }
        else{
            return request.getRemoteAddr();
        }
    }
    public static String fetchLocIp() {
        String localAddr = "127.0.0.1";
        InetAddress[] localIpArray = null;
        try {
            localIpArray = InetAddress.getAllByName(InetAddress.getLocalHost().getHostName());
        } catch (Exception e) {
        }
        if (localIpArray != null && localIpArray.length > 0) {
            for (InetAddress localIp : localIpArray) {
                if (localIp instanceof Inet4Address) {
                    localAddr = localIpArray[0].getHostAddress();
                    break;
                }
            }
        }
        return localAddr;
    }


    public void logout(){
        HttpSession session = getSession();
        log.debug("session:"+session);
        if (session!=null) {
            session.removeAttribute(ROLE_INFO);
            session.invalidate();
        }else{
            log.debug("退出");
        }
    }

    public Integer getRoleid() {
        return roleid;
    }

    public void setRoleid(Integer roleid) {
        this.roleid = roleid;
    }

    public String getRolename() {
        return rolename;
    }

    public void setRolename(String rolename) {
        this.rolename = rolename;
    }

    public String getRolepwd() {
        return rolepwd;
    }

    public void setRolepwd(String rolepwd) {
        this.rolepwd = rolepwd;
    }

    public void setRoleService(RoleService roleService) {
        this.roleService = roleService;
    }

    public RoleService getRoleService() {
        return roleService;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

//    @Override
//    public void setRequest(Map<String, Object> request) {
//       this.request=request;
//    }
//
//    @Override
//    public void setApplication(Map<String, Object> application) {
//        this.application=application;
//    }

    public List<Role> getRoleList() {
        return roleList;
    }

    public void setRoleList(List<Role> roleList) {
        this.roleList = roleList;
    }

    public Integer getNodeid() {
        return nodeid;
    }

    public void setNodeid(Integer nodeid) {
        this.nodeid = nodeid;
    }

    public String getRoledesc() {
        return roledesc;
    }

    public void setRoledesc(String roledesc) {
        this.roledesc = roledesc;
    }

    public String getRemark() {
        return remark;
    }

    public void setRemark(String remark) {
        this.remark = remark;
    }

    public boolean isFlag() {
        return flag;
    }

    public void setFlag(boolean flag) {
        this.flag = flag;
    }

    public List<Node> getNodeList() {
        return nodeList;
    }

    public void setNodeList(List<Node> nodeList) {
        this.nodeList = nodeList;
    }

    @Override
    public void setServletRequest(HttpServletRequest request) {
        this.request = request;
    }

    @Override
    public void setServletResponse(HttpServletResponse response) {
        this.response = response;
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

    public void setSession(HttpSession session) {
        this.session = session;
    }

    public List<Roles> getRolesList() {
        return rolesList;
    }

    public void setRolesList(List<Roles> rolesList) {
        this.rolesList = rolesList;
    }

    public String getRname() {
        return rname;
    }

    public void setRname(String rname) {
        this.rname = rname;
    }

    public String getRdesc() {
        return rdesc;
    }

    public void setRdesc(String rdesc) {
        this.rdesc = rdesc;
    }

    public Integer getModelnum() {
        return modelnum;
    }

    public void setModelnum(Integer modelnum) {
        this.modelnum = modelnum;
    }

    public Integer getRid() {
        return rid;
    }

    public void setRid(Integer rid) {
        this.rid = rid;
    }

    public RedisUtil getRedisUtil() {
        return redisUtil;
    }

    public void setRedisUtil(RedisUtil redisUtil) {
        this.redisUtil = redisUtil;
    }

    public static Logger getLog() {
        return log;
    }

    public static void setLog(Logger log) {
        RoleAction.log = log;
    }
}
