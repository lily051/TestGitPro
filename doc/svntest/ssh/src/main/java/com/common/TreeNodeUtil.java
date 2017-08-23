package com.common;


import com.userEntity.Node;

import java.util.HashMap;
import java.util.Map;

public class TreeNodeUtil {

    public static final String CONTROL_UNIT_PREFIX = "area_";
    public static final String LINE_INFO_PREFIX = "line_";
    public static final String DEVICE_INFO_PREFIX = "device_";
    public static final String CAMERA_INFO_PREFIX = "camera_";
    public static final String VEHICLE_INFO_PREFIX = "vehicle_";
    public static final String VEHTYPE_INFO_PREFIX = "vehtype_";
    public static final String WORKER_INFO_PREFIX = "worker_";
    public static final String USER_GROUP_PREFIX = "userGroup_";
    public static final String ACCESS_USER_GROUP_PREFIX = "accessUserGroup_";
    public static final String USER_INFO_PREFIX = "user_";
    public static final String SERVER_INFO_PREFIX = "server_";

    public static final String TREE_AREA = "tree_area";
    public static final String TREE_CORPORATION = "tree_corporation";
    public static final String TREE_BRANCH = "tree_branch";
    public static final String TREE_TEAM = "tree_team";
    public static final String TREE_LINE = "tree_line";
    public static final String TREE_DEVICE = "tree_device";
    public static final String TREE_DEVICE_OFFLINE = "tree_device_offline";
    public static final String TREE_CAMERA = "tree_camera";
    public static final String TREE_FOLDER = "tree_folder";
    public static final String TREE_DRIVER = "tree_driver";
    public static final String TREE_CONDUCT = "tree_conduct";
    public static final String TREE_DVR = "tree_dvr";
    public static final String TREE_DVS = "tree_dvs";
    public static final String TREE_IPC = "tree_ipc";
    public static final String TREE_USER = "tree_user";
    public static final String TREE_SERVER = "tree_server";

    public static Map<String, String> getIconMap() {
        Map<String, String> iconMap = new HashMap<String, String>();
        iconMap.put("1", TREE_AREA);
        iconMap.put("2", TREE_CORPORATION);
        iconMap.put("3", TREE_BRANCH);
        iconMap.put("4", TREE_TEAM);
        iconMap.put("5", TREE_LINE);
        iconMap.put("16", TREE_DEVICE);
        iconMap.put("17", TREE_DEVICE_OFFLINE);
        iconMap.put("18", TREE_CAMERA);
        iconMap.put("32", TREE_DVR);
        iconMap.put("33", TREE_DVS);
        iconMap.put("34", TREE_IPC);
        iconMap.put("48", TREE_FOLDER);
        iconMap.put("50", TREE_DRIVER);
        iconMap.put("51", TREE_CONDUCT);
        iconMap.put("52", TREE_USER);
        iconMap.put("60", TREE_SERVER);
        return iconMap;
    }

    public static TreeNodeInfo initTreeNodeInfo(Node controlUnit) {
        if (controlUnit == null) {
            return null;
        }
        TreeNodeInfo node = new TreeNodeInfo();
        node.setId(controlUnit.getNodeid().toString());
        node.setpId(controlUnit.getParentid().toString());
        node.setName("Root");
        return node;
    }

    public static TreeNodeInfo initTreeNodeInfo(Node controlUnit, int pId) {
        if (controlUnit == null) {
            return null;
        }
        TreeNodeInfo node = new TreeNodeInfo();
        node.setId(controlUnit.getNodeid().toString());
        node.setpId(controlUnit.getParentid().toString());
        node.setName(controlUnit.getNodename());
        return node;
    }

}
