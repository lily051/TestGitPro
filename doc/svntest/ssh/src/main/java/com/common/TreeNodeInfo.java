package com.common;

import java.io.Serializable;

public class TreeNodeInfo implements Serializable {

    private static final long serialVersionUID = -7248193506291769095L;
    private String nodeId;
    private String id;
    private String pId;
    private String name;
    private String rawName;
    private boolean open;
    private String iconSkin;
    private boolean isParent;
    private boolean checked;
    private String deviceId;
    private String parentName;
    private int typecode;
    private String groupNodeId;
    private int collecting;
    private boolean halfCheck;
    private int channnum;
    private boolean nocheck;
    private boolean expand;
    private int photoFileId;
    private int limittime;

    public int getLimittime() {
		return limittime;
	}

	public void setLimittime(int limittime) {
		this.limittime = limittime;
	}

	public String getNodeId() {
        return nodeId;
    }

    public void setNodeId(String nodeId) {
        this.nodeId = nodeId;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getpId() {
        return pId;
    }

    public void setpId(String pId) {
        this.pId = pId;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getRawName() {
        return rawName;
    }

    public void setRawName(String rawName) {
        this.rawName = rawName;
    }

    public boolean isOpen() {
        return open;
    }

    public void setOpen(boolean open) {
        this.open = open;
    }

    public String getIconSkin() {
        return iconSkin;
    }

    public void setIconSkin(String iconSkin) {
        this.iconSkin = iconSkin;
    }

    public boolean getIsParent() {
        return isParent;
    }

    public void setIsParent(boolean isParent) {
        this.isParent = isParent;
    }

    public boolean isChecked() {
        return checked;
    }

    public void setChecked(boolean checked) {
        this.checked = checked;
    }

    public String getDeviceId() {
        return deviceId;
    }

    public void setDeviceId(String deviceId) {
        this.deviceId = deviceId;
    }

    public String getParentName() {
        return parentName;
    }

    public void setParentName(String parentName) {
        this.parentName = parentName;
    }

    public int getTypecode() {
        return typecode;
    }

    public void setTypecode(int typecode) {
        this.typecode = typecode;
    }

    public String getGroupNodeId() {
        return groupNodeId;
    }

    public void setGroupNodeId(String groupNodeId) {
        this.groupNodeId = groupNodeId;
    }

    public int getCollecting() {
        return collecting;
    }

    public void setCollecting(int collecting) {
        this.collecting = collecting;
    }

    public boolean isHalfCheck() {
        return halfCheck;
    }

    public void setHalfCheck(boolean halfCheck) {
        this.halfCheck = halfCheck;
    }

    public int getChannnum() {
        return channnum;
    }

    public void setChannnum(int channnum) {
        this.channnum = channnum;
    }

    public boolean isNocheck() {
        return nocheck;
    }

    public void setNocheck(boolean nocheck) {
        this.nocheck = nocheck;
    }

    public boolean isExpand() {
        return expand;
    }

    public void setExpand(boolean expand) {
        this.expand = expand;
    }

    public int getPhotoFileId() {
        return photoFileId;
    }

    public void setPhotoFileId(int photoFileId) {
        this.photoFileId = photoFileId;
    }
}
