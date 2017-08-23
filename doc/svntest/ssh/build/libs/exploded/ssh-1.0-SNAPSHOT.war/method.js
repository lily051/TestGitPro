

// 最开始的二级节点数
//url: "nodeAction!findnode.action",
           success: function nodetree(data) {
               $("#nodetree").html("");
               var level0 = data.levelist0;
               var level1 = data.levelist1;
               var level2 = data.levelist2;

               var value = level0[0];
               var str1 = "<li id='id" + value.nodeid + "' class='selectedNode'><a onclick='showAll(" + value.nodeid + ");'><span>Root</span></a></li>";
               $("#nodetree").append(str1);

               for(var i=0; i<level1.length; i++){
                  var value = level1[i];
                 // var str = "<li id='id" + value.nodeid + "'><a onclick='showAll(" + value.nodeid + ");'><span>" + value.nodename + "</span></a><span>&nbsp&nbsp&nbsp<input type='button' value='添加' onclick='addsubnode(" + value.nodeid + ")'>&nbsp<input type='button' value='删除' onclick='deletesubnode(" + value.nodeid + ")'> </span></li>"
                   var str = "<li id='id" + value.nodeid + "' class='firstLevel'><a onclick='showAll(" + value.nodeid + ");'><span>" + value.nodename + "</span></a></li>";
                   $("#nodetree").append(str);
               }
               for(var i=0; i<level2.length; i++) {
                   var value = level2[i];
                   var parentid = value.parentid;
                   //var str = "<li id='id" + value.nodeid + "' class='secondLevel'><a onclick='showAll(" + value.nodeid + ");'><span>" + value.nodename + "</span></a><span>&nbsp&nbsp&nbsp&nbsp<input type='button' value='删除' onclick='deletesubnode(" + value.nodeid + ")'> </span></li>"
                   var str = "<li id='id" + value.nodeid + "' class='secondLevel'><a onclick='showAll(" + value.nodeid + ");'><span>" + value.nodename + "</span></a></li>"
                   $("#id" + parentid).after(str);
               }
               selectTreeNode();

           }

//      function addsubnode(id){
//          nodeid=id;
//          document.getElementById("asubnode").style.display="block";
//      }
//      function subnodeadd(){
//          var nodename = $("#subnodename").val();
//          $.ajax({
//              url:"addnode.action",
//              type:"post",
//              data:{nodename:nodename,"parentid":nodeid},
//              success:function(data){
//                  if(data.flag){
//                      showTree();
//                      document.getElementById("asubnode").style.display="none";
//                  }else {
//                      $("#submsg").html(data.msg);
//                  }
//              }
//          })
//      }
//      function showsubnode() {
//          $.ajax({
//              url:"findnode.action",
//              type:"post",
//              success:function (data) {
//                  $("#nodes").html("");
//                  $.each(data, function(n, value){
//                      var str = "<li id='li"+value.id+"'><a onclick='showAll("+value.nodeid+");'><span>" + value.nodename+"</span></a><span>&nbsp&nbsp&nbsp&nbsp<input type='button' value='删除' onclick='deletesubnode("+value.nodeid+")'> </span></li>"
//                      $("#nodes").append(str);
//                  });
//              }
//          })
//      }
//    function shownode(){
//        $.ajax({
//            url:"findnode.action",
//            type:"post",
//            success:function (data) {
//                $("#nodes").html("");
//                $.each(data, function(n, value){
//                    console.log(value.level);
//                    var str = "<li id='li"+value.id+"'><a onclick='showAll("+value.nodeid+");'><span>" + value.nodename+"</span></a><span>&nbsp&nbsp&nbsp<input type='button' value='添加' onclick='addsubnode("+value.nodeid+")'>&nbsp<input type='button' value='删除' onclick='deletesubnode("+value.nodeid+")'> </span></li>";
//                    $("#nodes").append(str);
//                });
//            }
//        })
//    }

//    function updateuser(id){
//        $.ajax({
//            url:"update.action",
//            type:"post",
//           // data:{"id":user.id,"name":user.name,"age":user.age,"phonenum":user.phonenum,"address":user.address},
//            data:{"id":id},
//            success:function(){
//                window.open('updateuser.jsp?id='+id,'newwindow','height=400,width=200,top=0,left=0');
//            }
//        })
//window.open('updateuser.jsp','newwindow','height=400,width=200,top=0,left=0');
//}

function selectTreeNode(){
    $("#nodetree li").click(function(){
        $("#nodetree li").removeClass("selectedNode");
        $(this).addClass("selectedNode");
        nodeid = $(".selectedNode").attr("id").substr(2,$(".selectedNode").attr("id").length);
        nodename = $(".selectedNode").find("span").html();
        console.log(nodename);
    });
}
function logsuccess(){
    $.ajax({
        url:"roleAction!logsuccess.action",
        type:"post",
        data:{modelnum:modelnum}
    })
}