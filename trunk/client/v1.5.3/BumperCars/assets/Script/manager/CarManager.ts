import AICom from '../component/AICom';
import CarCom from '../component/CarCom';
import EngineUtil from '../common/EngineUtil';
import GlobalVar from '../GlobalVar';

const {ccclass, property} = cc._decorator;

@ccclass
export default class CarManager extends cc.Component{

    private _index = null;

    private cars = null;

    init () {
        this._index = 0;

        this.cars = {};
    }

    doStart(){
        for(let key in this.cars){
            let carNode = this.cars[key];
            if(carNode){
                let carCom:CarCom = <CarCom>EngineUtil.GetComponent(carNode, "CarCom");
                if(carCom){
                    carCom.doStart();
                }
                let aiCom:AICom = <AICom>EngineUtil.GetComponent(carNode, "AICom");
                if(aiCom){
                    aiCom.doStart();
                }
            }
        }
    }

    addCar(car){
        // let carCom = EngineUtil.GetComponent(car, 'CarCom');

        // if(carCom){
        //     carCom.setID(++ this._index);
        // }
        if(this.cars && car){
            this.cars[car.userID] = car;
        }
    }

    getCarByID(id){
        if(this.cars && this.cars[id]){
            return this.cars[id];
        }
        return null;
    }

    removeCar(id){
        if(this.cars){
            let car = this.cars[id];
            if(car){
                car.destroy();
                delete this.cars[id];
            }
        }
    }

    clearAll(){
        if(this.cars){
            for(let key in this.cars){
                this.cars[key].destroy();
            }
        }

        this.cars = null;
    }

    //获得车的数量
    getCarCount(){
        if(this.cars){
            return Object.getOwnPropertyNames(this.cars).length
        }
        return 0;
    }

    getCarByIndex(index){
        if(this.cars && this.cars[index]){
            return this.cars[index];
        }
        return null;
    }

    //根据id判断是否为自己的车
    isSelf(id){
        return id == GlobalVar.userID;
    }

    onDestroy()
    {
        this.cars = null;
    }
}