import EventCenter, { MessageType } from '../common/EventCenter';
import CarItemView from './CarItemView';

const {ccclass, property} = cc._decorator;

@ccclass
export default class CarLibItemView extends CarItemView
{
    protected btnClicked()
    {
        EventCenter.DispatchEvent(MessageType.CarLibBtnClicked, this.carID);
    }
}