package com.hikvision.finance.fms.common.excel;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.List;
import java.util.Map;

import org.apache.poi.hssf.usermodel.HSSFDataValidationHelper;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.ss.usermodel.DataValidationHelper;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.hikvision.finance.fms.common.excel.model.ExcelHead;

public class HSSFExcel implements IPoiExcelOperation {
	
	private Workbook wb;
	private DataValidationHelper dataValidationHelper;
	
	public HSSFExcel() {
	}
	
	public Workbook readExcel(InputStream s) throws Exception {
		HSSFWorkbook wb = new HSSFWorkbook(s);
		this.wb = wb;
		return wb;
	}
	
	public Workbook readExcel(String filePath) throws Exception {
		FileInputStream fis = new FileInputStream(new File(filePath));
		return readExcel(fis);
	}
	
	public Workbook createExcel(String excelName, List<Map<Object, Object>> dataList, List<ExcelHead> headList) {
		Workbook wb = new HSSFWorkbook();
		return wb;
	}
	
	private DataValidationHelper createdataValidationHelper(Sheet sheet) {
		DataValidationHelper dataValidationHelper = new HSSFDataValidationHelper((HSSFSheet)sheet);
		this.dataValidationHelper = dataValidationHelper;
		return dataValidationHelper;
	}
	
	private Workbook createWorkbook() {
		return new HSSFWorkbook();
	}
	
	public Workbook getWorkbook() {
		if (wb == null) {
			return createWorkbook();
		} else {
			return wb;
		}
	}
	
	public DataValidationHelper getdataValidationHelper(Sheet sheet) {
		if (dataValidationHelper == null) {
			return createdataValidationHelper(sheet);
		} else {
			return dataValidationHelper;
		}
	}
	
}
