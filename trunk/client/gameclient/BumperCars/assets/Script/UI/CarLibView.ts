import AccountData from '../manager/AccountData';
import AdvertiseMgr from '../Advertise/AdvertiseMgr';
import AudioManager from '../common/AudioManager';
import CarItemView from '../component/CarItemView';
import CarLibItemView from '../component/CarLibItemView';
import CarLibMotionCom from '../component/CarLibMotionCom';
import CarTableInfo from '../table/CarTableInfo';
import EngineUtil from '../common/EngineUtil';
import GameTable from '../manager/TableManger';
import GameUI from '../manager/UIManager';
import PopUpBase from './PopUpBase';
import ShowCarView from '../component/ShowCarView';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import WXHelper from '../common/WXHelper';
import { AdvertiseType } from '../Advertise/AdvertiseMgr';
import { CarUnlockCon } from '../GameDefine';
import { GradingSystem } from '../Grading/GradingSystem';
import { MessageType } from '../common/EventCenter';
import { TableType } from '../table/TableDefine';
import { UIType } from './UIDefine';
import MotionTableInfo from '../table/MotionTableInfo';
import ItemSystem from '../Item/ItemSystem';
import ItemData from '../Item/ItemData';
import { AtlasType } from '../common/EngineUtil';
import { ItemType } from '../Item/ItemDefine';
import GameJump from '../GameJump';
import TaskSystem from '../Task/TaskSystem';
import ItemManager from '../Item/ItemManager';
import Util from '../common/Util';

const {ccclass, property} = cc._decorator;

@ccclass
export default class CarLibView extends PopUpBase
{
    @property({type: cc.Node})
    private carToggleNode: cc.Node              = null;

    @property({type: cc.Node})
    private motionToggleNode: cc.Node           = null;

    @property(cc.Node)
    private carListNode: cc.Node                = null;

    @property(cc.Node)
    private motionListNode: cc.Node             = null;

    @property(cc.Node)
    private remainTimeNode: cc.Node             = null;

    @property(cc.Node)
    private remainTime: cc.Node                 = null;

    @property(ShowCarView)
    private inShowCarView: ShowCarView          = null;

    @property(cc.Node)
    private carNameNode: cc.Node                = null;

    @property(cc.Node)
    private speedNode: cc.Node                  = null;

    @property(cc.Node)
    private powerNode: cc.Node                  = null;

    @property(cc.Node)
    private conNode: cc.Node                    = null;

    @property(cc.Node)
    private tipNode: cc.Node                    = null;

    @property(cc.Node)
    private btnNode: cc.Node                    = null;

    @property(cc.Node)
    private grandingNode: cc.Node               = null;    

    //当前选中的车辆id
    private curCarID: number                    = null;

    //当前选中的拖尾id
    private curMotionID: number                 = null;

    //1：车 2：拖尾
    private type: number                        = null;

    private tryItemID: number                   = null;

    private tryGameID: number                   = null;

    public OnInit () 
    {
        super.OnInit();

        this.type           = 1;
        this.curCarID       = 0;
        this.curMotionID    = 0;

        this.registerEvent(MessageType.CarLibBtnClicked, this.carItemClicked.bind(this));
        this.registerEvent(MessageType.CarLibMotionClicked, this.motionItemClicked.bind(this));
        this.registerEvent(MessageType.CarLib_Unlock_Item_Suc, this.unlockItemSuc.bind(this));
        this.registerEvent(MessageType.CarLib_Unlock_Item_Fail, this.unlockItemFail.bind(this));
        this.registerEvent(MessageType.Use_Car_Suc, this.useCarSuc.bind(this));
        this.registerEvent(MessageType.Use_Motion_Suc, this.useMotionSuc.bind(this));
        this.registerEvent(MessageType.CarLib_RefreshView, this.refreshView.bind(this));

        this.registerClickEvent(this.btnNode, this.btnClicked.bind(this));

        this.registerTouchEndEvent(this.carToggleNode, this.carToggleClicked.bind(this));
        this.registerTouchEndEvent(this.motionToggleNode, this.motionToggleClicked.bind(this));

        this.carListNode.active = true;
        this.motionListNode.active = false;
    }

