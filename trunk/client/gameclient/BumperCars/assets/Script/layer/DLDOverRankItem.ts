import Util from "../common/Util";
import EngineUtil from "../common/EngineUtil";

import {PlayerSettleData} from "../data/net/DLDSettleData";
import PlayerMgr from "../manager/PlayerMgr";
import PlayerData from '../data/PlayerData';
import { AtlasType } from '../common/EngineUtil';

const {ccclass, property} = cc._decorator;

@ccclass
export default class OverRankItem extends cc.Component
{
    @property({type: cc.Node})
    private bgImg = null;

    @property({type: cc.Node})
    private cupImg = null;

    @property({type: cc.Node})
    private headImg = null;

    @property({type: cc.Node})
    private playerName = null;

    @property({type: cc.Node})
    private rankNum = null;

    @property({type: cc.Node})
    private killNum = null;

    @property({type: cc.Node})
    private scoreNum = null;

    
    initView(rank: number, data: PlayerSettleData){
        if(rank <= 3)
        {
            this.cupImg.active = true;
            this.rankNum.active = false;

            EngineUtil.SetNodeTextureByType(AtlasType.AT_RankListLayer, this.cupImg, "No" + rank);
        }else
        {
            this.rankNum.active = true;
            this.cupImg.active = false;
            EngineUtil.SetNodeText(this.rankNum, rank.toString());
        }

        if(PlayerMgr.IsSelf(data.userID))
        {
            EngineUtil.SetNodeTextureByType(AtlasType.AT_RankListLayer, this.bgImg, "paimingtiao-B");
        }

        let playerData: PlayerData = PlayerMgr.GetPlayer(data.userID);
        if(playerData)
        {
            //限制昵称最大为10个字符
            let nickname = Util.GetByteVal(playerData.getNickName(), 10);
            EngineUtil.SetNodeText(this.playerName, nickname);
            EngineUtil.LoadRemoteImg(this.headImg, playerData.getAvatarUrl());
        }

        EngineUtil.SetNodeText(this.killNum, data.killNum.toString());
        EngineUtil.SetNodeText(this.scoreNum, data.scoreNum.toString());
    }
}

