
package com.hikvision.finance.fms.common.model;

import java.util.ArrayList;
import java.util.List;

/**
 * 菜单对象
 * @author jinxindong 2016年2月22日 下午2:40:31
 * @version V1.0
 */
public class MenuBean {
	
	private String menuId;
	private String name;
	private MenuBean parent;
	private String parentId;
	private String img;
	private String url;
	private String style;
	private Integer nbtype;
	/**
	 * 菜单打开的方式，1-在新窗口中打开，2-本页面打开，3-子窗口iframe中打开
	 * 
	 */
	private int openType = 0; 
	private List<MenuBean> children;
	
	public String getMenuId() {
		return menuId;
	}
	
	public void setMenuId(String menuId) {
		this.menuId = menuId;
	}
	
	public MenuBean getParent() {
		return parent;
	}
	
	public void setParent(MenuBean parent) {
		this.parent = parent;
	}
	
	public String getName() {
		return name;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	public String getImg() {
		return img;
	}
	
	public void setImg(String img) {
		this.img = img;
	}
	
	public String getUrl() {
		return url;
	}
	
	public void setUrl(String url) {
		this.url = url;
	}
	
	public String getStyle() {
		return style;
	}
	
	public void setStyle(String style) {
		this.style = style;
	}
	
	
    public int getOpenType() {
    	return openType;
    }
	
    public void setOpenType(int openType) {
    	this.openType = openType;
    }

	public List<MenuBean> getChildren(){
		return children;
	}
	
	public void setChildren(List<MenuBean> children) {
		this.children = children;
	}
	
	public void addChild(MenuBean mb) {
		if (this.children == null) {
			this.children = new ArrayList<MenuBean>();
		}
		this.children.add(mb);
	}
	
	
	public String getParentId() {
		return parentId;
	}
	
	public void setParentId(String parentId) {
		this.parentId = parentId;
	}

	
    public Integer getNbtype() {
    	return nbtype;
    }

	
    public void setNbtype(Integer nbtype) {
    	this.nbtype = nbtype;
    }



}