    public OnEnter()
    {
        super.OnEnter();

        this.registerEvent(MessageType.MiniProgramTryPlaySuc, this.tryPlaySuc.bind(this));
        this.registerEvent(MessageType.Item_Update_LiftTime_Event, this.refreshRemainTime.bind(this));
    }

    public OnExit()
    {
        super.OnExit();

        this.removeEvent(MessageType.MiniProgramTryPlaySuc);
        this.removeEvent(MessageType.Item_Update_LiftTime_Event);
    }

    public InitView()
    {
        if(this.carListNode)
        {
            let content = this.carListNode.getComponent(cc.ScrollView).content;
            if(content)
            {

                let carNum: number = GameTable.GetInstance().GetDataCount(TableType.TTD_CarType);
                for(let i: number = 0, len = carNum; i < len; i ++)
                {
                    let carInfo: CarTableInfo = GameTable.GetInstance().GetDataInfoByIndex(TableType.TTD_CarType, i);
                    if(carInfo)
                    {
                        let carID: number = carInfo.GetID();
                        let carItemPrefab: cc.Prefab = EngineUtil.GetPrefab("CarLibItemView");
                        let carItem = cc.instantiate(carItemPrefab);
                        if(carItem)
                        {
                            content.addChild(carItem);
                            let carItemView: CarItemView = carItem.getComponent("CarItemView");
                            if(carItemView)
                            {
                                carItemView.refreshView(carID);
                            }
                        }
                    }
                }
            }
        }

        if(this.motionListNode)
        {

            let content = this.motionListNode.getComponent(cc.ScrollView).content;
            if(content)
            {
                let count: number = GameTable.GetInstance().GetDataCount(TableType.TTD_MotionType);

                for(let i: number = 0, len = count; i < len; i ++)
                {
                    let motionInfo: MotionTableInfo = GameTable.GetInstance().GetDataInfoByIndex(TableType.TTD_MotionType, i);
                    if(motionInfo)
                    {
                        let motionPrefab: cc.Prefab = EngineUtil.GetPrefab("MotionNode");
                        let motionNode = cc.instantiate(motionPrefab);
                        if(motionNode)
                        {
                            content.addChild(motionNode);
                            let motionCom: CarLibMotionCom = motionNode.getComponent("CarLibMotionCom");
                            if(motionCom)
                            {
                                motionCom.initView(motionInfo.GetID());
                            }
                        }
                    }
                }
            }
        }
    }

    public RefreshView()
    {
        super.RefreshView();

        this.curCarID = this.param;

        this.curMotionID = AccountData.GetInstance().GetUsedMotionID();

        this.refreshShowCar();
        this.refreshShowMotion();

        if(this.type === 1)
        {
            //跳至当前车辆
            this.scroolToCurCar();

            this.showCarView();
        }
        else if(this.type === 2)
        {
            this.showMotionView();
        }
    }

    public OnDestroy()
    {
        super.onDestroy();
    }

    private refreshView()
    {
        if(this.type === 1)
        {
            this.showCarView();
        }
        else if(this.type == 2)
        {
            this.showMotionView();
        }
    }

    private scroolToCurCar()
    {
        let carInfo: CarTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(this.curCarID);
        if(carInfo)
        {
            // let content = this.motionListNode.getComponent(cc.ScrollView).content;
            // if(content && carInfo.GetImgID() >= 3)
            // {
            //     content.x -= (carInfo.GetImgID() - 1) * 150;
            // }
            let xOffect = (Number(carInfo.GetID().toString().substring(2, 5))) /  11;

            this.carListNode.getComponent(cc.ScrollView).scrollToPercentHorizontal(xOffect, 0);
        }
    }

    private carToggleClicked()
    {
        this.type = 1;

        this.showCarView();
    }

    private motionToggleClicked()
    {
        this.type = 2;

        this.showMotionView();
    }

    private showCarView()
    {
        this.carListNode.active = true;
        this.motionListNode.active = false;

        this.refreshCarList();
        this.refreshCarAttrb();
        this.refreshShowCar();
        this.refreshRemainTimeUI();
    }

    private showMotionView()
    {
        this.carListNode.active = false;
        this.motionListNode.active = true;

        this.refreshMotionList();
        this.refreshMotionAttrb();
        this.refreshShowMotion();
        this.refreshRemainTimeUI();
    }

