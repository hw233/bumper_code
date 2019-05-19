const {ccclass, property} = cc._decorator;

@ccclass
export default class RankListBase extends cc.Component
{
    @property(cc.ScrollView)
    protected rankingScrollView: cc.ScrollView = null;

    @property(cc.Node)
    protected scrollViewContent: cc.Node = null;

    @property(cc.Prefab)
    protected prefabRankItem: cc.Prefab = null;

    protected onLoad()
    {
        if(cc.director.getWinSize().height / cc.director.getWinSize().width - 16 / 9 > 0.1)
        { 
            this.node.scaleY = cc.director.getWinSize().height / 1334;
        }
    }

    show () 
    {
        this.removeChild();

        this.showRankList();
    }

    clear()
    {
        this.removeChild();
    }

    removeChild() 
    {
        this.node.removeChildByTag(1000);
        this.scrollViewContent.removeAllChildren();
        this.rankingScrollView.node.active = false;
    }

    //显示排行榜
    protected showRankList(){
        
    }

    //排序(ListData：res.data)
    protected dataSort(ListData)
    {
        for(var i = ListData.length - 1; i >= 0; i --){
            var item  = ListData[i];
            if(item){
                if(item.KVDataList.length <= 0){
                    ListData.splice(i, 1);
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