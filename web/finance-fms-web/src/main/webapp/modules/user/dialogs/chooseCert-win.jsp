<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>

<div>
	<div class="certBox">
		<s:iterator id="certAll" value="certAllList">
			<div class="certCard" data-id="${certAll.id}">
				<p class="certName" title="${certAll.strName}">
					${certAll.strName}</p>
			</div>
		</s:iterator>
	</div>
</div>
<script>
    $(function(){
        var certIds = $('.certIds').map(function(index, elem) {
            return $(elem).val();
        }).get();
        $(".certCard").each(function(index, el) {
            var id=$(el).data('id');
            for(var i=0;i<certIds.length;i++){
                if(certIds[i]==id){
                    $(el).addClass('selected');
                }
            }
        });
        $('.certCard').on('click',function(){
            var _self = $(this);
            if(_self.hasClass('selected')){
                _self.removeClass('selected');
            }else{
                _self.addClass('selected');
            }
        })
    })
</script>