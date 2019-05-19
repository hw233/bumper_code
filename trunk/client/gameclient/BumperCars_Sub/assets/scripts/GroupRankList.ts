import GlobalVar from './GlobalVar';
import WXHelper from './WXHelper';
import RankListBase from './RankListBase';

const {ccclass, property} = cc._decorator;

@ccclass
export default class GroupRankList extends RankListBase
{
    //显示排行榜
    protected showRankList(){
        var self = this;

        // var userData = null;

        //获取群同玩用户的托管数据成功
        var getGroupDataSuccess = function(res){
            if(res && res.data){

                console.log("获取群同玩用户的托管数据:", res);

                //排序
                self.dataSort(res.data);

                for(var i = 0; i < res.data.length; i++)
                { 
                    var item = cc.instantiate(self.prefabRankItem); //生成node节点
                    item.parent = self.scrollViewContent;
                    item.getComponent("RankItem").initView(i + 1, res.data[i], GlobalVar.rankType);  
                    item.y = -(i * 100);

                    if(res.data[i].avatarUrl == GlobalVar.avatarUrl)
                    {
                        var myitem = cc.instantiate(self.prefabRankItem); //生成node节点
                        res.data[i].is_rank = true;
                        myitem.parent = self.node;
                        myitem.getComponent("RankItem").initView(i + 1, res.data[i], GlobalVar.rankType, true);  
                        myitem.y = -320;
                    }
                }

                self.rankingScrollView.node.active = true;
            }
        };

        if(GlobalVar.rankType == 1)
        {
            WXHelper.GetGroupCloudData(["kill_num"], getGroupDataSuccess);
        }else if(GlobalVar.rankType == 2)
        {
            WXHelper.GetGroupCloudData(["star_num"], getGroupDataSuccess);
        }
    }

    //排序(ListData：res.data)
    protected dataSort(ListData)
    {
        for(var i = ListData.length - 1; i >= 0; i --){
            var item  = ListData[i];
            if(item){
                if(item.KVDataList.length <= 0){
                    ListData.splice(i, 1);
                    // item.KVDataList.push({key:"highest_score", value: i});
                }
            }
        }

        ListData.sort(function(a, b){
            var aScore = parseInt(a.KVDataList[0].value);
            var bScore = parseInt(b.KVDataList[0].value);

            a.score = aScore;
            b.score = bScore;

            return bScore - aScore;
        });
    }
}