import AudioManager from "../common/AudioManager";
import EngineUtil from "../common/EngineUtil";

import {SoundType} from "../common/AudioManager";

const {ccclass, property} = cc._decorator;

@ccclass
export default class CountDownCom extends cc.Component{

    @property({type: cc.SpriteFrame})
    private bgSprite = null

    @property([cc.SpriteFrame])
    private numSprite: Array<cc.SpriteFrame> = []

    @property({type: cc.SpriteFrame})
    private goSprite = null


    private index = null
 
    play(callback: Function, isShow: boolean = true) {
        if(!this.node){
            return;
        }

        let self = this;

        this.index = 0;

        let countDownNode = EngineUtil.CreateSpriteWithFrame("CountDownNode", this.bgSprite);

        if(!countDownNode){
            return;
        }

        countDownNode.zIndex = 50;
        countDownNode.y = cc.winSize.height + countDownNode.height * 0.5;
        this.node.addChild(countDownNode);

        let numNode = EngineUtil.CreateSpriteWithFrame("NumNode", this.numSprite[this.index ++]);

        countDownNode.addChild(numNode);

        AudioManager.PlaySoundEffect(SoundType.ST_CountDown);

        let action = this.createCountDownAction(function(){
            self.schedule(function(){
                if(self.index >=  self.numSprite.length){
                    countDownNode.runAction(cc.sequence(cc.moveTo(0.2, 0, cc.winSize.height + countDownNode.height * 0.5), cc.removeSelf(true)));
                    self.unscheduleAllCallbacks();
                    callback && callback();
                    return;
                }
                if(self.index == self.numSprite.length - 1){
                    AudioManager.PlaySoundEffect(SoundType.ST_Ready);
                }
                EngineUtil.SetNodeSprteFrame(numNode, this.numSprite[this.index ++]);
            }, 1);
            // AudioManager.playDaojishiSound();
        });

        countDownNode.runAction(action);

        if(isShow == false){
            countDownNode.scale = 0;
        }else{
            countDownNode.scale = 1;
        }
    }

    createCountDownAction(callback: Function){
        let moveAction = cc.moveTo(0.2, 0, 0);
        let callfunc = cc.callFunc(callback);

        return cc.sequence(moveAction, callfunc);
    }
}
