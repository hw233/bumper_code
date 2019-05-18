import Util from "../common/Util";
import GameLogic from "../GameLogic";
import EventCenter from "../common/EventCenter";

import {MessageType} from "../common/EventCenter";
import CarManager from '../manager/CarManager';
import PlayerMgr from '../manager/PlayerMgr';

const {ccclass, property} = cc._decorator;

@ccclass
export default class ColliderCallback extends cc.Component{

    private colliderInfo = null;

    onLoad(){
        this.colliderInfo = {};
    }

    clear(){
        this.colliderInfo = null;
        this.colliderInfo = {};
    }

    /**
     * 当碰撞产生的时候调用
     * @param  {Collider} other 产生碰撞的另一个碰撞组件
     * @param  {Collider} self  产生碰撞的自身的碰撞组件
     */
    onCollisionEnter(other, self) 
    {
        if(this.colliderInfo)
        {
            if(!this.colliderInfo[other.node.uuid])
            {
                this.colliderInfo[other.node.uuid] = [];
            }

            if(Util.IsContainElement(this.colliderInfo[other.node.uuid], self.uuid) == false)
            {
                this.colliderInfo[other.node.uuid].push(self.uuid);
            }
        }
        
        if(!other.node.collideTag)
        {
            other.node.collideTag = [];
        }

        if(other.node.collideTag)
        {
            other.node.collideTag.push(self.tag);
        }
    }

    onCollisionExit(other, self)
    {
        if(other.node.collideTag)
        {
            for(let i = 0, len = other.node.collideTag.length; i < len; i ++)
            {
                if(other.node.collideTag[i] == self.tag)
                {
                    other.node.collideTag.splice(i, 1);
                    break;
                }
            }
        }

        if(this.colliderInfo)
        {
            Util.RemoveItem(this.colliderInfo[other.node.uuid], self.uuid);
        }
    }

    update()
    {
        if(GameLogic.GetInstance().IsRunning() == false)
        {
            return;
        }

        let carManager: CarManager = GameLogic.GetInstance().GetCarManager();
        if(carManager)
        {
            let playerCount: number = PlayerMgr.GetPlayerCount()
            for(let i = 0; i < playerCount;i ++)
            {
                let playerID: number = PlayerMgr.GetPlayerIDByIndex(i);

                let carNode: cc.Node = carManager.getCarByID(playerID);
                if(carNode)
                {
                    if(this.colliderInfo[carNode.uuid] && this.colliderInfo[carNode.uuid].length <= 0)
                    {
                        if(!carNode.isCheck)
                        {
                            carNode.isCheck = true;
                            EventCenter.DispatchEvent(MessageType.Leave_Map, carNode.userID);
                        }
                    }
                }
            }
        }
    }
}