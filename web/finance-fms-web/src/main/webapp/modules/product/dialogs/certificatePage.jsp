<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>

<div class="iScroll">
	<div class="mt10 mr20 mb10 ml20">
		<div style="margin-bottom: 10px; position:relative;width :812px;"  class="clearfix">
			<a href="#" style="_float:left;"><span class="managerOpr oprAdd oprAddCertificate"
				id="oprAddCertificate"><i>添加</i></span></a>
			<div class="header-search gird_search" style="right:0px;">
				<input type="hidden" />
				<span class="icon iconSearch" id="certSearch"></span>
				<input id="dialogSearch" name="keyWord" type="text" placeholder="请输入证书名称"
					class="searchInput form-control treeSearch" >
			</div>
		</div>
		<div class="certificateContent clearfix" id="certificateContent">
			
		</div>
	</div>

</div>

<script>
	$(function() {
		$('#dialogSearch').keydown(function(e) {
			var key = e.keyCode;
			if (key == 13) {
				$(this).prev().click();
			}
		});
		FMS.certManage.certificateManageGetUrl();
		$("#oprAddCertificate").on('click', function(event) {
			FMS.certManage.addCertificate();
		});
		$("#certSearch").on('click', function(event) {
			FMS.certManage.certificateManageGetUrl();
		});
		$("#certificateContent").on('mouseenter', '.certificateInfo', function(event) {
			$(this).find(".certOverlay").stop().animate({top:"113px"});
		}).on('mouseleave', '.certificateInfo', function(event) {
			$(this).find(".certOverlay").stop().animate({top:"139px"});
		}).on('click', '.certificateInfo', function(event) {
			//弹出详情
			$(".certificateInfo").removeClass('active');
			$(this).addClass('active');
			var title = $(this).find('.certName').text(),
				id = $(this).data("id");
			FinanceUtil.dialog({
				id:"certificateDetail",
				title:title,
				overlay:true,
				draggable:false,
				width:452,
				height:494,
				iframeFix:false,
				closeButton:false,
				show:false,
				ajax:{
					url:'/modules/product/toCertDetailPage.action',//TODO 证书详情页面
					param:{
						'crtificateInfo.id':id
					},
					success:function(){
						$("#certificateDetail").parents('.popInner').find('.popTitle,.popContent,.popButton').addClass('detailTitle');
						FinanceUtil.initFormUI("#certificateDetail");
					}
				},
				buttons:{
					"cancel": {
						text: '关闭',
						click: function() {
							$('#certificateDetail').dialog('close');
						}
					}
				}
			})
		});;
	});
	
</script>