    private carItemClicked(carID: number)
    {
        this.curCarID = carID;

        this.refreshShowCar();
        this.refreshCarAttrb();
        this.refreshRemainTimeUI();
    }

    private motionItemClicked(motionID: number)
    {
        this.curMotionID = motionID;

        this.refreshShowMotion();
        this.refreshMotionAttrb();
        this.refreshRemainTimeUI();
    }

    //刷新车的属性
    private refreshCarAttrb()
    {
        let carInfo: CarTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(this.curCarID);
        if(carInfo)
        {
            this.btnNode.y = 0;
            this.conNode.active = false;
            this.tipNode.active = false;
            this.grandingNode.active = false;

            let usingCar: number = AccountData.GetInstance().GetUseCarID();

            if(usingCar === this.curCarID)
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, this.btnNode, "CarLib_Btn_CZZ");
            }
            else
            {
                if(this.isUnlock())
                {
                    EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, this.btnNode, "CarLib_Btn_CZ");
                }
                else
                {
                    if(carInfo.GetConditionID() == CarUnlockCon.CUC_ZuanShi5)
                    {
                        this.btnNode.y = -50;
                        this.grandingNode.active = true;
                        EngineUtil.SetNodeText(this.grandingNode.getChildByName("Name"), "当前段位:" + GradingSystem.GetGradingName(AccountData.GetInstance().GetCurLevelID()));
                    }
                    else
                    {
                        let tipIcon: cc.Node = this.tipNode.getChildByName("Icon");
                        let tipNum: cc.Node = this.tipNode.getChildByName("Num");
                        if(carInfo.GetConditionID() == CarUnlockCon.CUC_Gold || carInfo.GetConditionID() == CarUnlockCon.CUC_Debris)
                        {
                            EngineUtil.SetNodeText(tipNum, carInfo.GetValue().toString());
                            tipIcon.x = -40;
                            tipNum.active = true;
                        }
                        else
                        {
                            tipIcon.x = 0;
                            tipNum.active = false;
                        }
                        this.btnNode.y = -50;
                        this.tipNode.active = true;
                        EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, tipIcon, "CarLib_Tip" + carInfo.GetConditionID());
                    }
                    this.conNode.active = true;
                    EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, this.btnNode, "CarLib_Btn_" + carInfo.GetConditionID());
                    EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, this.conNode, "CarLib_Unlock_Con_" + carInfo.GetConditionID());
                }
            }

            EngineUtil.SetNodeText(this.speedNode, "+" + carInfo.GetSpeed() + "%");
            EngineUtil.SetNodeText(this.powerNode, "+" + carInfo.GetPower() + "%");
            
            EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, this.carNameNode, "CarLib_CarName_" + carInfo.GetImgID());
        }
    }

    //刷新拖尾的属性
    private refreshMotionAttrb()
    {
        let motionInfo: MotionTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(this.curMotionID);
        if(motionInfo)
        {
            this.btnNode.y = 0;
            this.conNode.active = false;
            this.tipNode.active = false;
            this.grandingNode.active = false;

            let usingCar: number = AccountData.GetInstance().GetUsedMotionID();
            if(usingCar === this.curMotionID)
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, this.btnNode, "CarLib_Btn_CZZ");
            }
            else
            {
                if(this.isUnlock())
                {
                    EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, this.btnNode, "CarLib_Btn_CZ");
                }
                else
                {
                    if(motionInfo.GetConditionID() == CarUnlockCon.CUC_ZuanShi5)
                    {
                        this.btnNode.y = -50;
                        this.grandingNode.active = true;
                        EngineUtil.SetNodeText(this.grandingNode.getChildByName("Name"), "当前段位:" + GradingSystem.GetGradingName(AccountData.GetInstance().GetCurLevelID()));
                    }
                    else
                    {
                        let tipIcon: cc.Node = this.tipNode.getChildByName("Icon");
                        let tipNum: cc.Node = this.tipNode.getChildByName("Num");
                        if(motionInfo.GetConditionID() == CarUnlockCon.CUC_Gold || motionInfo.GetConditionID() == CarUnlockCon.CUC_Debris)
                        {
                            EngineUtil.SetNodeText(tipNum, motionInfo.GetValue().toString());
                            tipIcon.x = -40;
                            tipNum.active = true;
                        }
                        else
                        {
                            tipIcon.x = 0;
                            tipNum.active = false;
                        }
                        this.btnNode.y = -50;
                        this.tipNode.active = true;
                        EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, tipIcon, "CarLib_Tip" + motionInfo.GetConditionID());
                    }

                    this.conNode.active = true;
                    EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, this.btnNode, "CarLib_Btn_" + motionInfo.GetConditionID());
                    EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, this.conNode, "CarLib_Unlock_Con_" + motionInfo.GetConditionID());
                }
            }

            EngineUtil.SetNodeText(this.speedNode, "+" + motionInfo.GetSpeed() + "%");
            EngineUtil.SetNodeText(this.powerNode, "+" + motionInfo.GetPower() + "%");
            EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, this.carNameNode, "MotionName2_" + motionInfo.GetImgID());
        }
    }

    //刷新显示中的拖尾
    private refreshShowMotion()
    {
        if(this.inShowCarView)
        {
            this.inShowCarView.refreshMotion(this.curMotionID);
        }
    }

    //刷新显示中的车
    private refreshShowCar()
    {
        if(this.inShowCarView)
        {
            this.inShowCarView.refreshCar(this.curCarID);
        }
    }

    //刷新车列表
    private refreshCarList()
    {
        let content = this.carListNode.getComponent(cc.ScrollView).content;
        if(content)
        {
            for(let i = 0; i < content.childrenCount; i ++)
            {
                let child: cc.Node = content.children[i];
                if(child)
                {
                    let carItemView: CarLibItemView = child.getComponent(CarLibItemView);
                    if(carItemView)
                    {
                        carItemView.refreshView();
                    }
                }
            }
        }
    }

    //刷新拖尾列表
    private refreshMotionList()
    {
        let content = this.motionListNode.getComponent(cc.ScrollView).content;
        if(content)
        {
            for(let i = 0; i < content.childrenCount; i ++)
            {
                let child: cc.Node = content.children[i];
                if(child)
                {
                    let motionItemView: CarLibMotionCom = child.getComponent(CarLibMotionCom);
                    if(motionItemView)
                    {
                        motionItemView.refreshView();
                    }
                }
            }
        }
    }

    private btnClicked(): void
    {
        AudioManager.PlayBtnSound();

        if(this.type === 1)
        {
            let usingCar: number = AccountData.GetInstance().GetUseCarID();
            if(usingCar === this.curCarID)
            {
                //出战中,点击无效
                return;
            }
        }
        else if(this.type === 2)
        {
            let usingMotion: number = AccountData.GetInstance().GetUsedMotionID();
            if(usingMotion === this.curMotionID)
            {
                //出战中,点击无效
                return;
            }
        }

        if(this.isUnlock())
        {
            //已解锁
            this.useItem();
        }
        else
        {
            //未解锁
            this.unlockItem(true);
        }
    }

    //使用选中的道具
    private useItem()
    {
        let itemID: number = this.getSelectedItemID();

        let soleID: number = ItemSystem.GetInstance().GetSoleIDByItemID(itemID);
        if(soleID !== -1)
        {
            if(this.type === 1)
            {
                Socket_SendMgr.GetCommonSend().SendInUseCarID(soleID);
            }
            else if(this.type === 2)
            {
                Socket_SendMgr.GetCommonSend().SendUseMotionMessage(soleID);
            }
        }
    }

    //解锁选中的道具
    private unlockItem(isFirst: boolean)
    {
        let self = this;

        let itemID: number = this.getSelectedItemID();

        let carInfo: CarTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(itemID);

        if(carInfo)
        {
            if(carInfo.GetConditionID() == CarUnlockCon.CUC_Invite)
            {
                //邀请获得
                WXHelper.ForHelpFriendShare();
                return;
            }

            if(carInfo.GetConditionID() == CarUnlockCon.CUC_TryPlay)
            {
                //试玩获得
                this.tryItemID = itemID;

                this.tryGameID = TaskSystem.GetInstance().GetNotComplateTryTask();
                GameJump.JupmGame(this.tryGameID);

                return;
            }

            if(carInfo.GetConditionID() == CarUnlockCon.CUC_Video && isFirst)
            {
                AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video_Unlock_Car, function(isFinish)
                {
                    if(isFinish)
                    {
                        self.unlockItem(false);
                    }
                });
                return;
            }

            //购买道具
            Socket_SendMgr.GetVasSend().SendBuyItemMessage(itemID);
        }
    }

    //解锁道具成功
    private unlockItemSuc(itemID: number)
    {
        if(ItemSystem.GetInstance().GetTypeByItemID(itemID) === ItemType.IT_CarType)
        {
            this.showCarView();
        }
        else if(ItemSystem.GetInstance().GetTypeByItemID(itemID) === ItemType.IT_MotionType)
        {
            this.showMotionView();
        }

        this.dispatchEvent(MessageType.StartScene_RefreshCarList);
        this.dispatchEvent(MessageType.StartScene_RefreshTopView);

        GameUI.ShowUI(UIType.UT_UnlockSucView, itemID);
    }

    //解锁道具失败
    private unlockItemFail(itemID: number)
    {
        let itemInfo: CarTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(itemID);
        if(itemInfo)
        {
            //解锁条件
            let unlockCon: number = itemInfo.GetConditionID();

            switch(unlockCon)
            {
                // case CarUnlockCon.CUC_Invite:   //邀请好友解锁
                case CarUnlockCon.CUC_ZuanShi5: //钻石5解锁
                case CarUnlockCon.CUC_Video: //看视频解锁
                {
                    //解锁失败
                    GameUI.ShowHint("解锁失败");
                    break;
                }
                case CarUnlockCon.CUC_Gold:     //金币解锁
                case CarUnlockCon.CUC_Debris: //碎片解锁
                {
                    GameUI.ShowUI(UIType.UT_UnlockFailView, itemID);
                    break;
                }
            }
        }
    }

    //使用车辆成功
    private useCarSuc()
    {
        this.showCarView();
    }
    
    //使用拖尾成功
    private useMotionSuc()
    {
        this.showMotionView();
    }

    //试玩成功
    private tryPlaySuc(gameID: number)
    {
        if(this.tryGameID != gameID)
        {
            return;
        }

        let self = this;

        setTimeout(() => {
            //购买道具
            Socket_SendMgr.GetVasSend().SendBuyItemMessage(self.tryItemID);
        }, 1000);
    }

    private refreshRemainTimeUI()
    {
        this.remainTimeNode.active = false;

        let curItemID: number = 0;

        if(this.type === 1)
        {
            curItemID = this.curCarID;
        }
        else if(this.type === 2)
        {
            curItemID = this.curMotionID;
        }

        if(ItemSystem.GetInstance().IsTimeLimitItem(curItemID))
        {
            let itemData: ItemData = ItemManager.GetInstance().GetItemByItemID(curItemID);
            if(itemData)
            {
                this.remainTimeNode.active = true;

                this.refreshRemainTime();
            }
        }
    }

    private refreshRemainTime()
    {
        if(this.remainTimeNode.active === true)
        {
            let curItemID: number = 0;

            if(this.type === 1)
            {
                curItemID = this.curCarID;
            }
            else if(this.type === 2)
            {
                curItemID = this.curMotionID;
            }

            if(ItemSystem.GetInstance().IsTimeLimitItem(curItemID))
            {
                let itemData: ItemData = ItemManager.GetInstance().GetItemByItemID(curItemID);
                if(itemData)
                {
                    let liftTime: number = itemData.GetLiftTime();
                    EngineUtil.SetNodeText(this.remainTime, Util.ConvertTimeString(liftTime));
                }
            }
        }
    }

    //判断当前选中的道具是否已解锁
    private isUnlock()
    {
        let itemID: number = this.getSelectedItemID();

        return ItemSystem.GetInstance().IsHasItemByItemID(itemID);
    }

    //获取当前选中的道具id
    private getSelectedItemID()
    {
        if(this.type === 1)
        {
            return this.curCarID;
        }
        else if(this.type === 2)
        {
            return this.curMotionID;
        }
        return 0;
    }
}