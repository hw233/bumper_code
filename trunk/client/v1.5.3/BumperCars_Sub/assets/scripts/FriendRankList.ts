import RankListBase from './RankListBase';
import GlobalVar from './GlobalVar';
import WXHelper from './WXHelper';
const {ccclass, property} = cc._decorator;

//好友排行榜
@ccclass
export default class FriendRankList extends RankListBase
{
     //显示排行榜
    protected showRankList()
    {
        var self = this;

        //获取用户好友的托管数据成功
        var getFriendDataSuccess = function(res){
            if(res && res.data){

                console.log("获取用户好友的托管数据:", res);

                //排序
                self.dataSort(res.data);

                var selfIndex = -1;

                for(var i = 0; i < 30; i++){ 
                    if(res.data.length <= i){
                        break;
                    }
                    
                    var item = cc.instantiate(self.prefabRankItem); //生成node节点
                    item.parent = self.scrollViewContent;
                    item.getComponent("RankItem").initView(i + 1, res.data[i], GlobalVar.rankType);  
                    item.y = -(i * 100);

                    if(res.data[i].avatarUrl == GlobalVar.avatarUrl)
                    {
                        selfIndex = i;
                    }
                }

                var selfData = self.getSelfData(res.data);
                if(selfData){
                    selfData.is_rank = (selfIndex != -1);
                    var myitem = cc.instantiate(self.prefabRankItem); //生成node节点
                    myitem.parent = self.node;
                    myitem.getComponent("RankItem").initView(selfIndex + 1, selfData, GlobalVar.rankType, true);  
                    myitem.y = -320;
                }

                self.rankingScrollView.node.active = true;
            }
        };

        if(GlobalVar.rankType == 1)
        {
            WXHelper.GetFriendCloudData(["kill_num"], getFriendDataSuccess);
        }else if(GlobalVar.rankType == 2)
        {
            WXHelper.GetFriendCloudData(["star_num"], getFriendDataSuccess);
        }
    }

    private getSelfData(data)
    {
        for(var i = 0; i < data.length; i++)
        { 
            if(data[i].avatarUrl == GlobalVar.avatarUrl){
                return data[i];
            }
        }
        return null;
    }

    //排序(ListData：res.data)
    protected dataSort(ListData){
        ListData.sort(function(a, b){
            var aScore = parseInt(a.KVDataList[0].value);
            var bScore = parseInt(b.KVDataList[0].value);

            a.score = aScore;
            b.score = bScore;

            return bScore - aScore;
        });
    }
}