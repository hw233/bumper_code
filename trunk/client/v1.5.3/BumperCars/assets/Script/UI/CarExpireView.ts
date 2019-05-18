import PopUpBase from './PopUpBase';
import EngineUtil from '../common/EngineUtil';
import GameUI from '../manager/UIManager';
import { UIType } from './UIDefine';
import CarExpireItem from '../component/CarExpireItem';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import Util from '../common/Util';
import { MessageType } from '../common/EventCenter';
import AudioManager from '../common/AudioManager';
import AdvertiseMgr from '../Advertise/AdvertiseMgr';
import { AdvertiseType } from '../Advertise/AdvertiseMgr';


const {ccclass, property} = cc._decorator;

@ccclass
export default class CarExpireView extends PopUpBase
{
    @property(cc.Node)
    private carList: cc.Node                = null;

    @property(cc.Node)
    private content: cc.Node                = null;

    @property(cc.Node)
    private carNode: cc.Node                = null;

    @property(cc.Node)
    private tryUseBtn: cc.Node              = null;

    @property(cc.Node)
    private buyBtn: cc.Node                 = null;

    private isSingle: boolean               = null;

    public OnInit()
    {
        super.OnInit();

        this.isSingle = false;

        this.registerEvent(MessageType.Item_Try_Suc_Event, this.trySuc.bind(this));

        this.registerClickEvent(this.tryUseBtn, this.tryUseBtnClicked.bind(this));
        this.registerClickEvent(this.buyBtn, this.buyBtnClicked.bind(this));
    }

    public RefreshView()
    {
        super.RefreshView();

        this.content.removeAllChildren();

        let carList = this.param;

        if(Util.IsArray(carList))
        {
            this.isSingle = false;

            this.carList.active = true;
            this.carNode.active = false;
            if(carList.length > 0)
            {
                for(let i = 0, len = carList.length; i < len; i ++)
                {
                    let carPrefab: cc.Prefab = EngineUtil.GetPrefab("CarExpireItem");
                    if(carPrefab)
                    {
                        let carNode: cc.Node = cc.instantiate(carPrefab);
                        if(carNode)
                        {
                            carNode.userID = carList[i];

                            let toggle: cc.Toggle = carNode.getChildByName("Toggle").getComponent(cc.Toggle);
                            toggle.toggleGroup = this.content.getComponent(cc.ToggleGroup);
        
                            this.content.addChild(carNode);

                            let itemCom: CarExpireItem = carNode.getComponent(CarExpireItem);
                            if(itemCom)
                            {
                                itemCom.refreshView(carList[i]);
                            }
                        }
                    }
                }
            }
        }
        else
        {
            this.isSingle = true;

            this.carNode.active = true;
            this.carList.active = false;

            let itemCom: CarExpireItem = this.carNode.getComponent(CarExpireItem);
            if(itemCom)
            {
                itemCom.refreshView(Number(carList));
            }
        }
    }

    private tryUseBtnClicked()
    {
        let self = this;

        AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video_JiaYou, function(isFinish){
            if(isFinish)
            {
                let selectCar: cc.Node = self.getSelectedCarNode();
                if(selectCar)
                {
                    let carExpireItem: CarExpireItem = selectCar.getComponent(CarExpireItem); 
                    if(carExpireItem)
                    {
                        let carID: number = carExpireItem.soleID;
                        Socket_SendMgr.GetActionSend().SendSkinTryUseMessage(carID, false);
                    }
                }
            }
        });
    }

    private buyBtnClicked()
    {
        this.deleteItem();

        GameUI.ShowUI(UIType.UT_CarLibView);
        GameUI.RemoveUI(UIType.UT_TryUseFinishView);
    }

    private trySuc(data: any)
    {
        // let soleID: number = data.itemID
        // if(this.isSingle)
        // {
        //     GameUI.RemoveUI(UIType.UT_TryUseFinishView);
        // }
        // else
        // {
        //     if(this.content.childrenCount <= 1)
        //     {
        //         GameUI.RemoveUI(UIType.UT_TryUseFinishView);
        //     }
        //     else
        //     {
        //         for(let i = 0, len = this.content.childrenCount;i < len; i ++)
        //         {
        //             let child: cc.Node = this.content.children[i];
        //             if(child)
        //             {
        //                 if(child.userID === soleID)
        //                 {
        //                     child.destroy();
        //                     break;
        //                 }
        //             }
        //         }
        //     }
        // }
    }

    //获取选中的车辆
    private getSelectedCarNode()
    {
        if(this.isSingle)
        {
            if(this.carNode)
            {
                return this.carNode;
            }
        }
        else
        {
            if(this.content)
            {
                for(let i = 0, len = this.content.childrenCount; i < len; i ++)
                {
                    let child: cc.Node = this.content.children[i];
                    if(child)
                    {
                        let toggle: cc.Toggle = child.getChildByName("Toggle").getComponent(cc.Toggle);
                        if(toggle)
                        {
                            if(toggle.isChecked)
                            {
                                return child;
                            }
                        }
                    }
                }
            }
        }
        return null;
    }

    private deleteItem()
    {
        if(this.isSingle)
        {
            let itemCom: CarExpireItem = this.carNode.getComponent(CarExpireItem);
            if(itemCom)
            {
                Socket_SendMgr.GetItemSend().SendDeleteItemMessage(itemCom.soleID);
            }
        }
        else
        {
            if(this.content)
            {
                for(let i = 0, len = this.content.childrenCount;i < len;i ++)
                {
                    let child: cc.Node = this.content.children[i];
                    if(child)
                    {
                        let itemCom: CarExpireItem = child.getComponent(CarExpireItem);
                        if(itemCom)
                        {
                            Socket_SendMgr.GetItemSend().SendDeleteItemMessage(itemCom.soleID);
                        }
                    }
                }
            }
        }
    }

    protected closeBtnClicked()
    {
        AudioManager.PlayBtnSound();

        this.deleteItem();

        GameUI.RemoveUI(this.GetType());
    }

    public OnExit()
    {
        super.OnExit();

        this.content.removeAllChildren();
    }
}