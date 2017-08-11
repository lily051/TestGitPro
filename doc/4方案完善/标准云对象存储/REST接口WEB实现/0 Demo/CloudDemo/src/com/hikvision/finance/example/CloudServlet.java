package com.hikvision.finance.example;

import java.io.IOException;
import java.io.PrintWriter;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import net.sf.json.JSONObject;
import com.hikvision.finance.example.model.Cloud;
import com.hikvision.finance.example.model.File;
import com.hikvision.finance.example.util.CloudUtil;
import com.hikvision.ivms.hikcstor.service.Bucket;

public class CloudServlet extends HttpServlet {
    private static final long serialVersionUID = 1L;
    private static final List<File> fileList = new ArrayList<File>();

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doPost(request, response);
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// 1，获取参数
		String fuc = request.getParameter("fuc");
		String ip = request.getParameter("ip");
		String port = request.getParameter("port");
		String accesskey = request.getParameter("accesskey");
		String secretkey = request.getParameter("secretkey");
		Cloud cloud = new Cloud(ip, port, accesskey, secretkey);
		String fileName = request.getParameter("fileName");
		String fileDisplayName = request.getParameter("fileDisplayName");
		String bucket = request.getParameter("bucket");
		// 2，调用方法
		if("getBucketData".equals(fuc)) {
			getBucketData(cloud, response);
		} else if("getUploadInfo".equals(fuc)) {
			getUploadInfo(cloud, bucket, fileName, fileDisplayName, response);
		} else if("getDownloadInfo".equals(fuc)) {
			getDownloadInfo(fileName, bucket, response);
		} else if("deleteFile".equals(fuc)) {
			deleteFile(fileName, bucket, response);
		} else if("getFileList".equals(fuc)) {
			getFileList(response);
		} else if("uploadFile".equals(fuc)) {
			uploadFile(new File(cloud, fileName, fileDisplayName, bucket), response);
		}
	}
	
	/**
	 * 标准云JAVA接口：获取Bucket数据
	 * @author gengqiaolei
	 * @param cloud
	 * @param response
	 */
	// TODO
	private void getBucketData(Cloud cloud, HttpServletResponse response) {
		JSONObject json = new JSONObject();
		try {
			List<Bucket> bucketList = CloudUtil.getJBucketService(cloud).listBuckets();
			if(null != bucketList && 0 < bucketList.size()) {
				StringBuilder sb = new StringBuilder();
				for(Bucket bucket:bucketList) {
					DecimalFormat df = new DecimalFormat("#0.00");
					sb.append("<input type='radio' name='bucket' value='" + bucket.getName() + "'>");
					sb.append(bucket.getName());
					sb.append("(");
					sb.append("总容量" + bucket.getSize() + "GB");
					sb.append("剩余容量" + df.format(Double.valueOf(bucket.getFreeSize())) + "GB");
					sb.append(")");
					sb.append("<br />");
				}
				json.put("msg", sb.toString());
			} else {
				json.put("msg", "获取Bucket数据为空");
			}
		} catch(Exception e) {
			json.put("msg", "获取Bucket数据失败");
			e.printStackTrace();
		}
		outJson(json, response);
	}
	
	/**
	 * 标准云REST接口：获取上传信息
	 * @author gengqiaolei
	 * @param cloud
	 * @param bucket
	 * @param fileName
	 * @param fileDisplayName
	 * @param response
	 */
	// TODO
	@SuppressWarnings("deprecation")
    private void getUploadInfo(Cloud cloud, String bucket, String fileName, String fileDisplayName, HttpServletResponse response) {
		JSONObject json = new JSONObject();
		try {
			Date date = new Date();
			String stringToSign = CloudUtil.getStringToSign("POST", date, bucket, fileName, fileDisplayName);
			String signature = CloudUtil.getSignature(stringToSign, cloud.getSecretkey()); // 签名
			String bestnode = CloudUtil.getJObjectService(cloud).getBestNode(bucket, fileName);
			json.put("success", true);
			json.put("accessKey", cloud.getAccesskey());
			json.put("Signature", signature);
			json.put("date", date.toGMTString());
			json.put("bestnode", bestnode); // 最佳节点
			json.put("bucket", bucket);
		} catch(Exception e) {
			json.put("success", false);
			json.put("msg", "获取上传信息失败");
			e.printStackTrace();
		}
		outJson(json, response);
	}
	
	/**
	 * 标准云REST接口：获取下载信息
	 * @author gengqiaolei
	 * @param fileName
	 * @param bucket
	 * @param response
	 */
	// TODO
	private void getDownloadInfo(String fileName, String bucket, HttpServletResponse response) {
		JSONObject json = new JSONObject();
		try {
			for(File file:fileList) {
				if(fileName.equals(file.getFileName()) && bucket.equals(file.getBucket())) {
					String[] strArray = CloudUtil.getDownLoadUrl(fileName, file.getFileDisplayName(), bucket, file.getCloud());
					json.put("success", true);
					json.put("url", strArray[0]);
					json.put("token", strArray[1]);	
					json.put("filename", strArray[2]);
					break;
				}
			}
		} catch(Exception e) {
			json.put("success", false);
			json.put("msg", "获取下载信息异常");
			e.printStackTrace();
		}
		outJson(json, response);
	}
	
	/**
	 * 标准云JAVA接口：删除文件
	 * @author gengqiaolei
	 * @param fileName
	 * @param bucket
	 * @param response
	 */
	// TODO
	private void deleteFile(String fileName, String bucket, HttpServletResponse response) {
		JSONObject json = new JSONObject();
		try {
			boolean isDelete = false;
			for(int i = 0; i < fileList.size(); i++) {
				File file = fileList.get(i);
				if(fileName.equals(file.getFileName()) && bucket.equals(file.getBucket())) {
					if(!isDelete) {
						int result = CloudUtil.getJObjectService(file.getCloud()).deleteObject(bucket, fileName);
						if(1 == result) {
							json.put("success", true);
							isDelete = true;
						} else {
							json.put("success", false);
							json.put("msg", "删除云存储文件失败");
							break;
						}
					}
					fileList.remove(file);
					i--;
				}
			}
		} catch(Exception e) {
			json.put("success", false);
			json.put("msg", "删除云存储文件异常");
			e.printStackTrace();
		}
		outJson(json, response);
	}
	
	/**
	 * 获取文件列表
	 * @author gengqiaolei
	 * @param response
	 */
	private void getFileList(HttpServletResponse response) {
		JSONObject json = new JSONObject();
		StringBuilder sb = new StringBuilder();
		for(File file:fileList) {
			sb.append("<input type='radio' name='file' value='" + file.getFileName() + "' bucket='" + file.getBucket() + "'>");
			sb.append(file.getFileDisplayName());
			sb.append("<br />");
		}
		json.put("fileList", sb.toString());
		outJson(json, response);
	}
	
	/**
	 * 上传文件（至系统）
	 * @author gengqiaolei
	 * @param file
	 * @param response
	 */
	private void uploadFile(File file, HttpServletResponse response) {
		fileList.add(file);
		outJson(new JSONObject(), response);
	}
	
	/**
	 * 输出Json数据
	 * @author gengqiaolei
	 * @param obj
	 * @param response
	 */
	private void outJson(JSONObject obj, HttpServletResponse response) {
		PrintWriter pw = null;
		try {
			response.setHeader("content-length", String.valueOf(obj.toString().getBytes("UTF-8").length));
			response.setContentType("text/html;charset=UTF-8");
			pw = response.getWriter();
			pw.write(obj.toString());
		} catch(IOException e) {
			e.printStackTrace();
		} finally {
			if(null != pw) {
				pw.close();
			}
		}
	}
}