<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<% if(!"notShowData".equals(request.getParameter("notShowData"))&&Integer.parseInt(request.getParameter("total"))<=0){ %>
<div class="msg-weak msg-tips nodata">
    <i></i>
    <div class="msg-cnt">
        <span>暂无数据！</span>
    </div>
</div>
<% } %>
<div class="pagination <%if("true".equals(request.getParameter("fixed"))){%>fixed<%}%>"  pageNo="${param.pageNo}" totalPage="${param.totalPage}">
    <div class="amount">
        <span>共<em>${param.total}</em>条</span>
        <span><%if(Integer.parseInt(request.getParameter("totalPage"))<1){ %>
            <em>0/0</em>页<%} else { %>
            <em>${param.pageNo}/${param.totalPage}</em>页<%} %>
        </span>
        <span>每页显示
            <select name="limit" page-type="page-size" useLimitCookie="${param.useLimitCookie}"<%if(Integer.parseInt(request.getParameter("totalPage"))<1){ %> disabled="disabled"<%} %>>
                <s:iterator value="{10,20,30,50,100}" id='numberLimit'>
                    <option value="${numberLimit}"
                        <s:if test="#request.numberLimit==limit">
                            selected
                        </s:if>
                        >${numberLimit}
                    </option>
                </s:iterator>
            </select>
            条
        </span>
    </div>
    <ul class="pages">
        <li class="first">
            <a page-type="first" href="javascript:void(0);"<%if(Integer.parseInt(request.getParameter("pageNo"))<2){ %> class="disabled" disabled="disabled"<%} %>>首页</a>
        </li>
        <li class="prev">
            <a page-type="prev" href="javascript:void(0);"<%if(Integer.parseInt(request.getParameter("pageNo"))<2){ %> class="disabled" disabled="disabled"<%} %>>上一页</a>
        </li>
        <li class="next">
            <a page-type="next" href="javascript:void(0);"<%if(Integer.parseInt(request.getParameter("pageNo"))>Integer.parseInt(request.getParameter("totalPage"))-1){ %> class="disabled" disabled="disabled"<%} %>>下一页</a>
        </li>
        <li class="last">
            <a page-type="last" href="javascript:void(0);"<%if(Integer.parseInt(request.getParameter("pageNo"))>Integer.parseInt(request.getParameter("totalPage"))-1){ %> class="disabled" disabled="disabled"<%} %>>尾页</a>
        </li>
        <li>
            <%if("false".equals(request.getParameter("defaultForm"))){ %>
            <div>
                到第 
                <input type="text" vtype="num" page-type="jump" _name="start" value="${start}" class="jumpTo">
                <input type="hidden" name="start" class="jumpTo">
                页 
                <input type="button" page-type="jump" value="跳转"<%if(Integer.parseInt(request.getParameter("totalPage"))<1){ %> class="jumpToBtn disabled" disabled="disabled"<%}else{ %> class="jumpToBtn" <%} %>>
            </div>
            <%}else{ %>
            <form method="post">
                到第 
                <input type="text" vtype="num" page-type="jump" _name="start" value="${start}" class="jumpTo">
                <input type="hidden" name="start" class="jumpTo">
                页 
                <input type="button" page-type="jump" value="跳转"<%if(Integer.parseInt(request.getParameter("totalPage"))<1){ %> class="jumpToBtn disabled" disabled="disabled"<%}else{ %> class="jumpToBtn" <%} %>>
            </form>
            <%} %>
        </li>
    </ul>
</div>
