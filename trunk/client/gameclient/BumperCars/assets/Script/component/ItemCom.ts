import {ItemTable} from "../table/OreItemTable";
import EngineUtil from "../common/EngineUtil";
import GameLog from '../Log/LogTool';
import { AtlasType } from '../common/EngineUtil';

const {ccclass, property} = cc._decorator;

@ccclass
export default class ItemCom extends cc.Component{
    
    private typeID: number = null

    start () {

    }

    init(){
        
    }

    private updateTexture(){
        let itemInfo = this.getItemInfoByID(this.typeID);

        if(!itemInfo){
            GameLog.PrintLog("道具id无效:", this.typeID);  
            return;  
        }

        EngineUtil.SetNodeTextureByType(AtlasType.AT_GameMap, this.node, itemInfo.Icon);
    }

    public setType(typeID: number)
    {
        this.typeID = typeID;

        this.updateTexture();
    }

    public getType(): number
    {
        return this.typeID;
    }

    private getItemInfoByID (id: number): any
    {
        for(let i = 0; i < ItemTable.length; i ++){
            if(ItemTable[i].ID === (id + "")){
                return ItemTable[i];
            }
        }
        return null;
    };
}
