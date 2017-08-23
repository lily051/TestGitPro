package com.common;

public class ModuleInfo implements java.io.Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 3320579587333920428L;
	private String moduleName;
	private Integer modulenum;
	private String moduleIcon;
	private Integer visible;

	public ModuleInfo() {
	}

	public ModuleInfo( String moduleName, Integer modulenum, String moduleIcon, Integer visible) {
		this.moduleName = moduleName;
		this.modulenum = modulenum;
		this.moduleIcon = moduleIcon;
		this.visible = visible;
	}

	public ModuleInfo clone() {
		ModuleInfo moduleInfo = new ModuleInfo(this.moduleName, this.modulenum, this.moduleIcon, this.visible);
		return moduleInfo;
	}


	public String getModuleName() {
		return moduleName;
	}

	public void setModuleName(String moduleName) {
		this.moduleName = moduleName;
	}

	public Integer getModulenum() {
		return modulenum;
	}

	public void setModulenum(Integer modulenum) {
		this.modulenum = modulenum;
	}


	public String getModuleIcon() {
		return moduleIcon;
	}

	public void setModuleIcon(String moduleIcon) {
		this.moduleIcon = moduleIcon;
	}

	public Integer getVisible() {
		return visible;
	}

	public void setVisible(Integer visible) {
		this.visible = visible;
	}

}
